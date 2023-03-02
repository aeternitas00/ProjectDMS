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
#include "Common/DMSTimingCondition.h"
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
class PROJECTDMSGAME_API UDMSCardDefinition : public UDataAsset
{
	GENERATED_BODY()
	
public:

	// ----------------------------------------------------------------------------------------- //
	// 기본적인 카드 구성 요소들은 인게임내에서 변경 가능성에 대해 생각해봐야함.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText CardName;

	// Using Data registry Connect with type behavior?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FName DefaultCardType; // ENUM?

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	//TSubclassOf<UDMSEffectNode_TypeBehavior> DefaultCardType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText EffectText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText FlavorText;
	
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	//FBrush Image;
		
	// ----------------------------------------------------------------------------------------- //
	// DEPRECATED
	
	// UI 연동하는 부분과 연결 필요성 있음. ( 타겟등 플레이 전에 리버트 가능한 Status condition check 구현 )

	// Card's Effect.
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = CardEffect)
	//UDMSEffectSet* CardEffects;

	// Card's Play Cost -> visualizing info
	// 플레이 '전'에 선택해야 요소들에 대한 셀렉터들도 여기에
	// 사용 가능한 코스트의 종류가 여러개일 경우를 대비해 Node 대신 Set
	// 코스트 구현 방식에 대해 재고? (지불 가능 할지에 대한 예측이 가능해야 함 => 별도의 클래스화가 맞는가? Effect의 파생클래스 )
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = CardEffect)
	//UDMSEffectSet* CostAndPlayCondition;

	// ----------------------------------------------------------------------------------------- //

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = CardEffect)
	TMap<FName,UDMSEffectSet*> CardEffectSets;

public:

	// TODO :: Asset Management Helper Funcs
};
