// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_GAME_MODULE
 *
 *	Base of Card actor in DMS GAME
 *
 *	========================================
 */

#include "ProjectDMS.h"
#include "DMSCardData.generated.h"

class UDMSCardDefinition;
class UDMSEffectInstance;
class ADMSCardBase;

/**
 * 	========================================
 *
 *	Serializable player Card data.
 *
 *	========================================
 *
 * 	초안 :: 필요 사항 명세화 부터.
 */
USTRUCT(BlueprintType)
struct FDMSCardData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId CardAssetID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UDMSEffectInstance*> AttachedEffect;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TArray<ADMSCardBase*> SpawnedCardActor;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	const UDMSCardDefinition* CardDefinition;

	void LoadCardDefinition();
	void CachingCardActor() {}
	void CollectSavingDataFromActor() {}

	//void Serialize(FArchive& Ar);


	friend FArchive& operator<<(FArchive& Ar, FDMSCardData& MyStruct);
};