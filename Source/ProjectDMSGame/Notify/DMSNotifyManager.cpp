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
	DMS_LOG_SIMPLE(TEXT("==== %s [%d] : BROADCASTING  ===="), *NotifyData->GetName(), NotifyData->Progress);

	// Check non relative first

	// Check all relatives
	TMultiMap<TScriptInterface<IDMSEffectorInterface>,UDMSEffectInstance*> ResponsedObjects;
	for (auto Object : NotifyObjects)
	{
		Object->OnNotifyReceived(ResponsedObjects, NotifyData->OriginalEffectNode->bIsChainableEffect,NotifyData);
	}
	OnResponseCompleted.Add(NotifyData);
	OnResponseCompleted[NotifyData].BindLambda(ResponseCompleted);
	CreateRespondentSelector(NotifyData, ResponsedObjects);
}

void UDMSNotifyManager::RegisterNotifyObject(TScriptInterface<IDMSEffectorInterface> Object)
{
	NotifyObjects.Add(Object);
}

void UDMSNotifyManager::CreateRespondentSelector(UDMSSequence* CurrentSequence, TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*>& ResponsedObjects)
{
	DMS_LOG_SIMPLE(TEXT("==== %s [%d] : Create Respondent Selector  ===="), *CurrentSequence->GetName(), CurrentSequence->Progress);

	if (ResponsedObjects.Num() == 0) {

		DMS_LOG_SIMPLE(TEXT("==== %s [%d] : NO Respondent / EXE OnResponseCompleted ===="), *CurrentSequence->GetName(), CurrentSequence->Progress);
		CallResponseCompleted(CurrentSequence);
		return;
	}
	// PC Getter?

	DMS_LOG_SIMPLE(TEXT("==== %s [%d] : %d Respondent(s) ===="), *CurrentSequence->GetName(), CurrentSequence->Progress, ResponsedObjects.Num());

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
	//(*ResponsedObjects.Find(iObject))->CreateSequenceFromNode(iObject.GetObject(), OwnerSeq);
	//return;
}
