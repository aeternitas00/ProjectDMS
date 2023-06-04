// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Effect/DMSEffectDefinition.h"
#include "DMSEffect_MoveLocatable.generated.h"

class ADMSCardBase;
class UDMSCardContainerComponent;

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Effect_MoveLocatable)

UCLASS(Blueprintable , ClassGroup = (Effect), meta = (DisplayName = "Move Locatable Effect Base"))
class PROJECTDMSGAME_API UDMSEffect_MoveLocatable : public UDMSEffectDefinition
{
	GENERATED_BODY()
	
public:
	UDMSEffect_MoveLocatable();

	virtual void Work_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnWorkCompleted& OnWorkCompleted) override;

	virtual AActor* GetPlayerFocusTarget_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI) override;
};
