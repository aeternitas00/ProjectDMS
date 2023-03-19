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

#include "ProjectDMS.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "DMSGameMode.generated.h"


class UDMSEffectHandler;
class UDMSSeqManager;
class UDMSNotifyManager;
class UDMSCardDefinition;
class IDMSEffectorInterface;


/**
 *	========================================
 * 
 *	DMS Game specified GM. Arkham LCG-Like Phase controll, Enemy fightings and Card Upgrade system... 
 *	Save/Load upgrade data... etc.
 * 
 *  ========================================
 */
UCLASS(/*Blueprintable, Config = Game*/)
class PROJECTDMSGAME_API ADMSGameMode: public AGameModeBase
{
	GENERATED_BODY()

public:
	ADMSGameMode();
	//ADMSGameMode(const FObjectInitializer& ObjectInitializer);

protected:
	//private and getter()?
	UPROPERTY(BlueprintReadOnly, Instanced)
	UDMSEffectHandler* EffectHandler;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced)
	UDMSSeqManager* SequenceManager;
	UPROPERTY(BlueprintReadOnly, Instanced)
	UDMSNotifyManager* NotifyManager;

public:
	//static ADMSGameMode* GetDMSGameMode();

	FORCEINLINE UDMSSeqManager* GetSequenceManager() {return SequenceManager;}
	FORCEINLINE UDMSEffectHandler* GetEffectHandler() {return EffectHandler;}
	FORCEINLINE UDMSNotifyManager* GetNotifyManager() { return NotifyManager; }

//public:
// ADMSCardBase* RequestSpawnCard(UDMSCardDefinition* iCardDef,AActor* iOwner,const FName& DefaultContainerName);
//protected:
	void SpawnCardsFromDeck(class ADMSPlayerController* iPC);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	ADMSCardBase* SpawnCard(const UDMSCardDefinition* iCardDef,AActor* iOwner,const FName& DefaultContainerName=TEXT("Deck"));

	UFUNCTION(BlueprintCallable)
	void RegisterNotifyObject(TScriptInterface<IDMSEffectorInterface> Object);

	void PushSequence();
};

//DEPRECATED
//#define GetDMSGameMode() (Cast<ADMSGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
