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

void UDMSNotifyManager::ActivateNextForced(ADMSSequence* Sequence, const FSimpleDelegate& OnForcedFinished) {

	FString InTimingStr = Sequence->GetCurrentProgressTag().ToString();
	//FString InTimingStr = UDMSCoreFunctionLibrary::GetTimingString(Sequence->GetCurrentProgress());
	if(ForcedEIMap[Sequence].Count == ForcedEIMap[Sequence].ForcedObjects.Num()) {
		DMS_LOG_SIMPLE(TEXT("==== %s [%s] : NO MORE FORCED EFFECT  ===="), *Sequence->GetName(), *InTimingStr);
		ForcedFinished(Sequence, OnForcedFinished);
	}
	else {
		DMS_LOG_SIMPLE(TEXT("==== %s [%s] : ACTIVATE NEXT FORCED EFFECT  ===="), *Sequence->GetName(), *InTimingStr);

		// Create Applying AE from Responsed Pesistent AE.
		auto& CurrentForcedPair = ForcedEIMap[Sequence].ForcedObjects[ForcedEIMap[Sequence].Count];
		auto NewSeq = CurrentForcedPair.Value->CreateApplyingSequence(CurrentForcedPair.Key, Sequence);
		NewSeq->AddToOnSequenceFinished_Native([this, Sequence, OnForcedFinished](bool) {
			ActivateNextForced(Sequence,OnForcedFinished); // :: RECURSIVE ::
			DMS_LOG_SIMPLE(TEXT("==== %s : after forced EI lambda ===="), *Sequence->GetName());
			});
		ForcedEIMap[Sequence].Count++;
		UDMSCoreFunctionLibrary::GetDMSSequenceManager()->RunSequence(NewSeq);
	}
}

void UDMSNotifyManager::ForcedFinished(ADMSSequence* Sequence, const FSimpleDelegate& OnForcedFinished) 
{
	FString InTimingStr = Sequence->GetCurrentProgressTag().ToString();
	//FString InTimingStr = UDMSCoreFunctionLibrary::GetTimingString(Sequence->GetCurrentProgress());
	DMS_LOG_SIMPLE(TEXT("==== %s [%s] : FORCED EFFECT FINISHED ===="), *Sequence->GetName(), *InTimingStr);
	// Cleanup Forced queue.
	auto temp = std::move(ForcedEIMap[Sequence]);
	ForcedEIMap.Remove(Sequence);
	// Continue to user selectable effects.
	CreateRespondentSelector(Sequence, temp.NonForcedObjects,OnForcedFinished);
}

