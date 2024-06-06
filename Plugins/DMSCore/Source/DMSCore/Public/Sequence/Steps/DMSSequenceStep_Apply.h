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

protected:	
	/**
	* Actual effects that activatable in that timing
	* Works in order to 0~n
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect, Instanced)
	TArray<TObjectPtr<UDMSEffectDefinition>> EffectDefinitions;

public:
	UDMSSequenceStepDefinition_Apply();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	EDMSBroadCastFlag BroadcastFlag_Before;

	UFUNCTION()
	void Progress_Before(UDMSSequenceStep* InstancedStep);

	UFUNCTION()
	void Progress_During(UDMSSequenceStep* InstancedStep);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	EDMSBroadCastFlag BroadcastFlag_After;

	UFUNCTION()
	void Progress_After(UDMSSequenceStep* InstancedStep);

	// Implementations
	virtual TArray<FName> GetDefaultProgressOrder_Implementation() const {return {"Progress_Before,Progress_During,Progress_After"};}
	virtual FGameplayTag GetPureStepTag_Implementation() const;
	virtual FGameplayTagContainer GetStepTag_Implementation(const UDMSSequenceStep* InstancedStep) const;

	virtual bool GetProgressOps_Implementation(const FGameplayTag& ProgressTag,TArray<FProgressExecutor>& OutExecutor);
};

