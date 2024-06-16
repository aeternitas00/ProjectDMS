// Copyright JeongWoo Lee


#include "Misc/DMSChaosTokenDefinition.h"
#include "System/DMSAssetManager.h"

FPrimaryAssetId UDMSChaosTokenDefinition::GetPrimaryAssetId() const
{
    return FPrimaryAssetId(UDMSAssetManager::ChaosTokenAssetType, GetFName());
}
