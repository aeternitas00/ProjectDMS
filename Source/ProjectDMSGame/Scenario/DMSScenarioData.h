// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Engine/DataAsset.h"
#include "DMSScenarioData.generated.h"

class UDMSLocationData;

USTRUCT(BlueprintType)
struct FDMSScenarioLocatingData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSoftObjectPtr<UDMSLocationData> LocationAsset;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FTransform LocationOffset;
};
/**
 * 
 */
UCLASS()
class PROJECTDMSGAME_API UDMSScenarioData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	//FBrush? ScenarioBG;

	// BGM

	// theme

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	TArray<FDMSScenarioLocatingData> Locations;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName ScenarioName;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FText ScenarioDescription;
	// Locations, Offset

	// Scenario rules ( as effect )
};
