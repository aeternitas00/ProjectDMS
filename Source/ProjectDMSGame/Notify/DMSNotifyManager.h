// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *	Notifying
 *
 *	========================================
 *	이거 사실상 SeqMan이랑 병합해도 상관이 없지않나?
 */

#include "ProjectDMS.h"
#include "UObject/NoExportTypes.h"
#include "Sequence/DMSSequence.h"
#include "Effect/DMSEffectInstance.h"
#include "Selector/DMSDecisionWidget.h"
#include "DMSNotifyManager.generated.h"

class IDMSEffectorInterface;

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_System_Notify_Respondent)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_System_Notify_ActivatingEffect)

UCLASS()
class PROJECTDMSGAME_API UDMSNotifyRespondentSelector : public UDMSDecisionWidget
{
	GENERATED_BODY()

public:
	//UPROPERTY()
	TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*> ResponsedObjects;

	UPROPERTY(BlueprintReadOnly)
	TArray<TScriptInterface<IDMSEffectorInterface>> Respondents;

	//UPROPERTY(BlueprintReadOnly)
	//FGameplayTag OutTag;

	//UDMSSequence* CurrentSequence;

	UFUNCTION(BlueprintCallable)
	UDMSDataObjectSet* MakeOutputDatas(UObject* Respondent, UObject* EffectInstance);

	UFUNCTION(BlueprintCallable)
	void GetEffectInstancesFromObject(TScriptInterface<IDMSEffectorInterface> iObject, TArray<UDMSEffectInstance*>& outArray);
};
/**
 *	========================================
 * 
 *	노티파이 매니저가 노티파이 리시버들에게 [ "SourceObjectClass"가 "EffectKeyword"를 (어떻게) "TargetObjectClass"에게 "Timing"(하기전/하는중/한후) ] 
 *
 *	========================================
 */
UCLASS()
class PROJECTDMSGAME_API UDMSNotifyManager : public UObject
{
	GENERATED_BODY()

protected:
	/**
	 * Store all of effectors in level.
	 * Used in Notify broadcast.
	 */
	UPROPERTY(BlueprintGetter = GetNotifyObjects)
	TArray<TScriptInterface<IDMSEffectorInterface>> NotifyObjects;

	/**
	 * Delegates for chained delegates of notify step.
	 */
	DECLARE_DELEGATE_OneParam(FOnForcedSequenceCompleted, UDMSSequence*);
	DECLARE_DELEGATE_OneParam(FForcedEIIteratingDelegate, UDMSSequence*);
	
	/**
	 * Structure for implementing chained delegates to ensure synchronization in the Notify step.
	 */
	struct FForcedEICounter {
		FOnForcedSequenceCompleted Delegate;
		uint8 Count = 0;
		TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*> NonForcedObjects;
		TArray<TPair<UObject*, UDMSEffectInstance*>> ForcedObjects;
		FForcedEIIteratingDelegate IteratingDelegate;
	};

	/**
	 * Store FForcedEICounter for each sequences.
	 */
	TMap<UDMSSequence*, FForcedEICounter> ForcedEIMap;

	/**
	 * 
	 */
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	TSubclassOf<UDMSNotifyRespondentSelector> ResponsedObjectSelector;
public:

	//이거 좀 더 전체순회 없이 하는 방법 있을까?
	/**
	 * Broadcast sequence for NotifyObjects.
	 * @param	NotifyData						Current sequence.
	 * @param	ResponseCompleted				
	 */
	template<typename FuncCompleted >
	void BroadCast(UDMSSequence* NotifyData, FuncCompleted&& ResponseCompleted);

	/**
	 * Broadcast sequence for NotifyObjects.
	 * @param	Object							Register target.
	 */
	UFUNCTION(BlueprintCallable)
	void RegisterNotifyObject(TScriptInterface<IDMSEffectorInterface> Object);

	/**
	 * Broadcast sequence for NotifyObjects.
	 * @param	CurrentSequence					Current sequence.
	 * @param	ResponsedObjects				Out Responsed object.
	 */
	void CreateRespondentSelector(UDMSSequence* CurrentSequence, TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*>& ResponsedObjects);

