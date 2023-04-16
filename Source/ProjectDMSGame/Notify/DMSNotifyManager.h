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
 *	�̰� ��ǻ� SeqMan�̶� �����ص� ����� �����ʳ�?
 */

#include "ProjectDMS.h"
#include "UObject/NoExportTypes.h"
//#include "Containers/Map.h"
#include "Selector/DMSDecisionWidget.h"
#include "DMSNotifyManager.generated.h"


class UDMSSequence;
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
 *	��Ƽ���� �Ŵ����� ��Ƽ���� ���ù��鿡�� [ "SourceObjectClass"�� "EffectKeyword"�� (���) "TargetObjectClass"���� "Timing"(�ϱ���/�ϴ���/����) ] 
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

	//�̰� �� �� ��ü��ȸ ���� �ϴ� ��� ������?
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

