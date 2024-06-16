// Copyright JeongWoo Lee

#pragma once

#include "ProjectDMS.h"
#include "Engine/AssetManager.h"
#include "DMSAssetManager.generated.h"

class UDMSCardDefinition;
/**
 *
 */
UCLASS()
class PROJECTDMSGAME_API UDMSAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	UDMSAssetManager() {}

	static const FPrimaryAssetType CardAssetType;
	static const FPrimaryAssetType ChaosTokenAssetType;
	static const FPrimaryAssetType PlayerCharacterAssetType;
	static const FPrimaryAssetType EnemyCharacterAssetType;
	static const FPrimaryAssetType LocationAssetType;
	static UDMSAssetManager& Get();

	UDMSCardDefinition* ForceLoadCard(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning = true);
};