void UDMSNotifyManager::Broadcast(ADMSSequence* NotifyData, const FSimpleDelegate& ResponseCompleted)
{
	DMS_LOG_SIMPLE(TEXT("==== %s : BROADCASTING  ===="), *NotifyData->GetName());
	FString TimingStr = NotifyData->GetCurrentProgressTag().ToString();
	//FString TimingStr = UDMSCoreFunctionLibrary::GetTimingString(NotifyData->GetCurrentProgress());

	if (NotifyData->SequenceState == EDMSSequenceState::SS_Canceled) {
		DMS_LOG_SIMPLE(TEXT("Sequence is canceled"));
		ResponseCompleted.ExecuteIfBound();
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

	ForcedEIMap.Add(NotifyData);

	// Search forced effects
	TArray<TScriptInterface<IDMSEffectorInterface>> Keys;
	ResponsedObjects.GetKeys(Keys);
	for (auto& ResponsedObject : Keys) {
		TArray<ADMSActiveEffect*> EffectInstances;
		ResponsedObjects.MultiFind(ResponsedObject, EffectInstances, true);

		for (auto EI : EffectInstances) {
			if (EI->EffectNode->bForced) {
				TPair<AActor*, ADMSActiveEffect*> NewValue;
				NewValue.Key = ResponsedObject->GetObject(); NewValue.Value = EI;
				ForcedEIMap[NotifyData].ForcedObjects.Add(std::move(NewValue));
				ResponsedObjects.Remove(ResponsedObject, EI);
			}
		}
	}

	// Process forced effect first, followed by processing the selectable.
	if (ForcedEIMap[NotifyData].ForcedObjects.Num() != 0)
	{
		// Resolve forced effect
		DMS_LOG_SIMPLE(TEXT("==== %s [%s] : ACTIVATING FORCED EFFECT START  ===="), *NotifyData->GetName(), *TimingStr);

		ForcedEIMap[NotifyData].NonForcedObjects = ResponsedObjects;
		ActivateNextForced(NotifyData, ResponseCompleted);
	}
	else 
	{
		// No remaining forced effect
		DMS_LOG_SIMPLE(TEXT("==== %s [%s] : NO FORCED EFFECT  ===="), *NotifyData->GetName(), *TimingStr);

		ForcedEIMap.Remove(NotifyData);

		// Selectable effects.
		CreateRespondentSelector(NotifyData, ResponsedObjects, ResponseCompleted);
	}
}

bool UDMSNotifyManager::RegisterNotifyObject(TScriptInterface<IDMSEffectorInterface> Object)
{
	// Validation
	if(Object.GetObject() == nullptr || !Object.GetObject()->Implements<UDMSEffectorInterface>() ) {DMS_LOG_SIMPLE(TEXT("NotifyManager : Register invalid item")); return false; }
	if(NotifyObjects.Find(Object)!=INDEX_NONE) { DMS_LOG_SIMPLE(TEXT("NotifyManager : Register duplicate item")); return false;}
	
	NotifyObjects.Add(Object);
	return true;
}

void UDMSNotifyManager::CreateRespondentSelector(ADMSSequence* CurrentSequence, TMultiMap<TScriptInterface<IDMSEffectorInterface>, ADMSActiveEffect*>& ResponsedObjects, const FSimpleDelegate& ResponseCompleted)
{
	//auto GS = Cast<ADMSGameModeBase>(GetWorld()->GetAuthGameMode())->GetDMSGameState();
	//auto SelM = GS->GetSelectorManager();
	auto SelM = UDMSCoreFunctionLibrary::GetDMSSelectorManager(this); check(SelM);
	FString TimingStr = CurrentSequence->GetCurrentProgressTag().ToString();
	//FString TimingStr = UDMSCoreFunctionLibrary::GetTimingString(CurrentSequence->GetCurrentProgress());
	DMS_LOG_SIMPLE(TEXT("==== %s [%s] : Create Respondent Selector  ===="), *CurrentSequence->GetName(), *TimingStr);

	if (ResponsedObjects.Num() == 0) {

		DMS_LOG_SIMPLE(TEXT("==== %s [%s] : NO Respondent / EXE OnResponseCompleted ===="), *CurrentSequence->GetName(), *TimingStr);
		ResponseCompleted.ExecuteIfBound();
		return;
	}

	DMS_LOG_SIMPLE(TEXT("==== %s [%s] : %d Respondent(s) ===="), *CurrentSequence->GetName(), *TimingStr, ResponsedObjects.Num());

	// TODO :: 각 오너에게 뿌리는 방식으로 변경 해야함. && 리더 플레이어를 구해오는 방식을 게임 모드에 종속적이지 않게 따로 빼야할 듯.
	ADMSPlayerControllerBase* LeaderPC = UDMSCoreFunctionLibrary::GetDMSGameState(this)->GetLeaderPlayerController();

	UDMSSelectorRequestForm_Object* NewForm = NewObject<UDMSSelectorRequestForm_Object>(this);		

	NewForm->SelectorClass = ResponsedObjectSelector;
	NewForm->SelectAmount=1;	

	TArray<TScriptInterface<IDMSEffectorInterface>> TempCandidates;
	ResponsedObjects.GetKeys(TempCandidates);

	auto SelHandle = SelM->RequestCreateSelector(NewForm);	
	SelHandle->CreateSelectorWidget(LeaderPC);
	UDMSNotifyRespondentSelector* InstancedWidget = Cast<UDMSNotifyRespondentSelector>(SelHandle->Widget);

	InstancedWidget->ResponsedObjects = ResponsedObjects; 
	ResponsedObjects.GetKeys(InstancedWidget->Respondents);
	InstancedWidget->CurrentSequence = CurrentSequence;

	// 노티파이 셀렉터는 특수한 케이스로 델리게이트 수행이 아닌
	// 자체적으로 위젯이 업데이트 데이터 (UDMSNotifyRespondentSelector::UpdateData)를 실행하는 것으로

	SelHandle->SetupDelegates([=,NotifyManager=this]() {
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
			ResponseCompleted.ExecuteIfBound();
			InstancedWidget->CloseSelector();
			return;
		}

		// prepare for resume. ( we'll check again but except responded one.)

		// change this to per active effect.
		if(!EffectInstance->EffectNode->bCanResponseMulTime)
			InstancedWidget->ResponsedObjects.Remove(Respondent,EffectInstance);
		TArray<ADMSActiveEffect*> NewRespondents;
		InstancedWidget->ResponsedObjects.GenerateValueArray(NewRespondents);

		//TArray<TScriptInterface<IDMSEffectorInterface>> NewRespondents;
		//InstancedWidget->ResponsedObjects.GetKeys(NewRespondents);

		ADMSSequence* NewSeq = EffectInstance->CreateApplyingSequence(Respondent->GetObject(), InstancedWidget->CurrentSequence);

		NewSeq->AddToOnSequenceFinished_Native([=, ResumingSequence= InstancedWidget->CurrentSequence](bool PreviousResult){

			// Replay response 

			// NOTE :: What can we do with the Result of previous response? ( PreviousResult )

			TMultiMap<TScriptInterface<IDMSEffectorInterface>, ADMSActiveEffect*> LocalNRO;

			if (ResumingSequence->SequenceState == EDMSSequenceState::SS_Canceled) {
				DMS_LOG_SIMPLE(TEXT("Resumed sequence was canceled"));
				ResponseCompleted.ExecuteIfBound();
				return;
			}

			// Check again. ( Apply changed env )
			//for (auto Object : NewRespondents)
			//{
			//	if(Object->GetEffectorManagerComponent())
			//		Object->GetEffectorManagerComponent()->OnNotifyReceived(LocalNRO, ResumingSequence->OriginalEffectNode->bIsChainableEffect, ResumingSequence);
			//}

			for (auto AE : NewRespondents)
			{
				AE->ReceiveNotify(LocalNRO, ResumingSequence->OriginalEffectNode->bIsChainableEffect, ResumingSequence);
			}

			DMS_LOG_SIMPLE(TEXT("==== %s : RESUME RESPONSE ===="), *ResumingSequence->GetName());

			NotifyManager->CreateRespondentSelector(ResumingSequence, LocalNRO,ResponseCompleted);
		});		

		UDMSCoreFunctionLibrary::GetDMSSequenceManager(NotifyManager)->RunSequence(NewSeq);

		//InstancedWidget->CurrentSequence->OnSequenceFinish();
		InstancedWidget->CloseSelector();
		//...
	}, [=, NotifyManager=this]() {
		// [ X Bttn ]

		// Player choose to not run EI
		ResponseCompleted.ExecuteIfBound();
		//InstancedWidget->CurrentSequence->OnSequenceFinish();
		InstancedWidget->CloseSelector();

	});
	SelHandle->SetupSelector();
	SelHandle->RunSelector();
}


