// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DMSPlayerCharacterDefinition.h"
#include "System/DMSAssetManager.h"

FPrimaryAssetId UDMSPlayerCharacterDefinition::GetPrimaryAssetId() const
{
    return FPrimaryAssetId(UDMSAssetManager::PlayerCharacterAssetType, GetFName());
}
