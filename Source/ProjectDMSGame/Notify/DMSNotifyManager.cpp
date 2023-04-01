// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/DMSNotifyManager.h"
#include "Effect/DMSEffectorInterface.h"
#include "Effect/DMSEffectDefinition.h"
#include "Effect/DMSEffectInstance.h"
#include "Sequence/DMSSeqManager.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Sequence/DMSSequence.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_System_Notify_Respondent, "System.Notify.Respondent");
UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_System_Notify_ActivatingEffect, "System.Notify.ActivatingEffect");

template<typename FuncCompleted>
void UDMSNotifyManager::BroadCast(UDMSSequence* NotifyData, FuncCompleted&& ResponseCompleted)
{
	DMS_LOG_SIMPLE(TEXT("==== %s [%s] : BROADCASTING  ===="), *NotifyData->GetName(), *UDMSCoreFunctionLibrary::GetTimingString(NotifyData->Progress));
	TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*> ResponsedObjects;
	// Check non relative first

	// Check all relatives

	for (auto& Object : NotifyObjects)
	{
		Object->OnNotifyReceived(ResponsedObjects, NotifyData->OriginalEffectNode->bIsChainableEffect,NotifyData);
	}

	OnResponseCompleted.Add(NotifyData);	ForcedEIMap.Add(NotifyData);
	OnResponseCompleted[NotifyData].BindLambda(ResponseCompleted);

	// DO FORCED FIRST
	TArray<TScriptInterface<IDMSEffectorInterface>> Keys;
	ResponsedObjects.GetKeys(Keys);
	for (auto& ResponsedObject : Keys)	{
		TArray<UDMSEffectInstance*> EffectInstances;
		ResponsedObjects.MultiFind(ResponsedObject, EffectInstances, true);

		for (auto EI : EffectInstances) {
			if (EI->EffectNode->bForced) {
				TPair<UObject*, UDMSEffectInstance*> NewValue;
				NewValue.Key= ResponsedObject.GetObject(); NewValue.Value= EI;
				ForcedEIMap[NotifyData].ForcedObjects.Add(std::move(NewValue));
				ResponsedObjects.Remove(ResponsedObject, EI);
			}
		}
	}


	if (ForcedEIMap[NotifyData].ForcedObjects.Num()==0)
	{
		DMS_LOG_SIMPLE(TEXT("==== %s [%s] : NO FORCED EFFECT  ===="), *NotifyData->GetName(), *UDMSCoreFunctionLibrary::GetTimingString(NotifyData->Progress));

		ForcedEIMap.Remove(NotifyData);
		CreateRespondentSelector(NotifyData, ResponsedObjects);
	}
	else {	
		DMS_LOG_SIMPLE(TEXT("==== %s [%s] : ACTIVATING FORCED EFFECT START  ===="), *NotifyData->GetName(), *UDMSCoreFunctionLibrary::GetTimingString(NotifyData->Progress));

		ForcedEIMap[NotifyData].NonForcedObjects = ResponsedObjects;

		ForcedEIMap[NotifyData].Delegate.BindLambda([this](UDMSSequence* Sequence) {
			DMS_LOG_SIMPLE(TEXT("==== %s [%s] : FORCED EFFECT FINISHED ===="), *Sequence->GetName(), *UDMSCoreFunctionLibrary::GetTimingString(Sequence->Progress));
			auto temp = std::move(ForcedEIMap[Sequence]);
			ForcedEIMap.Remove(Sequence);
			CreateRespondentSelector(Sequence, temp.NonForcedObjects);
		});
		ForcedEIMap[NotifyData].IteratingDelegate.BindLambda([this](UDMSSequence* Sequence) {
				
			if (ForcedEIMap[Sequence].Count == ForcedEIMap[Sequence].ForcedObjects.Num()){
				DMS_LOG_SIMPLE(TEXT("==== %s [%s] : NO MORE FORCED EFFECT  ===="), *Sequence->GetName(), *UDMSCoreFunctionLibrary::GetTimingString(Sequence->Progress));
				ForcedEIMap[Sequence].Delegate.ExecuteIfBound(Sequence);
			}
			else{
				DMS_LOG_SIMPLE(TEXT("==== %s [%s] : ACTIVATE NEXT FORCED EFFECT  ===="), *Sequence->GetName(), *UDMSCoreFunctionLibrary::GetTimingString(Sequence->Progress));

				auto NewSeq = ForcedEIMap[Sequence].ForcedObjects[ForcedEIMap[Sequence].Count].Value->CreateSequenceFromNode(ForcedEIMap[Sequence].ForcedObjects[ForcedEIMap[Sequence].Count].Key, Sequence);
				NewSeq->AddToOnSequenceFinished_Native([this,Sequence](){ForcedEIMap[Sequence].IteratingDelegate.ExecuteIfBound(Sequence);});
				ForcedEIMap[Sequence].Count++;
				UDMSCoreFunctionLibrary::GetDMSSequenceManager()->RunSequence(NewSeq);
			}
		});

		ForcedEIMap[NotifyData].IteratingDelegate.ExecuteIfBound(NotifyData);
	}
}

