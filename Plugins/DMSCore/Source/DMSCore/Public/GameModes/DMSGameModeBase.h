// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

/**
 * 	========================================
 *
 *	DMS_GAME_MODULE
 *
 *	Default rules of DMS GAME
 *
 *	========================================
 */

#include "DMSCoreIncludes.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "DMSGameModeBase.generated.h"


class UDMSEffectHandler;
class UDMSSeqManager;
class UDMSNotifyManager;
class UDMSPhaseManager;
class UDMSCardDefinition;
class IDMSEffectorInterface;
class ADMSGameStateBase;


/**
 *	========================================
 * 
 *	DMS Game specified GM. Arkham LCG-Like Phase controll, Enemy fightings and Card Upgrade system... 
 *	Save/Load upgrade data... etc.
 * 
 *  ========================================
 */
UCLASS(/*Blueprintable, Config = Game*/)
class DMSCORE_API ADMSGameModeBase: public AGameModeBase
{
	GENERATED_BODY()

public:
	ADMSGameModeBase();

protected:
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable,BlueprintPure,meta = (CompactNodeTitle = "DMS Game State"))
	ADMSGameStateBase* GetDMSGameState();
};

