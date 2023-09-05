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
 *	
 *	NOTE :: �̰� ��ǻ� SeqMan�̶� �����ص� ����� �����ʳ�?
 * 
 */

#include "DMSCoreIncludes.h"
#include "UObject/NoExportTypes.h"
#include "Sequence/DMSSequence.h"
#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEffectorInterface.h"
#include "Selector/DMSDecisionWidget.h"
#include "DMSNotifyManager.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_System_Notify_Respondent)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_System_Notify_ActivatingEffect)

UCLASS()
class DMSCORE_API UDMSNotifyRespondentSelector : public UDMSDecisionWidget
{
	GENERATED_BODY()

public:
	/**
	 * Objects which 
	 */
	//UPROPERTY()
	TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*> ResponsedObjects;

	/**
	 * Copy of ResponsedObjects's Key array.
	 */

	UPROPERTY(BlueprintReadOnly)
	TArray<TScriptInterface<IDMSEffectorInterface>> Respondents;

	/**
	 * Make new set of Notify respondent datas. 
	 * @param	Respondent				Respondent object.
	 * @param	EffectInstance			Responsed EffectInstance. (belong to Respondent)
	 * @return	Created UDMSDataObjectSet. ( Owner of UDMSDataObjectSet is widget's owning player state )
	 */
	UFUNCTION(BlueprintCallable)
	UDMSDataObjectSet* MakeOutputDatas(UObject* Respondent, UObject* EffectInstance);

	/**
	 * 
	 */
	UFUNCTION(BlueprintCallable)
	void GetEffectInstancesFromObject(TScriptInterface<IDMSEffectorInterface> iObject, TArray<UDMSEffectInstance*>& outArray);
};
/**
 *	========================================
 * 
 *	��Ƽ���� �Ŵ����� ��Ƽ���� ���ù��鿡�� [ "SourceObjectClass"�� "EffectKeyword"�� (���) "TargetObjectClass"���� "Timing"(�ϱ���/�ϴ���/����) ] 
 *
 *	========================================
 */
UCLASS(BlueprintType, Blueprintable)
class DMSCORE_API UDMSNotifyManager : public UObject
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
	 * Widget class to be used for selecting response objects in the game mode that uses this notify manager.
	 */
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	TSubclassOf<UDMSNotifyRespondentSelector> ResponsedObjectSelector;
public:

	//�̰� �� �� ��ü��ȸ ���� �ϴ� ��� ������?
	/**
	 * Broadcast sequence for NotifyObjects.
	 * @param	NotifyData						Current sequence.
	 * @param	ResponseCompleted				
	 */
	template<typename FuncCompleted >
	void Broadcast(UDMSSequence* NotifyData, FuncCompleted&& ResponseCompleted);

	/**
	 * Broadcast sequence for NotifyObjects.
	 * @param	Object							Register target.
	 * @return	true if registring was success.
	 */
	UFUNCTION(BlueprintCallable)
	bool RegisterNotifyObject(TScriptInterface<IDMSEffectorInterface> Object);

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
	TArray<TScriptInterface<IDMSEffectorInterface>> GetNotifyObjects(){ return NotifyObjects; }

	DECLARE_DELEGATE(FOnResponseCompleted);

	TMap<UDMSSequence*, FOnResponseCompleted> OnResponseCompleted; // MAKE IT SEQ RELATIVE
};

