// Fill out your copyright notice in the Description page of Project Settings.


#include "System/DMSSaveGame.h"

void UDMSSaveGame::Serialize(FArchive& Ar)
{
	DMS_LOG_SCREEN(TEXT("SAVEGAME SERIALIZED"));
	Super::Serialize(Ar);

	Ar << SavedCardDatas;

	//for (auto& Data : SavedCardDatas)
	//{
	//	Data.Serialize(Ar);
	//}
}
