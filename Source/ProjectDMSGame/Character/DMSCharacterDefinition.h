// Copyright JeongWoo Lee

#pragma once

#include "ProjectDMS.h"
#include "Common/DMSSpawnableDataBase.h"
#include "Attribute/DMSAttribute.h"
#include "DMSCharacterDefinition.generated.h"

class UDMSEffectSet;

/**
 * 
 */
UCLASS(BlueprintType,Const)
class PROJECTDMSGAME_API UDMSCharacterDefinition : public UDMSSpawnableDataBase
{
	GENERATED_BODY()

public:
	/**
	 * Display name of card.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character)
	FText CharacterName;

	// [TO ATTRIBUTE]
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FGameplayTagContainer DefaultCharacterTraits;

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
	TMap<FGameplayTag, TObjectPtr<UDMSEffectSet>> CharacterEffectSets;

	/**
	 * Default attributes of character.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character)
	TArray<FDMSAttributeDefinition>	DefaultAttributes;

	// Sprites for character...
	// FBrush arr?

	// what do we need ?

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// TODO :: Asset Management Helper Funcs
};
