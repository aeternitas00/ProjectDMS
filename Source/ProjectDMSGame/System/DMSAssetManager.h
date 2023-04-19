// Fill out your copyright notice in the Description page of Project Settings.

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
	static const FPrimaryAssetType PlayerCharacterAssetType;
	static const FPrimaryAssetType EnemyCharacterAssetType;

	static UDMSAssetManager& Get();

	UDMSCardDefinition* ForceLoadCard(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning = true);
};
