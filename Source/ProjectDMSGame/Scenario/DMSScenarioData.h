// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Engine/DataAsset.h"
#include "DMSScenarioData.generated.h"

class UDMSLocationData;
class UDMSCharacterData;


/**
 * Data for scenario selection.
 */
UCLASS()
class PROJECTDMSGAME_API UDMSScenarioData : public UDataAsset
{
	GENERATED_BODY()
	
public:

	// BGM

	// theme

	//UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	//TArray<FDMSScenarioLocatingData> Locations;

	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	//TSet<FDMSConnectionData> Connections;
	//TSet<FDMSConnectionData> OldConnections;

	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	//uint8 StartingLocationIndex;
	//uint8 OldStartingLocationIndex;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName ScenarioName;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FText ScenarioDescription;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FPrimaryAssetId LevelAsset;



	//virtual void PreEditChange(FProperty* PropertyAboutToChange) override;
	//virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
};
