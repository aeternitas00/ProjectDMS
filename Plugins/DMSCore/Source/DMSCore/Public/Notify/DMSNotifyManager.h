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
 *	NOTE :: 이거 사실상 SeqMan이랑 병합해도 상관이 없지않나?
 * 
 */

#include "DMSCoreIncludes.h"
#include "Components/ActorComponent.h"
#include "Common/DMSSynchronousTaskWorker.h"
#include "Sequence/DMSSequence.h"
#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEffectorInterface.h"
#include "Selector/DMSConfirmWidgetBase.h"
#include "DMSNotifyManager.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_System_Notify_Respondent)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_System_Notify_ActivatingEffect)

UCLASS()
class DMSCORE_API UDMSForcedEffectContext : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TScriptInterface<IDMSEffectorInterface> Respondent;
	UPROPERTY()
	TObjectPtr<ADMSActiveEffect> ActiveEffect;
};
	
UCLASS()
class DMSCORE_API UDMSForcedEffectWorker : public UDMSSynchronousTaskWorker
{
	GENERATED_BODY()

private:
	TObjectPtr<ADMSSequence> SourceSequence;

public:
	FOnExecuteCompleted IteratingDelegate;

	void SetupForcedWorker(ADMSSequence* iSequence);

	virtual void Work_Implementation();
	//virtual void OnAllTaskCompleted_Implementation(bool WorkerSucceeded);
};



// Not using selector form
UCLASS()
class DMSCORE_API UDMSNotifyRespondentSelector : public UDMSSelectorBase
{
	GENERATED_BODY()

public:
	//UPROPERTY(BlueprintReadOnly)
	ADMSSequence* CurrentSequence;

	/**
	 * Objects which responsed to sequence's current progress
	 */
	//UPROPERTY()
	TMultiMap<TScriptInterface<IDMSEffectorInterface>, ADMSActiveEffect*> ResponsedObjects;

	/**
	 * Copy of ResponsedObjects's Key array.
	 */
	UPROPERTY(BlueprintReadOnly)
	TArray<TScriptInterface<IDMSEffectorInterface>> Respondents;

	/**
	 * Helper function that replaces the Respondent in the data of the effect instance with the provided parameter object.
	 * @param	Respondent			Object to be updated as a respondent
	 * @param	EffectInstance		Update target.
	 */
	UFUNCTION(BlueprintCallable)
	void UpdateData(UObject* Respondent, UObject* EffectInstance);

	/**
	 * 
	 */
	UFUNCTION(BlueprintCallable)
	void GetEffectInstancesFromObject(TScriptInterface<IDMSEffectorInterface> iObject, TArray<ADMSActiveEffect*>& outArray);

	void InitRespondentSelector(ADMSSequence* iCurrentSequence, const TMultiMap<TScriptInterface<IDMSEffectorInterface>, ADMSActiveEffect*>& iResponsedObjects);
};
/**
 *	========================================
 * 
 *	노티파이 매니저가 노티파이 리시버들에게 [ "SourceObjectClass"가 "EffectKeyword"를 (어떻게) "TargetObjectClass"에게 "Timing"(하기전/하는중/한후) ] 
 *
 *	========================================
 */
UCLASS(BlueprintType, Blueprintable)
class DMSCORE_API UDMSNotifyManager : public UActorComponent
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
	DECLARE_DELEGATE_OneParam(FOnForcedSequenceCompleted, ADMSSequence*);
	DECLARE_DELEGATE_OneParam(FForcedEIIteratingDelegate, ADMSSequence*);
	
	/**
	 * Widget class to be used for selecting response objects in the game mode that uses this notify manager.
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
	void Broadcast(ADMSSequence* NotifyData, const FOnTaskCompletedNative& ResponseCompleted);

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
	void CreateRespondentSelector(ADMSSequence* CurrentSequence, TMultiMap<TScriptInterface<IDMSEffectorInterface>, ADMSActiveEffect*>& ResponsedObjects, const FOnTaskCompletedNative& ResponseCompleted);

	/**
	 * Broadcast sequence for NotifyObjects.
	 * @return	Object
	 */
	UFUNCTION(BlueprintCallable,BlueprintPure)
	TArray<TScriptInterface<IDMSEffectorInterface>> GetNotifyObjects(){ return NotifyObjects; }
};

