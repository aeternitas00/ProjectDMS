// Copyright JeongWoo Lee


#include "Card/DMSCardDefinition.h"
#include "System/DMSAssetManager.h"
#include "EffectSet/TypeBehavior/DMSEffectNode_TypeBehavior.h"

FPrimaryAssetId UDMSCardDefinition::GetPrimaryAssetId() const
{
    return FPrimaryAssetId(UDMSAssetManager::CardAssetType, GetFName());
}
