// Copyright JeongWoo Lee


#include "Character/DMSEnemyCharacterDefinition.h"
#include "System/DMSAssetManager.h"

FPrimaryAssetId UDMSEnemyCharacterDefinition::GetPrimaryAssetId() const
{
    return FPrimaryAssetId(UDMSAssetManager::EnemyCharacterAssetType, GetFName());
}