void UDMSNotifyRespondentSelector::UpdateData(UObject* Respondent, UObject* EffectInstance)
{
	auto SeqAttComp = CurrentSequence->GetComponentByClass<UDMSAttributeComponent>();

	auto RespondentAtt = SeqAttComp->MakeAttribute(FGameplayTagContainer(TAG_DMS_System_Notify_Respondent),UDMSAttributeValue_Object::StaticClass());
	auto AEAtt = SeqAttComp->MakeAttribute(FGameplayTagContainer(TAG_DMS_System_Notify_Respondent),UDMSAttributeValue_Object::StaticClass());

	if(!RespondentAtt || !AEAtt)
	{
		DMS_LOG_SIMPLE(TEXT("UDMSNotifyRespondentSelector :: Update data Failed"));
		return; /* LOG? */
	}
	Cast<UDMSAttributeValue_Object>(RespondentAtt->AttributeValue)->AddValue(Respondent);
	Cast<UDMSAttributeValue_Object>(AEAtt->AttributeValue)->AddValue(EffectInstance);

	//UDMSDataObjectSet* UpdatingData = CurrentSequence->SequenceDatas;
	//UpdatingData->SetData(TAG_DMS_System_Notify_Respondent, Respondent);
	//UpdatingData->SetData(TAG_DMS_System_Notify_ActivatingEffect, EffectInstance);
}

void UDMSNotifyRespondentSelector::GetEffectInstancesFromObject(TScriptInterface<IDMSEffectorInterface> iObject, TArray<ADMSActiveEffect*>& outArray)
{
	ResponsedObjects.MultiFind(iObject, outArray,true);
	//(*ForcedObjects.Find(iObject))->CreateApplyingSequence(iObject.GetObject(), CurrentSequence);
	//return;
}
