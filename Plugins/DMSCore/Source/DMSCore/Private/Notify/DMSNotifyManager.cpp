// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/DMSNotifyManager.h"
#include "Effect/DMSEffectorInterface.h"
#include "Effect/DMSEffectDefinition.h"
#include "GameModes/DMSGameStateBase.h"
#include "Sequence/DMSSeqManager.h"
#include "Sequence/DMSSequenceStep.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"


UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_System_Notify_Respondent, "System.Notify.Respondent");
UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_System_Notify_ActivatingEffect, "System.Notify.ActivatingEffect");



bool UDMSNotifyManager::RegisterNotifyObject(TScriptInterface<IDMSEffectorInterface> Object)
{
	// Validation
	if(Object.GetObject() == nullptr || !Object.GetObject()->Implements<UDMSEffectorInterface>() ) {DMS_LOG_SIMPLE(TEXT("NotifyManager : Register invalid item")); return false; }
	if(NotifyObjects.Find(Object)!=INDEX_NONE) { DMS_LOG_SIMPLE(TEXT("NotifyManager : Register duplicate0 item")); return false;}
	
	NotifyObjects.Add(Object);
	return true;
}

void UDMSNotifyManager::CreateRespondentSelector(UDMSSequence* CurrentSequence, TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*>& ResponsedObjects)
{
	FString TimingStr = UDMSCoreFunctionLibrary::GetTimingString(CurrentSequence->GetCurrentProgress());
	DMS_LOG_SIMPLE(TEXT("==== %s [%s] : Create Respondent Selector  ===="), *CurrentSequence->GetName(), *TimingStr);

	if (ResponsedObjects.Num() == 0) {

		DMS_LOG_SIMPLE(TEXT("==== %s [%s] : NO Respondent / EXE OnResponseCompleted ===="), *CurrentSequence->GetName(), *TimingStr);
		CallResponseCompleted(CurrentSequence);
		return;
	}

	DMS_LOG_SIMPLE(TEXT("==== %s [%s] : %d Respondent(s) ===="), *CurrentSequence->GetName(), *TimingStr, ResponsedObjects.Num());

	// TODO :: 각 오너에게 뿌리는 방식으로 변경 해야함. && 리더 플레이어를 구해오는 방식을 게임 모드에 종속적이지 않게 따로 빼야할 듯.
	APlayerController* LeaderPC = UDMSCoreFunctionLibrary::GetDMSGameState()->GetLeaderPlayerController();
	//APlayerController* LeaderPC = UGameplayStatics::GetGameState(GetWorld())->PlayerArray[0]->GetPlayerController();
	UDMSNotifyRespondentSelector* InstancedWidget = CreateWidget<UDMSNotifyRespondentSelector>(LeaderPC, ResponsedObjectSelector);

	InstancedWidget->ResponsedObjects = ResponsedObjects; 
	ResponsedObjects.GetKeys(InstancedWidget->Respondents);
	InstancedWidget->OwnerSeq = CurrentSequence;
	//InstancedWidget->OutTag=TAG_DMS_System_NotifyRespondent;

	if (!InstancedWidget->SetupWidget()) {
		//Something goes wrong
		CallResponseCompleted(CurrentSequence);
		return;
	}

	InstancedWidget->SetupWidgetDelegates([InstancedWidget, NotifyManager=this](UDMSDataObjectSet* Data) {
		// [ OK Bttn ]
		TScriptInterface<IDMSEffectorInterface> Respondent = Data->GetData(TAG_DMS_System_Notify_Respondent)->Get<UObject*>();
		UDMSEffectInstance* EffectInstance = Cast<UDMSEffectInstance>(Data->GetData(TAG_DMS_System_Notify_ActivatingEffect)->Get<UObject*>());
		
		// prepare for resume. ( we'll check again but except responded one.)
		InstancedWidget->ResponsedObjects.Remove(Respondent);
		TArray<TScriptInterface<IDMSEffectorInterface>> NewRespondents;
		InstancedWidget->ResponsedObjects.GetKeys(NewRespondents);

		UDMSSequence* NewSeq = EffectInstance->CreateSequenceFromNode(Respondent.GetObject(), InstancedWidget->OwnerSeq);
		
		NewSeq->AddToOnSequenceFinished_Native([NewRespondents, ResumingSequence= InstancedWidget->OwnerSeq , NotifyManager](bool PreviousResult){
			
			// Replay response 

			// NOTE :: What can we do with the Result of previous response? ( PreviousResult )

			TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*> LocalNRO;

			if (ResumingSequence->SequenceState == EDMSSequenceState::SS_Canceled) {
				DMS_LOG_SIMPLE(TEXT("Resumed sequence was canceled"));
				NotifyManager->CallResponseCompleted(ResumingSequence);
				return;
			}

			// Check again. ( Apply changed env )
			for (auto Object : NewRespondents)
			{
				// 이거 말고 그냥 체크만 하는 함수로 재구성 하던가 포스드 일 때 강제실행을 여기서 노티파이 매니저가 하도록 해서 순차적으로 실행하게 해야함 ( 기다려주면서 ) 해야함.
				Object->OnNotifyReceived(LocalNRO, ResumingSequence->OriginalEffectNode->bIsChainableEffect, ResumingSequence);
			}

			DMS_LOG_SIMPLE(TEXT("==== %s : RESUME RESPONSE ===="), *ResumingSequence->GetName());

			NotifyManager->CreateRespondentSelector(ResumingSequence, LocalNRO);
		});		

		UDMSCoreFunctionLibrary::GetDMSSequenceManager()->RunSequence(NewSeq);

		//InstancedWidget->OwnerSeq->OnSequenceFinish();
		InstancedWidget->CloseSelector();
		//...
		}, [&,InstancedWidget]() {
			// [ X Bttn ]

			// Player choose to not run EI
			CallResponseCompleted(InstancedWidget->OwnerSeq);
			//InstancedWidget->OwnerSeq->OnSequenceFinish();
			InstancedWidget->CloseSelector();

		}
	);

	
	
	InstancedWidget->PopupSelector();
}

void UDMSNotifyManager::CallResponseCompleted(UDMSSequence* CurrentSequence)
{
	if (!OnResponseCompleted.Contains(CurrentSequence))
	{
		DMS_LOG_SIMPLE(TEXT("==== %s : Accessing to Closed OnResponseCompleted ===="), *CurrentSequence->GetName());
		return;
	}
	DMS_LOG_SIMPLE(TEXT("==== %s : Calling to OnResponseCompleted ===="), *CurrentSequence->GetName());
	auto temp = std::move(OnResponseCompleted[CurrentSequence]);
	OnResponseCompleted.Remove(CurrentSequence);
	temp.ExecuteIfBound();
	temp.Unbind();
}


UDMSDataObjectSet* UDMSNotifyRespondentSelector::MakeOutputDatas(UObject* Respondent, UObject* EffectInstance)
{
	UDMSDataObjectSet* NewSet = NewObject<UDMSDataObjectSet>(GetOwningPlayer());
	NewSet->SetData(TAG_DMS_System_Notify_Respondent, Respondent);
	NewSet->SetData(TAG_DMS_System_Notify_ActivatingEffect, EffectInstance);
	return NewSet;
}


void UDMSNotifyRespondentSelector::GetEffectInstancesFromObject(TScriptInterface<IDMSEffectorInterface> iObject, TArray<UDMSEffectInstance*>& outArray)
{
	ResponsedObjects.MultiFind(iObject, outArray,true);
	//(*ForcedObjects.Find(iObject))->CreateSequenceFromNode(iObject.GetObject(), OwnerSeq);
	//return;
}
