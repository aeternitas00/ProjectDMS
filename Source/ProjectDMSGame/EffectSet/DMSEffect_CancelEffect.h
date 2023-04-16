// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Effect/DMSEffectDefinition.h"
#include "DMSEffect_CancelEffect.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Effect_CancelEffect)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Effect_IgnoreEffect)

/// <summary>
/// This effect doesn't take targets to activate
/// only works to first met sequence.
/// </summary>
UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew, ClassGroup = (Effect), meta = (DisplayName = "Cancel Parent Effect"))
class PROJECTDMSGAME_API UDMSEffect_CancelEffect : public UDMSEffectDefinition
{
	GENERATED_BODY()
	
public:
	//UDMSEffect_CancelEffect();

	/**
	 * Cancel param sequence if sequence's effect node matches TargetTagQuery
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	FGameplayTagQuery CancelTargetTagQuery;

	virtual void Work_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnWorkCompleted& OnWorkCompleted) override;
};

UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew, ClassGroup = (Effect), meta = (DisplayName = "Ignore Parent Effect"))
class PROJECTDMSGAME_API UDMSEffect_IgnoreEffect : public UDMSEffectDefinition
{
	GENERATED_BODY()

public:
	//UDMSEffect_CancelEffect();

	/**
	 * Ignore param sequence if sequence's effect node matches TargetTagQuery.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	FGameplayTagQuery IgnoreTargetTagQuery;

	/**
	 * Ignore sequence's applying part only if false.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	bool IgnoreWholeSequence;

	virtual void Work_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnWorkCompleted& OnWorkCompleted) override;
};
