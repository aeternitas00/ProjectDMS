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
class ADMSGameState;
class ADMSSpawnableBase;

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

	/**
	 * Manager objects / components.
	 */
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

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	TMap<FPrimaryAssetType, TSubclassOf<ADMSSpawnableBase>> DefaultSpawningClasses;

	//UPROPERTY(BlueprintReadOnly)
	//ADMSGameState* DMSGameState;

protected:

	virtual void BeginPlay() override;
public:
	FORCEINLINE UDMSSeqManager* GetSequenceManager() {return SequenceManager;}
	FORCEINLINE UDMSEffectHandler* GetEffectHandler() {return EffectHandler;}
	FORCEINLINE UDMSNotifyManager* GetNotifyManager() { return NotifyManager; }
	FORCEINLINE UDMSPhaseManager* GetPhaseManager() { return PhaseManager; }
	FORCEINLINE ADMSGameState* GetDMSGameState();

	virtual void PreInitializeComponents() override;

	/**
	 *
	 */
	void SpawnCardsFromDeck(class ADMSPlayerController* iPC);

	/**
	 *
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	ADMSCardBase* SpawnCard(const FDMSCardData& CardData,AActor* iOwner,const FName& DefaultContainerName=TEXT("Deck"));
	virtual ADMSCardBase* SpawnCard_Implementation(const FDMSCardData& CardData, AActor* iOwner, const FName& DefaultContainerName = TEXT("Deck"));
	/**
	 *
	 */
	UFUNCTION(BlueprintCallable)
	void RegisterNotifyObject(TScriptInterface<IDMSEffectorInterface> Object);
	//void RegisterNotifyObject_Native(TScriptInterface<IDMSEffectorInterface> Object);
	//void RegisterNotifyObject_Native(UObject* Object);



	// Deprecated ( Scenario => Direct to Level )
	//UFUNCTION(BlueprintCallable)
	//void LoadAndSpawnScenario(class UDMSScenarioData* ScenarioData);

	UFUNCTION(BlueprintCallable,meta = (DisplayName="Spawn DMS Game Actor"))
	ADMSSpawnableBase* SpawnDMSGameActor_BP(const UDMSSpawnableDataBase* ActorData, AActor* NewOwner = nullptr, ADMSLocationBase* DefaultLocation = nullptr, const FTransform& inRelativeTransform = FTransform());

	template<typename ReturnType>
	ReturnType* SpawnDMSGameActor(const UDMSSpawnableDataBase* ActorData, AActor* NewOwner = nullptr, ADMSLocationBase* DefaultLocation = nullptr, const FTransform& inRelativeTransform = FTransform());
	ADMSSpawnableBase* SpawnDMSGameActor(const TSubclassOf<ADMSSpawnableBase>& SpawningClass, const UDMSSpawnableDataBase* ActorData, AActor* NewOwner = nullptr, ADMSLocationBase* DefaultLocation = nullptr, const FTransform& inRelativeTransform = FTransform());
	ADMSSpawnableBase* SpawnDMSGameActor(const UDMSSpawnableDataBase* ActorData, AActor* NewOwner = nullptr, ADMSLocationBase* DefaultLocation = nullptr, const FTransform& inRelativeTransform = FTransform());
};

