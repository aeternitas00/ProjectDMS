// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Effect/DMSEffectDefinition.h"
#include "DMSEffect_CancelEffect.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Effect_CancelEffect)

/// <summary>
/// This effect doesn't take targets to activate
/// only works to Source Sequence's direct parent sequence.
/// </summary>
UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew, ClassGroup = (Effect), meta = (DisplayName = "Cancel Effect"))
class PROJECTDMSGAME_API UDMSEffect_CancelEffect : public UDMSEffectDefinition
{
	GENERATED_BODY()
	
public:
	//UDMSEffect_CancelEffect();

	virtual void Work_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnWorkCompleted& OnWorkCompleted) override;
};
