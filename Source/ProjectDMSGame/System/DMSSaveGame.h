// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "GameFramework/SaveGame.h"
#include "Card/DMSCardData.h"
#include "DMSSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDMSGAME_API UDMSSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<FDMSCardData> SavedCardDatas;
	// Decklist ( original card data asset, attached effects, ~~~ )

	// Character datas ( exp, ~~ )

	// Scenario datas ( campain progress, ~~ )
	UFUNCTION(BlueprintCallable)
	void LoadSavedCardData();

protected:
	virtual void Serialize(FArchive& Ar) override;
};
