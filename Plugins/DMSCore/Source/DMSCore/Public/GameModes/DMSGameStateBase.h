// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DMSCoreIncludes.h"
#include "GameFramework/GameStateBase.h"
#include "Effect/DMSEffectorInterface.h"
#include "Effect/DMSEffectorOwnableInterface.h"
#include "DMSGameStateBase.generated.h"

class UDMSEIManagerComponent;
class ADMSPlayerStateBase;
class ADMSPlayerControllerBase;
/**
 * 
 */
UCLASS()
class DMSCORE_API ADMSGameStateBase : public AGameStateBase, public IDMSEffectorInterface, public IDMSEffectorOwnableInterface
{
	GENERATED_BODY()

protected:
	/**
	 * Manager objects / components.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Replicated)
	UDMSEffectHandler* EffectHandler;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Replicated)
	UDMSSeqManager* SequenceManager;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Replicated)
	UDMSNotifyManager* NotifyManager;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Replicated)
	UDMSPhaseManager* PhaseManager;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UDMSSeqManager> SequenceManagerClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UDMSPhaseManager> PhaseManagerClass;

	UPROPERTY(BlueprintReadOnly, Replicated)
	UDMSEIManagerComponent* EIManagerComponent;

	UPROPERTY(BlueprintReadOnly/*, Replicated*/)
	int32 LeaderPlayerID;

public:
	FORCEINLINE UDMSSeqManager* GetSequenceManager() { return SequenceManager; }
	FORCEINLINE UDMSEffectHandler* GetEffectHandler() { return EffectHandler; }
	FORCEINLINE UDMSNotifyManager* GetNotifyManager() { return NotifyManager; }
	FORCEINLINE UDMSPhaseManager* GetPhaseManager() { return PhaseManager; }

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
	
	virtual void PreInitializeComponents() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	/**
	 *
	 */
	UFUNCTION(BlueprintCallable)
	void RegisterNotifyObject(TScriptInterface<IDMSEffectorInterface> Object);


	virtual AActor* GetOwningPlayer() override { return this; }
	virtual int32 GetID() override { /* temp */return -1; }
	virtual ADMSPlayerControllerBase* GetWidgetOwner() override { return GetLeaderPlayerController(); }
};
