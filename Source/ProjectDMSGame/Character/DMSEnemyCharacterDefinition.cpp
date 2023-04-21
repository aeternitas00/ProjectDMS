// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DMSEnemyCharacterDefinition.h"
#include "System/DMSAssetManager.h"

FPrimaryAssetId UDMSEnemyCharacterDefinition::GetPrimaryAssetId() const
{
    return FPrimaryAssetId(UDMSAssetManager::EnemyCharacterAssetType, GetFName());
}
