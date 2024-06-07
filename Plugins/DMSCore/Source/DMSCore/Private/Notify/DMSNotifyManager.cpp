// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/DMSNotifyManager.h"
#include "Effect/DMSEIManagerComponent.h"
#include "Effect/DMSEffectorInterface.h"
#include "Effect/DMSEffectDefinition.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Attribute/DMSAttributeValue_Object.h"

#include "GameModes/DMSGameStateBase.h"
#include "Sequence/DMSSeqManager.h"
#include "Sequence/DMSSequenceStep.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"
#include "Player/DMSPlayerControllerBase.h"

#include "Selector/DMSSelectorManager.h"
#include "Selector/DMSDecisionDefinition_Object.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_System_Notify_Respondent, "System.Notify.Respondent");
UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_System_Notify_ActivatingEffect, "System.Notify.ActivatingEffect");

void UDMSForcedEffectWorker::SetupForcedWorker(ADMSSequence* iSequence)
{
	SourceSequence = iSequence;
}

void UDMSForcedEffectWorker::Work_Implementation()
{		
	auto CurrentForcedPair = Cast<UDMSForcedEffectContext>(GetCurrentContext());
	auto ActiveEffect = CurrentForcedPair->ActiveEffect;
	auto Respondent = CurrentForcedPair->Respondent;
	auto NewSeq = ActiveEffect->CreateApplyingSequence(Respondent->GetObject(), SourceSequence);
	NewSeq->AddToOnSequenceFinished_Native([this](bool) {
		this->CompleteSingleTask(true); // :: RECURSIVE ::
	});
	UDMSCoreFunctionLibrary::GetDMSSequenceManager(this)->RunSequence(NewSeq);
}

void UDMSNotifyManager::Broadcast(ADMSSequence* NotifyData, const FOnTaskCompletedNative& ResponseCompleted)
{
	DMS_LOG_SIMPLE(TEXT("==== %s : BROADCASTING  ===="), *NotifyData->GetName());
	FString TimingStr = NotifyData->GetCurrentProgressExactTag().ToString();
	//FString TimingStr = UDMSCoreFunctionLibrary::GetTimingString(NotifyData->GetCurrentProgress());

	if (NotifyData->SequenceState == EDMSSequenceState::SS_Canceled || 
		NotifyData->GetInstancedStep()->GetCurrentProgressData().ProgressBroadcastFlag == EDMSBroadCastFlag::BF_NoBroadcast ) {
		DMS_LOG_SIMPLE(TEXT("Sequence is canceled or broadcast mode is NoBroadcast"));
		ResponseCompleted.ExecuteIfBound(true);
		return;
	}

	TMultiMap<TScriptInterface<IDMSEffectorInterface>, ADMSActiveEffect*> ResponsedObjects;

	// Collect responsed objects 
	for (auto& Object : NotifyObjects)
	{
		if (Object->GetEffectorManagerComponent())
			//DMS_LOG_SIMPLE(TEXT("%s"), (Object==nullptr) ? TEXT("Object is NULLPTR") : *(Object->GetObject()->GetName()));
			Object->GetEffectorManagerComponent()->OnNotifyReceived(ResponsedObjects, NotifyData->IsChainableSequence(), NotifyData);
	}

	// Search forced effects
	TArray<TScriptInterface<IDMSEffectorInterface>> Keys;
	ResponsedObjects.GetKeys(Keys);

	TArray<UObject*> ForcedContexts;

	for (auto& ResponsedObject : Keys) 
	{
		TArray<ADMSActiveEffect*> ActiveEffects;
		ResponsedObjects.MultiFind(ResponsedObject, ActiveEffects, true);

		for (auto AE : ActiveEffects) 
		{
			if(AE->EffectNode->bForced)
			{
				UDMSForcedEffectContext* NewContext = NewObject<UDMSForcedEffectContext>(this);
				NewContext->ActiveEffect = AE;
				NewContext->Respondent = ResponsedObject;
				ForcedContexts.Add(NewContext);
				ResponsedObjects.Remove(ResponsedObject,AE);
			}
		}
	}

	UDMSForcedEffectWorker* ForcedWorker = NewObject<UDMSForcedEffectWorker>(this);
	FOnTaskCompletedNative OnForcedEffectCompleted;

	OnForcedEffectCompleted.BindLambda([=](bool){
		auto CapturedRO = ResponsedObjects;
		CreateRespondentSelector(NotifyData, CapturedRO, ResponseCompleted);
	});

	ForcedWorker->SetupForcedWorker(NotifyData);
	ForcedWorker->SetupTaskWorkerDelegate_Native(ForcedContexts, OnForcedEffectCompleted);
	ForcedWorker->RunTaskWorker(false);
}

