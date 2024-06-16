// Copyright JeongWoo Lee

#pragma once

#include "ProjectDMS.h"
#include "Common/DMSSpawnableDataBase.h"
#include "DMSLocationData.generated.h"

class UDMSEffectSet;
class ADMSLocationBase;


/**
 * DataAsset for storiging dms location.
 */
UCLASS()
class PROJECTDMSGAME_API UDMSLocationData : public UDMSSpawnableDataBase
{
	GENERATED_BODY()

public:
	/**
	 * Original name of location
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FName LocationName;

	/**
	 * Location's effect
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UDMSEffectSet> LocationEffect;

	/**
	 * Traits of location
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<FGameplayTag> LocationTraits;

	// location brush
	//UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	//FBrush LocationBG;

	// location gatherable resources ( further farming system )



	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
