// Copyright JeongWoo Lee

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
class ADMSActiveEffect;
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
	/**
	 * Referencing card asset.
	 */
	UPROPERTY(BlueprintReadWrite)
	FPrimaryAssetId CardAssetID;

	/**
	 * Saved persistent effects.
	 */
	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<ADMSActiveEffect>> AttachedEffect;

	/**
	 * Caching spawned card actor for saving.
	 */
	UPROPERTY(BlueprintReadWrite, Transient)
	TArray<TObjectPtr<ADMSCardBase>> SpawnedCardActor;

	/**
	 * Caching card definition by CardAssetID for saving.
	 */
	UPROPERTY(BlueprintReadWrite, Transient)
	TObjectPtr<UDMSCardDefinition> CardDefinition;

	void LoadCardDefinition();

	friend FArchive& operator<<(FArchive& Ar, FDMSCardData& MyStruct);

};