void UDMSNotifyManager::CreateRespondentSelector(ADMSSequence* CurrentSequence, TMultiMap<TScriptInterface<IDMSEffectorInterface>, ADMSActiveEffect*>& ResponsedObjects, const FOnTaskCompletedNative& ResponseCompleted)
{
	//auto GS = Cast<ADMSGameModeBase>(GetWorld()->GetAuthGameMode())->GetDMSGameState();
	//auto SelM = GS->GetSelectorManager();
	auto SelM = UDMSCoreFunctionLibrary::GetDMSSelectorManager(this); check(SelM);
	FString TimingStr = CurrentSequence->GetCurrentProgressExactTag().ToString();
	//FString TimingStr = UDMSCoreFunctionLibrary::GetTimingString(CurrentSequence->GetCurrentProgress());
	DMS_LOG_SIMPLE(TEXT("==== %s [%s] : Create Respondent Selector  ===="), *CurrentSequence->GetName(), *TimingStr);

	if (ResponsedObjects.Num() == 0) 
	{
		DMS_LOG_SIMPLE(TEXT("==== %s [%s] : NO Respondent / EXE OnResponseCompleted ===="), *CurrentSequence->GetName(), *TimingStr);
		ResponseCompleted.ExecuteIfBound(true);
		return;
	}

	DMS_LOG_SIMPLE(TEXT("==== %s [%s] : %d Respondent(s) ===="), *CurrentSequence->GetName(), *TimingStr, ResponsedObjects.Num());

	// TODO :: 각 오너에게 뿌리는 방식으로 변경 해야함. && 리더 플레이어를 구해오는 방식을 게임 모드에 종속적이지 않게 따로 빼야할 듯.
	ADMSPlayerControllerBase* LeaderPC = UDMSCoreFunctionLibrary::GetDMSGameState(this)->GetLeaderPlayerController();

	UDMSSelectorRequestForm_Object* NewForm = NewObject<UDMSSelectorRequestForm_Object>(this);		

	NewForm->SelectorClass = ResponsedObjectSelector;
	NewForm->SelectAmount = 1;	

	auto SelHandle = SelM->RequestCreateSelector(NewForm);	
	SelHandle->CreateSelectorWidget(LeaderPC);
	UDMSNotifyRespondentSelector* InstancedWidget = Cast<UDMSNotifyRespondentSelector>(SelHandle->Widget);

	InstancedWidget->InitRespondentSelector(CurrentSequence, ResponsedObjects); 

	// 노티파이 셀렉터는 특수한 케이스로 델리게이트 수행이 아닌
	// 자체적으로 위젯이 업데이트 데이터 (UDMSNotifyRespondentSelector::UpdateData)를 실행하는 것으로
	FOnSelectCompletedNative RespondentSelectCompleted;
	RespondentSelectCompleted.BindLambda([=,NotifyManager=this](bool Succeed, const TArray<uint8>& Indexes){

		if(Succeed){
			// [ OK Bttn ]

			auto OwnerSequence = InstancedWidget->CurrentSequence;
			auto SequenceAttComp = OwnerSequence->GetComponentByClass<UDMSAttributeComponent>();
			auto RespondentAttribute = SequenceAttComp->GetTypedAttributeValue<UDMSAttributeValue_Object>(FGameplayTagContainer(TAG_DMS_System_Notify_Respondent));
			auto EffectInstanceAttribute = SequenceAttComp->GetTypedAttributeValue<UDMSAttributeValue_Object>(FGameplayTagContainer(TAG_DMS_System_Notify_ActivatingEffect));

			TScriptInterface<IDMSEffectorInterface> Respondent = RespondentAttribute ? RespondentAttribute->GetValue()[0] : nullptr;
			ADMSActiveEffect* EffectInstance = EffectInstanceAttribute ? Cast<ADMSActiveEffect>(EffectInstanceAttribute->GetValue()[0]) : nullptr;
			if (EffectInstance == nullptr)
			{
				// Widget didn't made proper data.
				ResponseCompleted.ExecuteIfBound(true);
				InstancedWidget->CloseSelector();
				return;
			}

			// prepare for resume. ( we'll check again but except responded one.)

			// change this to per active effect.
			if(!EffectInstance->EffectNode->bCanResponseMulTime)
				InstancedWidget->ResponsedObjects.Remove(Respondent,EffectInstance);
			TArray<ADMSActiveEffect*> NewRespondents;
			InstancedWidget->ResponsedObjects.GenerateValueArray(NewRespondents);

			ADMSSequence* NewSeq = EffectInstance->CreateApplyingSequence(Respondent->GetObject(), InstancedWidget->CurrentSequence);

			NewSeq->AddToOnSequenceFinished_Native([=, ResumingSequence= InstancedWidget->CurrentSequence](bool PreviousResult){
				// Replay response 
				// NOTE :: What can we do with the Result of previous response? ( PreviousResult )
				TMultiMap<TScriptInterface<IDMSEffectorInterface>, ADMSActiveEffect*> LocalNRO;

				if (ResumingSequence->SequenceState == EDMSSequenceState::SS_Canceled) 
				{
					DMS_LOG_SIMPLE(TEXT("Resumed sequence was canceled"));
					ResponseCompleted.ExecuteIfBound(true);
					return;
				}

				for (auto AE : NewRespondents)
				{
					AE->ReceiveNotify(LocalNRO, ResumingSequence->OriginalEffectNode->bIsChainableEffect, ResumingSequence);
				}

				DMS_LOG_SIMPLE(TEXT("==== %s : RESUME RESPONSE ===="), *ResumingSequence->GetName());

				NotifyManager->CreateRespondentSelector(ResumingSequence, LocalNRO,ResponseCompleted);
				});		

			UDMSCoreFunctionLibrary::GetDMSSequenceManager(NotifyManager)->RunSequence(NewSeq);

			InstancedWidget->CloseSelector();
			//...
		}
		else 
		{
			// [ X Bttn ]

			// Player choose to not run EI
			ResponseCompleted.ExecuteIfBound(true);
			//InstancedWidget->CurrentSequence->OnSequenceFinish();
			InstancedWidget->CloseSelector();

		}	
	});

	SelHandle->SetupDelegate(RespondentSelectCompleted);
	SelHandle->SetupSelector();
	SelHandle->RunSelector();
}

