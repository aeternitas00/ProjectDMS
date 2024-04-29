// Fill out your copyright notice in the Description page of Project Settings.


#include "Misc/DMSChaosTokenDefinition.h"
#include "System/DMSAssetManager.h"

FPrimaryAssetId UDMSChaosTokenDefinition::GetPrimaryAssetId() const
{
    return FPrimaryAssetId(UDMSAssetManager::ChaosTokenAssetType, GetFName());
}
