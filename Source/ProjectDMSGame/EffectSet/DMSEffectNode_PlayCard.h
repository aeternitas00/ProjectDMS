// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Card/DMSCardDefinition.h"
#include "DMSEffectNode_PlayCard.generated.h"

/**
 * BASIC ACTION _ Play Card from Hand ÀÇ Á¤ÀÇ.
 */
UCLASS(NotBlueprintable, HideDropdown/*, meta = (DisplayName = "Play Card Node Base")*/)
class PROJECTDMSGAME_API UDMSEffectNode_PlayCard : public UDMSEffectNode
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	bool bIgnoreCost;

	UDMSEffectNode_PlayCard();
};
