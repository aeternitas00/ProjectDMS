// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DMSCoreIncludes.h"
#include "Effect/DMSEffectDefinition.h"
#include "DMSEffect_ProgressControl.generated.h"

/// <summary>
/// This effect doesn't take targets to activate
/// only works to first met sequence.
/// </summary>
UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew, ClassGroup = (Effect), meta = (DisplayName = "Control progress flow of parent sequence"))
class DMSCORE_API UDMSEffect_ProgressControl : public UDMSEffectDefinition
{
	GENERATED_BODY()
	
public:
	//UDMSEffect_ProgressControl();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	bool bTargetByIndex;

	/**
	 * Jump to tagged step progress which matches TargetTagQuery
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect, meta = (EditCondition = "!bTargetByIndex", EditConditionHides))
	FGameplayTag JumpTargetTag;

	/**
	 * Jump to tagged step progress which matches TargetTagQuery
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect, meta = (EditCondition = "bTargetByIndex", EditConditionHides))
	int JumpTargetIdx;

	virtual void Work_Implementation(ADMSSequence* SourceSequence, ADMSActiveEffect* iEI, const FOnExecuteCompleted& OnWorkCompleted) override;
};
