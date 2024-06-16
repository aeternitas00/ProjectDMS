// Copyright JeongWoo Lee


#include "Character/DMSPlayerCharacterDefinition.h"
#include "System/DMSAssetManager.h"

FPrimaryAssetId UDMSPlayerCharacterDefinition::GetPrimaryAssetId() const
{
    return FPrimaryAssetId(UDMSAssetManager::PlayerCharacterAssetType, GetFName());
}
