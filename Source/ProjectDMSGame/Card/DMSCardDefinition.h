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
#include "Engine/DataAsset.h"
#include "Common/DMSCommons.h"
#include "Effect/DMSEffectDefinition.h"
#include "DMSCardDefinition.generated.h"



class UDMSEffect_Cost;
class UDMSEffectNode_TypeBehavior;

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
class PROJECTDMSGAME_API UDMSCardDefinition : public UPrimaryDataAsset
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
	 * Name of card type.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FName DefaultCardType; // ENUM?

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

	/**
	 * Displaying mana cost of card. (not actual cost. cosmetic)
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText DisplayManaCost;
	
	/**
	 * Displaying action point cost of card. (not actual cost. cosmetic)
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText DisplayAPCost;

	/**
	 * Effect sets of Card.
	 * ex) <"Cost",CostEffectSet>
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = CardEffect)
	TMap<FName, TObjectPtr<UDMSEffectSet>> CardEffectSets;

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// TODO :: Asset Management Helper Funcs
};
