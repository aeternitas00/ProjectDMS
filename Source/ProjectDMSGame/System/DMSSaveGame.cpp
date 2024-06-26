// Copyright JeongWoo Lee


#include "System/DMSSaveGame.h"

void UDMSSaveGame::LoadSavedCardData()
{
	for ( auto& Data : SavedCardDatas ) Data.LoadCardDefinition();
}

void UDMSSaveGame::Serialize(FArchive& Ar)
{
	//DMS_LOG_SCREEN(TEXT("SAVEGAME SERIALIZED"));
	Super::Serialize(Ar);

	Ar << SavedCardDatas << SavedCharacterData << SavedChaosTokenDatas;

	//for (auto& Data : SavedCardDatas)
	//{
	//	Data.Serialize(Ar);
	//}
}
