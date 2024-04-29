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
#include "DMSChaosTokenDefinition.generated.h"


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
UCLASS(Blueprintable, BlueprintType, Const, ClassGroup = (Misc))
class PROJECTDMSGAME_API UDMSChaosTokenDefinition : public UDMSSpawnableDataBase
{
	GENERATED_BODY()
	
public:

	// ----------------------------------------------------------------------------------------- //
	// 기본적인 카드 구성 요소들은 인게임내에서 변경 가능성에 대해 생각해봐야함.
	
	/**
	 * Display name of card.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	TArray<FDMSAttributeDefinition> DefaultAttributes;

	/**
	 * Effect sets of Card.
	 * ex) <"Cost",CostEffectSet>
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = Effect)
	TObjectPtr<UDMSEffectSet> EffectSet;

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// TODO :: Asset Management Helper Funcs
};
