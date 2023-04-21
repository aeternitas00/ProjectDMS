// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Engine/DataAsset.h"
#include "Attribute/DMSSerializedAttribute.h"
#include "DMSCharacterDefinition.generated.h"

class UDMSEffectSet;

/**
 * 
 */
UCLASS(BlueprintType,Const)
class PROJECTDMSGAME_API UDMSCharacterDefinition : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:
	/**
	 * Display name of card.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character)
	FText CharacterName;

	/**
	 * Flavor text of character.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character)
	FText FlavorText;

	/**
	 * Effect sets of character.
	 * ex) <"Cost",CostEffectSet>
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = Character)
	TMap<FName, TObjectPtr<UDMSEffectSet>> CharacterEffectSets;

	/**
	 * Default attributes of character.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character)
	TArray<FDMSSerializedAttribute>	DefaultAttributes;

	// Sprites for character...
	// FBrush arr?

	// what do we need ?

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// TODO :: Asset Management Helper Funcs
};
