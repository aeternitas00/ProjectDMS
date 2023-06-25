// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "GameFramework/SaveGame.h"
#include "Card/DMSCardData.h"
#include "Character/DMSPlayerCharacterData.h"
#include "DMSSaveGame.generated.h"

/**
 *	Save data per player profile.
 */
UCLASS()
class PROJECTDMSGAME_API UDMSSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	// Decklist ( original card data asset, attached effects, ~~~ )
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<FDMSCardData> SavedCardDatas;

	// Character datas ( exp, ~~ )
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FDMSPlayerCharacterData	SavedCharacterData;


	// Scenario datas ( campain progress, ~~ )

	/**
	 *	Experince point
	 */
	UPROPERTY(BlueprintReadWrite)
	int32 EXP;

	/**
	 *	Current Level
	 */
	UPROPERTY(BlueprintReadWrite)
	int32 Level;

	/**
	 *	Loads card assets used by this players with [SavedCardDatas].
	 */
	UFUNCTION(BlueprintCallable)
	void LoadSavedCardData();

	virtual void Serialize(FArchive& Ar) override;
};
