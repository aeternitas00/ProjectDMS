// Copyright JeongWoo Lee


#include "Location/DMSLocationData.h"
#include "Effect/DMSEffectDefinition.h"
#include "Location/DMSLocationBase.h"
#include "System/DMSAssetManager.h"

FPrimaryAssetId UDMSLocationData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(UDMSAssetManager::LocationAssetType, GetFName());
}
