// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Effect/DMSEffectDefinition.h"
#include "DMSEffect_MoveLocatable.generated.h"

class ADMSCardBase;
class UDMSCardContainerComponent;

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Effect_MoveLocatable)

// Simplified move effect for dms spawnable which implements Locatable interface.
// Get destination only from attribute tagged "Effect.MoveLocatable" of sequence.
UCLASS(Blueprintable , ClassGroup = (Effect), meta = (DisplayName = "ED Move Locatable Base"))
class PROJECTDMSGAME_API UDMSEffect_MoveLocatable : public UDMSEffectDefinition
{
	GENERATED_BODY()
	
public:
	UDMSEffect_MoveLocatable();

	virtual void Work_Implementation(ADMSSequence* SourceSequence, ADMSActiveEffect* iEI, const FOnExecuteCompleted& OnWorkCompleted) override;

	//virtual AActor* GetPlayerFocusTarget_Implementation(ADMSSequence* SourceSequence, ADMSActiveEffect* iEI) override;
};
