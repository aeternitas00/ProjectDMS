// Copyright JeongWoo Lee


#include "Character/DMSPlayerCharacterData.h"

FArchive& operator<<(FArchive& Ar, FDMSPlayerCharacterData& CharacterData)
{
	//DMS_LOG_SCREEN(TEXT("FDMSCardData Serialized"));
	Ar << CharacterData.AssetID << CharacterData.Attributes;
	return Ar;
}
