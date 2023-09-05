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
	//ADMSGameMode(const FObjectInitializer& ObjectInitializer);

protected:

	/**
	 * Manager objects / components.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced)
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

	//UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	//TMap<FPrimaryAssetType, TSubclassOf<ADMSSpawnableBase>> DefaultSpawningClasses;

	//UPROPERTY(BlueprintReadOnly)
	//ADMSGameState* DMSGameState;

protected:

	virtual void BeginPlay() override;
public:
	FORCEINLINE UDMSSeqManager* GetSequenceManager() {return SequenceManager;}
	FORCEINLINE UDMSEffectHandler* GetEffectHandler() {return EffectHandler;}
	FORCEINLINE UDMSNotifyManager* GetNotifyManager() { return NotifyManager; }
	FORCEINLINE UDMSPhaseManager* GetPhaseManager() { return PhaseManager; }


	UFUNCTION(BlueprintCallable,BlueprintPure,meta = (CompactNodeTitle = "DMS Game State"))
	ADMSGameStateBase* GetDMSGameState();

	virtual void PreInitializeComponents() override;

	/**
	 *
	 */
	UFUNCTION(BlueprintCallable)
	void RegisterNotifyObject(TScriptInterface<IDMSEffectorInterface> Object);

	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	//void SetupDMSGame();
	//virtual void SetupDMSGame_Implementation();

	//UFUNCTION(BlueprintCallable, meta = (DisplayName = "Spawn DMS Game Actor_ID"))
	//ADMSSpawnableBase* SpawnDMSGameActor_ID(const UDMSSpawnableDataBase* ActorData, int32 OwnerID = -1, ADMSLocationBase* DefaultLocation = nullptr, const FTransform& inRelativeTransform = FTransform());

	//template<typename ReturnType>
	//ReturnType* SpawnDMSGameActor(const UDMSSpawnableDataBase* ActorData, int32 OwnerID = -1, ADMSLocationBase* DefaultLocation = nullptr, const FTransform& inRelativeTransform = FTransform());
	//ADMSSpawnableBase* SpawnDMSGameActor(const TSubclassOf<ADMSSpawnableBase>& SpawningClass, const UDMSSpawnableDataBase* ActorData, int32 OwnerID = -1, ADMSLocationBase* DefaultLocation = nullptr, const FTransform& inRelativeTransform = FTransform());
	//ADMSSpawnableBase* SpawnDMSGameActor(const UDMSSpawnableDataBase* ActorData, int32 OwnerID = -1, ADMSLocationBase* DefaultLocation = nullptr, const FTransform& inRelativeTransform = FTransform());

};

//template<typename ReturnType>
//ReturnType* ADMSGameMode::SpawnDMSGameActor(const UDMSSpawnableDataBase* ActorData, int32 OwnerID, ADMSLocationBase* DefaultLocation, const FTransform& inRelativeTransform)
//{
//	TSubclassOf<ADMSSpawnableBase> SpawningClass = ActorData->SpawningClass == nullptr && DefaultSpawningClasses.Contains(ActorData->GetPrimaryAssetId().PrimaryAssetType) ? DefaultSpawningClasses[ActorData->GetPrimaryAssetId().PrimaryAssetType] : nullptr;
//
//	if (SpawningClass == nullptr || !SpawningClass->IsChildOf<ReturnType>()) return nullptr;
//
//	return Cast<ReturnType>(SpawnDMSGameActor(SpawningClass, ActorData, OwnerID, DefaultLocation, inRelativeTransform));
//}
