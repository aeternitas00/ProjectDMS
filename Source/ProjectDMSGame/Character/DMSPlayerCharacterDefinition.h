// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_GAME_MODULE
 *
 *	DMS Game specific character data form.
 *
 * =========================================
 */

#include "ProjectDMS.h"
#include "Character/DMSCharacterDefinition.h"
#include "DMSPlayerCharacterDefinition.generated.h"

/**
 *
 * 	========================================
 * 
 *	Character Definition : Non mutatable character's original data / definition.
 *
 * =========================================
 *
 */
UCLASS()
class PROJECTDMSGAME_API UDMSPlayerCharacterDefinition : public UDMSCharacterDefinition
{
	GENERATED_BODY()

public:
	//Deck restriction.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character)
	TArray<FGameplayTagQuery> DefaultDeckRestriction;


	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// TODO :: Asset Management Helper Funcs
};
