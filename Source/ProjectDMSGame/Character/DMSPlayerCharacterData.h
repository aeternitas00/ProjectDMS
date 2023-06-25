// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_GAME_MODULE
 *
 *	DMS Game specific character data form.
 *
 * =========================================
 */

#include "ProjectDMS.h"
#include "Attribute/DMSSerializedAttribute.h"
#include "DMSPlayerCharacterData.generated.h"

/**
 * SaveGame / Loading sequence ���� �÷��̾� �����͸� �а� �� �� ��� �� ����ü.
 */
USTRUCT(BlueprintType)
struct FDMSPlayerCharacterData 
{
	GENERATED_BODY()

public:
	/**
	 * AssetID of Character dataasset
	 */
	UPROPERTY(BlueprintReadWrite)
	FPrimaryAssetId		AssetID;

	/**
	 * Saved Attribute datas.
	 */
	UPROPERTY(BlueprintReadWrite)
	TArray<FDMSSerializedAttribute> Attributes;
	//...

	friend FArchive& operator<<(FArchive& Ar, FDMSPlayerCharacterData& CharacterData);
};