	/**
	 * Broadcast sequence for NotifyObjects.
	 * @param	CurrentSequence
	 */
	void CallResponseCompleted(UDMSSequence* CurrentSequence);

	/**
	 * Broadcast sequence for NotifyObjects.
	 * @return	Object
	 */
	UFUNCTION(BlueprintCallable,BlueprintPure)
	TArray<TScriptInterface<IDMSEffectorInterface>> GetNotifyObjects(){return NotifyObjects;}

	DECLARE_DELEGATE(FOnResponseCompleted);

	TMap<UDMSSequence*, FOnResponseCompleted> OnResponseCompleted; // MAKE IT SEQ RELATIVE
};

template<typename FuncCompleted>
void UDMSNotifyManager::BroadCast(UDMSSequence* NotifyData, FuncCompleted&& ResponseCompleted)
{
	DMS_LOG_SIMPLE(TEXT("==== %s [%s] : BROADCASTING  ===="), *NotifyData->GetName(), *UDMSCoreFunctionLibrary::GetTimingString(NotifyData->Progress));

	if (NotifyData->SequenceState == EDMSSequenceState::SS_Canceled) {
		DMS_LOG_SIMPLE(TEXT("Sequence is canceled"));
		ResponseCompleted();
		return;
	}

	TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*> ResponsedObjects;


	for (auto& Object : NotifyObjects)
	{
		Object->OnNotifyReceived(ResponsedObjects, NotifyData->OriginalEffectNode->bIsChainableEffect, NotifyData);
	}

	//OnResponseCompleted[NotifyData].BindUObject();

	OnResponseCompleted.Add(NotifyData);	ForcedEIMap.Add(NotifyData);
	OnResponseCompleted[NotifyData].BindLambda(ResponseCompleted);

	// DO FORCED FIRST
	TArray<TScriptInterface<IDMSEffectorInterface>> Keys;
	ResponsedObjects.GetKeys(Keys);
	for (auto& ResponsedObject : Keys) {
		TArray<UDMSEffectInstance*> EffectInstances;
		ResponsedObjects.MultiFind(ResponsedObject, EffectInstances, true);

		for (auto EI : EffectInstances) {
			if (EI->EffectNode->bForced) {
				TPair<UObject*, UDMSEffectInstance*> NewValue;
				NewValue.Key = ResponsedObject.GetObject(); NewValue.Value = EI;
				ForcedEIMap[NotifyData].ForcedObjects.Add(std::move(NewValue));
				ResponsedObjects.Remove(ResponsedObject, EI);
			}
		}
	}


	if (ForcedEIMap[NotifyData].ForcedObjects.Num() == 0)
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

			if (ForcedEIMap[Sequence].Count == ForcedEIMap[Sequence].ForcedObjects.Num()) {
				DMS_LOG_SIMPLE(TEXT("==== %s [%s] : NO MORE FORCED EFFECT  ===="), *Sequence->GetName(), *UDMSCoreFunctionLibrary::GetTimingString(Sequence->Progress));
				ForcedEIMap[Sequence].Delegate.ExecuteIfBound(Sequence);
			}
			else {
				DMS_LOG_SIMPLE(TEXT("==== %s [%s] : ACTIVATE NEXT FORCED EFFECT  ===="), *Sequence->GetName(), *UDMSCoreFunctionLibrary::GetTimingString(Sequence->Progress));

				auto NewSeq = ForcedEIMap[Sequence].ForcedObjects[ForcedEIMap[Sequence].Count].Value->CreateSequenceFromNode(ForcedEIMap[Sequence].ForcedObjects[ForcedEIMap[Sequence].Count].Key, Sequence);
				NewSeq->AddToOnSequenceFinished_Native([this, Sequence]() {ForcedEIMap[Sequence].IteratingDelegate.ExecuteIfBound(Sequence); });
				ForcedEIMap[Sequence].Count++;
				UDMSCoreFunctionLibrary::GetDMSSequenceManager()->RunSequence(NewSeq);
			}
			});

		ForcedEIMap[NotifyData].IteratingDelegate.ExecuteIfBound(NotifyData);
	}
}