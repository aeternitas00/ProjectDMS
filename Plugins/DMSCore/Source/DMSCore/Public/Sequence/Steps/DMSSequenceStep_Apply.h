// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sequence/DMSSequenceStep.h"
#include "Effect/DMSEffectDefinition.h"
#include "DMSSequenceStep_Apply.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Step_Apply)

class UDMSActiveEffect;
class UDMSEffectOption;

UCLASS()
class DMSCORE_API UDMSEffectResolveWorker : public UDMSSynchronousTaskWorker
{
	GENERATED_BODY()
private:
	TObjectPtr<ADMSSequence> SourceSequence;
	FOnTaskCompleted IteratingDelegate;
	TObjectPtr<UDMSSequenceStepDefinition_Apply> ApplyDefinition;
public:
	void SetupResolveWorker(ADMSSequence* iSequence, UDMSSequenceStepDefinition_Apply* iApplyDefinition);
	virtual void Work_Implementation();
	//virtual void OnAllTaskCompleted_Implementation(bool WorkerSucceeded);
};

/**
* 	========================================
*
*	서버사이드에서 Apply Step 로직 처리를 할 때 사용 할 단순 워커
* 
*	========================================
*	
*/

UCLASS()
class DMSCORE_API UDMSEffectApplyWorker : public UDMSSynchronousTaskWorker
{
	GENERATED_BODY()

private:
	TObjectPtr<ADMSActiveEffect> OwnerInstance;
	TObjectPtr<ADMSSequence> SourceSequence;
	TObjectPtr<UDMSEffectDefinition> CurrentDef;
	uint8 ExecutedOptionNum;
	FOnOptionCompleted EffectOptionCompleted;

public:
	FOnExecuteCompleted IteratingDelegate;

	void SetupApplyWorker(ADMSSequence* iSequence, ADMSActiveEffect* iEI);

	UFUNCTION()
	void OnEffectOptionCompleted(UDMSEffectOption* CompletedOption);

	virtual void Work_Implementation();
	virtual void OnAllTaskCompleted_Implementation(bool WorkerSucceeded);
};


UCLASS()
class DMSCORE_API UDMSSequenceStepDefinition_Apply : public UDMSSequenceStepDefinition
{
	GENERATED_BODY()

protected:	


public:
	UDMSSequenceStepDefinition_Apply();
	/**
	* Actual effects that activatable in that timing
	* Works in order to 0~n
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect, Instanced)
	TArray<TObjectPtr<UDMSEffectDefinition>> EffectDefinitions;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Effect)
	EDMSBroadCastFlag BroadcastFlag_Before;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Effect)
	EDMSBroadCastFlag BroadcastFlag_After;
	UFUNCTION()
	void Progress_Before(UDMSSequenceStep* InstancedStep);

	UFUNCTION()
	void Progress_During(UDMSSequenceStep* InstancedStep);

	UFUNCTION()
	void Progress_After(UDMSSequenceStep* InstancedStep);

	// Implementations
	virtual TArray<FDMSStepProgressMetaData> GetOrderedProgressData_Implementation() const;
	virtual FGameplayTag GetPureStepTag_Implementation() const;
	virtual FGameplayTagContainer GetStepTag_Implementation(const UDMSSequenceStep* InstancedStep) const;
};

