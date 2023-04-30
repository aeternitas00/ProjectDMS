// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Engine/DataAsset.h"
#include "DMSLocationData.generated.h"

class UDMSEffectSet;
class ADMSLocationBase;
/**
 * 
 */
UCLASS()
class PROJECTDMSGAME_API UDMSLocationData : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<ADMSLocationBase> LocationClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FName LocationName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UDMSEffectSet> LocationEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<FGameplayTag> LocationTraits;

	// location brush
	//UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	//FBrush LocationBG;

	// location gatherable resources ( further farming system )
};
