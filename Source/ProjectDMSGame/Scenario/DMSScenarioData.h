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
	TObjectPtr<UDMSLocationData> LocationAsset;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FTransform LocationOffset;

};

USTRUCT(BlueprintType)
struct FDMSConnectionData
{
	GENERATED_BODY()

	FDMSConnectionData() :StartLocationIndex(0), DestLocationIndex(0), bIsOneWay(false){}
	FDMSConnectionData(const uint8& Start , const uint8& End, const bool& IsOneWay):StartLocationIndex(Start), DestLocationIndex(End), bIsOneWay(IsOneWay){}
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	uint8 StartLocationIndex;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	uint8 DestLocationIndex;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bIsOneWay;

	bool DirectionIsEqual(const FDMSConnectionData& Other) const 
	{
		return (StartLocationIndex == Other.StartLocationIndex &&
			DestLocationIndex == Other.DestLocationIndex);
	}

	const FDMSConnectionData Reverse() const {
		return FDMSConnectionData(DestLocationIndex, StartLocationIndex, bIsOneWay);
	}

	bool operator==(const FDMSConnectionData& Other) const
	{
		if (bIsOneWay == true && bIsOneWay == Other.bIsOneWay)
			return DirectionIsEqual(Other);
	
		return (DirectionIsEqual(Other) || DirectionIsEqual(Other.Reverse()));
	}
	friend uint32 GetTypeHash(const FDMSConnectionData& i)
	{
		uint32 Hash = FCrc::MemCrc32(&i, sizeof(FDMSConnectionData));
		return Hash;
	}
};


/**
 * 
 */
UCLASS()
class PROJECTDMSGAME_API UDMSScenarioData : public UDataAsset
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
	TSet<FDMSConnectionData> Connections;
	TSet<FDMSConnectionData> OldConnections;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	uint8 StartingLocationIndex;
	uint8 OldStartingLocationIndex;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName ScenarioName;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FText ScenarioDescription;
	// Locations, Offset

	// Scenario rules ( as effect )


	virtual void PreEditChange(FProperty* PropertyAboutToChange) override;
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
};
