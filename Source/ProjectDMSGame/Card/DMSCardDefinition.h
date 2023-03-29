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
class PROJECTDMSGAME_API UDMSCardDefinition : public UDataAsset
{
	GENERATED_BODY()
	
public:

	// ----------------------------------------------------------------------------------------- //
	// �⺻���� ī�� ���� ��ҵ��� �ΰ��ӳ����� ���� ���ɼ��� ���� �����غ�����.
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

	// enum for key?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = CardEffect)
	TMap<FName,UDMSEffectSet*> CardEffectSets;

public:

	// TODO :: Asset Management Helper Funcs
};