template<typename FuncCompleted>
void UDMSNotifyManager::Broadcast(UDMSSequence* NotifyData, FuncCompleted&& ResponseCompleted)
{
	DMS_LOG_SIMPLE(TEXT("==== %s [%s] : BROADCASTING  ===="), *NotifyData->GetName(), *UDMSCoreFunctionLibrary::GetTimingString(NotifyData->GetCurrentProgress()));

	if (NotifyData->SequenceState == EDMSSequenceState::SS_Canceled) {
		DMS_LOG_SIMPLE(TEXT("Sequence is canceled"));
		ResponseCompleted();
		return;
	}

	TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*> ResponsedObjects;

	// Collect responsed objects 
	for (auto& Object : NotifyObjects)
	{
		//DMS_LOG_SIMPLE(TEXT("%s"), (Object==nullptr) ? TEXT("Object is NULLPTR") : *(Object->GetObject()->GetName()));
		Object->OnNotifyReceived(ResponsedObjects, NotifyData->IsChainableSequence(), NotifyData);
	}

	OnResponseCompleted.Add(NotifyData);	ForcedEIMap.Add(NotifyData);
	OnResponseCompleted[NotifyData].BindLambda(ResponseCompleted);

	// Search forced effects
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

	// Process forced effect first, followed by processing the selectable.
	if (ForcedEIMap[NotifyData].ForcedObjects.Num() != 0)
	{
		// Resolve forced effect
		DMS_LOG_SIMPLE(TEXT("==== %s [%s] : ACTIVATING FORCED EFFECT START  ===="), *NotifyData->GetName(), *UDMSCoreFunctionLibrary::GetTimingString(NotifyData->GetCurrentProgress()));

		ForcedEIMap[NotifyData].NonForcedObjects = ResponsedObjects;

		ForcedEIMap[NotifyData].Delegate.BindLambda([this](UDMSSequence* Sequence) {
			DMS_LOG_SIMPLE(TEXT("==== %s [%s] : FORCED EFFECT FINISHED ===="), *Sequence->GetName(), *UDMSCoreFunctionLibrary::GetTimingString(Sequence->GetCurrentProgress()));
			auto temp = std::move(ForcedEIMap[Sequence]);
			ForcedEIMap.Remove(Sequence);
			CreateRespondentSelector(Sequence, temp.NonForcedObjects);
		});

		ForcedEIMap[NotifyData].IteratingDelegate.BindLambda([this](UDMSSequence* Sequence) {

			if (ForcedEIMap[Sequence].Count == ForcedEIMap[Sequence].ForcedObjects.Num()) {
				DMS_LOG_SIMPLE(TEXT("==== %s [%s] : NO MORE FORCED EFFECT  ===="), *Sequence->GetName(), *UDMSCoreFunctionLibrary::GetTimingString(Sequence->GetCurrentProgress()));
				ForcedEIMap[Sequence].Delegate.ExecuteIfBound(Sequence);
			}
			else {
				DMS_LOG_SIMPLE(TEXT("==== %s [%s] : ACTIVATE NEXT FORCED EFFECT  ===="), *Sequence->GetName(), *UDMSCoreFunctionLibrary::GetTimingString(Sequence->GetCurrentProgress()));

				auto NewSeq = ForcedEIMap[Sequence].ForcedObjects[ForcedEIMap[Sequence].Count].Value->CreateSequenceFromNode(ForcedEIMap[Sequence].ForcedObjects[ForcedEIMap[Sequence].Count].Key, Sequence);
				NewSeq->AddToOnSequenceFinished_Native([this, Sequence](bool) {ForcedEIMap[Sequence].IteratingDelegate.ExecuteIfBound(Sequence); });
				ForcedEIMap[Sequence].Count++;
				UDMSCoreFunctionLibrary::GetDMSSequenceManager()->RunSequence(NewSeq);
			}
		});

		ForcedEIMap[NotifyData].IteratingDelegate.ExecuteIfBound(NotifyData);
	}
	else 
	{
		// No remaining forced effect
		DMS_LOG_SIMPLE(TEXT("==== %s [%s] : NO FORCED EFFECT  ===="), *NotifyData->GetName(), *UDMSCoreFunctionLibrary::GetTimingString(NotifyData->GetCurrentProgress()));

		ForcedEIMap.Remove(NotifyData);

		// Selectable effects.
		CreateRespondentSelector(NotifyData, ResponsedObjects);
	}
}