// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DMSCoreIncludes.h"
#include "GameFramework/GameStateBase.h"
#include "Effect/DMSEffectorInterface.h"
#include "Effect/DMSEffectorOwnableInterface.h"
#include "DMSGameStateBase.generated.h"

class UDMSEffectHandler;
class UDMSSeqManager;
class UDMSNotifyManager;
class UDMSPhaseManager;
class UDMSEIManagerComponent;
class UDMSSelectorManager;
class ADMSPlayerStateBase;
class ADMSPlayerControllerBase;

/**
 *	DMS Game specified GS. Arkham LCG-Like Phase controll, Enemy fightings and Card Upgrade system... 
 *	Save/Load upgrade data... etc.
 */
UCLASS()
class DMSCORE_API ADMSGameStateBase : public AGameStateBase, public IDMSEffectorInterface, public IDMSEffectorOwnableInterface
{
	GENERATED_BODY()

protected:
	bool bNeedToCleanup;
	/**
	 * Manager objects / components.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Replicated)
	TObjectPtr<UDMSEffectHandler> EffectHandler;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Replicated)
	TObjectPtr<UDMSSeqManager> SequenceManager;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Replicated)
	TObjectPtr<UDMSNotifyManager> NotifyManager;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Replicated)
	TObjectPtr<UDMSPhaseManager> PhaseManager;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Replicated)
	TObjectPtr<UDMSSelectorManager> SelectorManager;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UDMSSeqManager> SequenceManagerClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UDMSPhaseManager> PhaseManagerClass;

	UPROPERTY(BlueprintReadOnly, Replicated)
	TObjectPtr<UDMSEIManagerComponent> EIManagerComponent;

	UPROPERTY(BlueprintReadOnly/*, Replicated*/)
	int32 LeaderPlayerID;

public:
	FORCEINLINE UDMSSeqManager* GetSequenceManager() { return SequenceManager; }
	FORCEINLINE UDMSEffectHandler* GetEffectHandler() { return EffectHandler; }
	FORCEINLINE UDMSNotifyManager* GetNotifyManager() { return NotifyManager; }
	FORCEINLINE UDMSPhaseManager* GetPhaseManager() { return PhaseManager; }
	FORCEINLINE UDMSSelectorManager* GetSelectorManager() { return SelectorManager; }


	UFUNCTION(BlueprintCallable)
	ADMSPlayerControllerBase* GetLeaderPlayerController() { return FindPlayerControllerFromId(LeaderPlayerID); }

	UFUNCTION(BlueprintCallable)
	int32 GetLeaderPlayerID() {return LeaderPlayerID;}

	UFUNCTION(BlueprintCallable)
	bool SetLeaderPlayer(int32 LeaderID);

	UFUNCTION(BlueprintCallable,BlueprintPure)
	ADMSPlayerStateBase* FindPlayerFromId(int32 OwnerID);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ADMSPlayerControllerBase* FindPlayerControllerFromId(int32 OwnerID);
	// PhaseState, Manager

public:
	ADMSGameStateBase(const FObjectInitializer& Initializer);
	
	virtual void Tick(float Deltatime) override;
	virtual void PreInitializeComponents() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	/**
	 * Register object to Notifying system.
	 * Use for actors already placed in the level.
	 */
	UFUNCTION(BlueprintCallable)
	void RegisterNotifyObject(TScriptInterface<IDMSEffectorInterface> Object);

	void NotifyNeedToCleanup(){	bNeedToCleanup = true; }
	// Interface implements
	virtual AActor* GetOwningPlayer() override { return this; }
	virtual int32 GetID() override { /* temp */return -1; }
	virtual ADMSPlayerControllerBase* GetWidgetOwner() override { return GetLeaderPlayerController(); }
};
