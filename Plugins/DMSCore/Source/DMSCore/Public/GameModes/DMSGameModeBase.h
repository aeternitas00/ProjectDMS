// Copyright JeongWoo Lee

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

