// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sequence/DMSSequenceStep.h"
#include "Effect/DMSEffectDefinition.h"
#include "DMSSequenceStep_Apply.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Step_Apply)

class UDMSActiveEffect;

UCLASS()
class DMSCORE_API UDMSSequenceStepDefinition_Apply : public UDMSSequenceStepDefinition
{
	GENERATED_BODY()

//protected:
//
//	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = Effect)
//	TArray<FDMSConditionedApplyDefinitions> ConditionedEffectDefinitions;

public:
	UDMSSequenceStepDefinition_Apply();

	UFUNCTION()
	void Progress_Before(UDMSSequenceStep* InstancedStep);

	UFUNCTION()
	void Progress_During(UDMSSequenceStep* InstancedStep);

	UFUNCTION()
	void Progress_After(UDMSSequenceStep* InstancedStep);

	// Implementations
	virtual FGameplayTag GetPureStepTag_Implementation() const;
	virtual FGameplayTagContainer GetStepTag_Implementation(UDMSSequenceStep* InstancedStep) const;

	virtual bool GetProgressOps_Implementation(const FGameplayTag& ProgressTag,TArray<FProgressExecutor>& OutExecutor);
};