void UDMSNotifyManager::RegisterNotifyObject(TScriptInterface<IDMSEffectorInterface> Object)
{
	NotifyObjects.Add(Object);
}

void UDMSNotifyManager::CreateRespondentSelector(UDMSSequence* CurrentSequence, TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*>& ResponsedObjects)
{
	FString TimingStr = UDMSCoreFunctionLibrary::GetTimingString(CurrentSequence->Progress);
	DMS_LOG_SIMPLE(TEXT("==== %s [%s] : Create Respondent Selector  ===="), *CurrentSequence->GetName(), *TimingStr);

	if (ResponsedObjects.Num() == 0) {

		DMS_LOG_SIMPLE(TEXT("==== %s [%s] : NO Respondent / EXE OnResponseCompleted ===="), *CurrentSequence->GetName(), *TimingStr);
		CallResponseCompleted(CurrentSequence);
		return;
	}
	// PC Getter?

	DMS_LOG_SIMPLE(TEXT("==== %s [%s] : %d Respondent(s) ===="), *CurrentSequence->GetName(), *TimingStr, ResponsedObjects.Num());

	APlayerController* LocalPC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UDMSNotifyRespondentSelector* InstancedWidget = CreateWidget<UDMSNotifyRespondentSelector>(LocalPC, ResponsedObjectSelector);

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
		
		InstancedWidget->ResponsedObjects.Remove(Respondent);

		TArray<TScriptInterface<IDMSEffectorInterface>> NewRespondents;
		TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*> NewResponsedObjects;

		InstancedWidget->ResponsedObjects.GetKeys(NewRespondents);
		for (auto Object : NewRespondents)
		{
			// 이거 말고 그냥 체크만 하는 함수로 재구성 하던가 포스드 일 때 강제실행을 여기서 노티파이 매니저가 하도록 해서 순차적으로 실행하게 해야함 ( 기다려주면서 ) 해야함.
			Object->OnNotifyReceived(NewResponsedObjects, InstancedWidget->OwnerSeq->OriginalEffectNode->bIsChainableEffect, InstancedWidget->OwnerSeq);
		}

		UDMSSequence* NewSeq = EffectInstance->CreateSequenceFromNode(Respondent.GetObject(), InstancedWidget->OwnerSeq);
		
		NewSeq->AddToOnSequenceFinished_Native([ ResumingSequence= InstancedWidget->OwnerSeq,NewResponsedObjects](){
			// Replay response 

			DMS_LOG_SIMPLE(TEXT("==== %s : RESUME RESPONSE ===="), *ResumingSequence->GetName());

			auto LocalNRO = std::move(NewResponsedObjects);
			UDMSCoreFunctionLibrary::GetDMSNotifyManager()->CreateRespondentSelector(ResumingSequence, LocalNRO);
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
