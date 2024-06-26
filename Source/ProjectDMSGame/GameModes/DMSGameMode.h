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

#include "ProjectDMS.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "GameModes/DMSGameModeBase.h"
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
class PROJECTDMSGAME_API ADMSGameMode : public ADMSGameModeBase
{
	GENERATED_BODY()

public:
	ADMSGameMode();
	//ADMSGameMode(const FObjectInitializer& ObjectInitializer);

protected:
	/**
	 * Default classed used when spawning a DMSSpawnableBase derived class.
	 * If no class is specified in SpawnableData, it uses this class for spawning.
	 */
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	TMap<FPrimaryAssetType, TSubclassOf<ADMSSpawnableBase>> DefaultSpawningClasses;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	FGameplayTag DefaultCardSpawnContainer;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	int NumReadyPlayer = 0;

protected:
	virtual void BeginPlay() override;

public:

	virtual void PreInitializeComponents() override;

	//UFUNCTION(BlueprintCallable)
	//void RegisterPlayer_BP(APlayerController* PC);

	/**
	 * Spawn card with card data and owner info.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	ADMSCardBase* SpawnCard(const FDMSCardData& CardData, int32 OwnerID, const FGameplayTag& DefaultContainerName);
	virtual ADMSCardBase* SpawnCard_Implementation(const FDMSCardData& CardData, int32 OwnerID, const FGameplayTag& DefaultContainerName = FGameplayTag::EmptyTag);

	/**
	 * Setup DMS Game systems.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetupDMSGame();
	virtual void SetupDMSGame_Implementation();

	UFUNCTION(BlueprintCallable)
	void PlayerReady();

	UFUNCTION(BlueprintImplementableEvent)
	void OnAllPlayerReady();

	// Spawn DMSSpawnable in param location to Owner player.
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Spawn DMS Game Actor_ID"))
	ADMSSpawnableBase* SpawnDMSGameActor_ID(const UDMSSpawnableDataBase* ActorData, int32 OwnerID = -1, ADMSLocationBase* DefaultLocation = nullptr, const FTransform& inRelativeTransform = FTransform());

	template<typename ReturnType>
	ReturnType* SpawnDMSGameActor(const UDMSSpawnableDataBase* ActorData, int32 OwnerID = -1, ADMSLocationBase* DefaultLocation = nullptr, const FTransform& inRelativeTransform = FTransform());
	ADMSSpawnableBase* SpawnDMSGameActor(const TSubclassOf<ADMSSpawnableBase>& SpawningClass, const UDMSSpawnableDataBase* ActorData, int32 OwnerID = -1, ADMSLocationBase* DefaultLocation = nullptr, const FTransform& inRelativeTransform = FTransform());
	ADMSSpawnableBase* SpawnDMSGameActor(const UDMSSpawnableDataBase* ActorData, int32 OwnerID = -1, ADMSLocationBase* DefaultLocation = nullptr, const FTransform& inRelativeTransform = FTransform());

};

template<typename ReturnType>
ReturnType* ADMSGameMode::SpawnDMSGameActor(const UDMSSpawnableDataBase* ActorData, int32 OwnerID, ADMSLocationBase* DefaultLocation, const FTransform& inRelativeTransform)
{
	TSubclassOf<ADMSSpawnableBase> SpawningClass = ActorData->SpawningClass == nullptr && DefaultSpawningClasses.Contains(ActorData->GetPrimaryAssetId().PrimaryAssetType) ? DefaultSpawningClasses[ActorData->GetPrimaryAssetId().PrimaryAssetType] : nullptr;

	if (SpawningClass == nullptr || !SpawningClass->IsChildOf<ReturnType>()) return nullptr;

	return Cast<ReturnType>(SpawnDMSGameActor(SpawningClass, ActorData, OwnerID, DefaultLocation, inRelativeTransform));
}
