// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_GAME_MODULE
 *
 *	DMS Game Specific Card data form.
 *
 * =========================================
 */

#include "ProjectDMS.h"
#include "Common/DMSSpawnableDataBase.h"
#include "Common/DMSCommons.h"
#include "Attribute/DMSAttribute.h"
#include "Effect/DMSEffectDefinition.h"
#include "DMSCardDefinition.generated.h"

class UDMSAttribute;

/**
 *
 * 	========================================
 *
 *	Card Definition : Non mutatable Card's original data / definition.
 *
 * =========================================
 *
 */
UCLASS(Blueprintable, BlueprintType, Const, ClassGroup = (Card))
class PROJECTDMSGAME_API UDMSCardDefinition : public UDMSSpawnableDataBase
{
	GENERATED_BODY()
	
public:

	// ----------------------------------------------------------------------------------------- //
	// 기본적인 카드 구성 요소들은 인게임내에서 변경 가능성에 대해 생각해봐야함.
	
	/**
	 * Display name of card.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText CardName;

	/**
	 * Container of traits. [TO ATTRIBUTE]
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FGameplayTagContainer DefaultCardTraits;

	/**
	 * Description text of card effect.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText EffectText;

	/**
	 * Flavor text of card effect.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText FlavorText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CardEffect)
	TArray<FDMSAttributeDefinition> DefaultAttributes;

	/**
	 * Effect sets of Card.
	 * ex) <"Cost",CostEffectSet>
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = CardEffect)
	TMap<FGameplayTag, TObjectPtr<UDMSEffectSet>> CardEffectSets;

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// TODO :: Asset Management Helper Funcs
};
