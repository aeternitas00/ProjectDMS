// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Card/DMSCardDefinition.h"
#include "DMSScenarioCardDefinition.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDMSGAME_API UDMSScenarioCardDefinition : public UDMSCardDefinition
{
	GENERATED_BODY()
	
public:
	/**
	* Flavor text of card effect.
	*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText EffectText_Back;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText FlavorText_Back;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText Name_Back;
};
