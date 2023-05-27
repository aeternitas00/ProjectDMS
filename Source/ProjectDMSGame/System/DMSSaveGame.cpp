// Fill out your copyright notice in the Description page of Project Settings.


#include "System/DMSSaveGame.h"

void UDMSSaveGame::LoadSavedCardData()
{
	for ( auto& Data : SavedCardDatas) Data.LoadCardDefinition();
}

void UDMSSaveGame::Serialize(FArchive& Ar)
{
	//DMS_LOG_SCREEN(TEXT("SAVEGAME SERIALIZED"));
	Super::Serialize(Ar);

	Ar << SavedCardDatas << SavedCharacterData;

	//for (auto& Data : SavedCardDatas)
	//{
	//	Data.Serialize(Ar);
	//}
}
