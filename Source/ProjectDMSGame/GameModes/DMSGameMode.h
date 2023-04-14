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
class UDMSPhaseManager;
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
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Instanced)
	UDMSEffectHandler* EffectHandler;
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly)
	UDMSSeqManager* SequenceManager;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced)
	UDMSNotifyManager* NotifyManager;
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly)
	UDMSPhaseManager* PhaseManager;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UDMSSeqManager> SequenceManagerClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UDMSPhaseManager> PhaseManagerClass;

public:
	FORCEINLINE UDMSSeqManager* GetSequenceManager() {return SequenceManager;}
	FORCEINLINE UDMSEffectHandler* GetEffectHandler() {return EffectHandler;}
	FORCEINLINE UDMSNotifyManager* GetNotifyManager() { return NotifyManager; }
	FORCEINLINE UDMSPhaseManager* GetPhaseManager() { return PhaseManager; }

	virtual void PreInitializeComponents() override;

	void SpawnCardsFromDeck(class ADMSPlayerController* iPC);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	ADMSCardBase* SpawnCard(const FDMSCardData& CardData,AActor* iOwner,const FName& DefaultContainerName=TEXT("Deck"));

	UFUNCTION(BlueprintCallable)
	void RegisterNotifyObject(TScriptInterface<IDMSEffectorInterface> Object);

};

//DEPRECATED
//#define GetDMSGameMode() (Cast<ADMSGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
