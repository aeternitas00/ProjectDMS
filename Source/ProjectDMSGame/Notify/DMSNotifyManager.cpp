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

		auto temp = std::move(OnResponseCompleted[CurrentSequence]);
		OnResponseCompleted.Remove(CurrentSequence);
		temp.ExecuteIfBound();
		temp.Unbind();
		return;
	}
	// PC Getter?

	DMS_LOG_SIMPLE(TEXT("==== %s [%d] : %d Respondent(s) ===="), *CurrentSequence->GetName(), CurrentSequence->Progress, ResponsedObjects.Num());

	APlayerController* LocalPC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UDMSNotifyRespondentSelector* InstancedWidget = CreateWidget<UDMSNotifyRespondentSelector>(LocalPC, ResponsedObjectSelector);

	InstancedWidget->ResponsedObjects = ResponsedObjects;
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
		
		NewSeq->AddToOnSequenceFinished_Native([NotifyManager, ResumingSequence= InstancedWidget->OwnerSeq,NewResponsedObjects](){
			// Replay response 

			DMS_LOG_SIMPLE(TEXT("==== %s : RESUME RESPONSE ===="), *ResumingSequence->GetName());

			auto LocalNRO = std::move(NewResponsedObjects);
			NotifyManager->CreateRespondentSelector(ResumingSequence, LocalNRO);
		});		

		UDMSCoreFunctionLibrary::GetDMSSequenceManager()->RunSequence(NewSeq);

		//InstancedWidget->OwnerSeq->OnSequenceFinish();
		InstancedWidget->CloseSelector();
		//...
		}, [&,InstancedWidget]() {
			// [ X Bttn ]

			// Player choose to not run EI
			OnResponseCompleted[CurrentSequence].ExecuteIfBound();
			OnResponseCompleted[CurrentSequence].Unbind();
			OnResponseCompleted.Remove(CurrentSequence);
			//InstancedWidget->OwnerSeq->OnSequenceFinish();
			InstancedWidget->CloseSelector();

		}, CurrentSequence);

	ResponsedObjects.GetKeys(InstancedWidget->Respondents);
	//InstancedWidget->OutTag=TAG_DMS_System_NotifyRespondent;
	InstancedWidget->PopupSelector();
	if (!InstancedWidget->SetupWidget()) InstancedWidget->CloseSelector();
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