bool UDMSNotifyManager::RegisterNotifyObject(TScriptInterface<IDMSEffectorInterface> Object)
{
	// Validation
	if(Object.GetObject() == nullptr || !Object.GetObject()->Implements<UDMSEffectorInterface>() ) {DMS_LOG_SIMPLE(TEXT("NotifyManager : Register invalid item")); return false; }
	if(NotifyObjects.Find(Object)!=INDEX_NONE) { DMS_LOG_SIMPLE(TEXT("NotifyManager : Register duplicate item")); return false;}

	NotifyObjects.Add(Object);
	return true;
}

void UDMSNotifyRespondentSelector::UpdateData(UObject* Respondent, UObject* EffectInstance)
{
	auto SeqAttComp = CurrentSequence->GetComponentByClass<UDMSAttributeComponent>();

	auto RespondentAtt = SeqAttComp->MakeAttribute(FGameplayTagContainer(TAG_DMS_System_Notify_Respondent),UDMSAttributeValue_Object::StaticClass());
	auto AEAtt = SeqAttComp->MakeAttribute(FGameplayTagContainer(TAG_DMS_System_Notify_ActivatingEffect),UDMSAttributeValue_Object::StaticClass());

	if(!RespondentAtt || !AEAtt)
	{
		DMS_LOG_SIMPLE(TEXT("UDMSNotifyRespondentSelector :: Update data Failed"));
		return; /* LOG? */
	}
	Cast<UDMSAttributeValue_Object>(RespondentAtt->AttributeValue)->SetValue({Respondent});
	Cast<UDMSAttributeValue_Object>(AEAtt->AttributeValue)->SetValue({EffectInstance});
}

void UDMSNotifyRespondentSelector::GetEffectInstancesFromObject(TScriptInterface<IDMSEffectorInterface> iObject, TArray<ADMSActiveEffect*>& outArray)
{
	for(auto Pair : ResponsedObjects)
	{
		if(Pair.Key == iObject) outArray.Add(Pair.Value);
	}

	// Can't use multifind somehow ( Maybe key must be OBJECT? )
	//ResponsedObjects.MultiFind(iObject, outArray,true);
}

void UDMSNotifyRespondentSelector::InitRespondentSelector(ADMSSequence* iCurrentSequence, const TMultiMap<TScriptInterface<IDMSEffectorInterface>, ADMSActiveEffect*>& iResponsedObjects)
{
	ResponsedObjects=iResponsedObjects;
	ResponsedObjects.GenerateKeyArray(Respondents);
	//InstancedWidget->ResponsedObjects.GetKeys(InstancedWidget->Respondents);
	CurrentSequence = iCurrentSequence;
}
