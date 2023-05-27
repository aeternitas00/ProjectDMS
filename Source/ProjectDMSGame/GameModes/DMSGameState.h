// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "GameFramework/GameStateBase.h"
#include "Effect/DMSEffectorInterface.h"
#include "DMSGameState.generated.h"

class UDMSEIManagerComponent;
class ADMSLocationBase;
class ADMSPlayerState;
/**
 * 
 */
UCLASS()
class PROJECTDMSGAME_API ADMSGameState : public AGameStateBase, public IDMSEffectorInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly)
	UDMSEIManagerComponent* EffectManagerComponent;

	UPROPERTY(BlueprintReadOnly)
	int32 LeaderPlayerID;

public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<ADMSLocationBase*> Locations;

	UFUNCTION(BlueprintCallable)
	APlayerController* GetLeaderPlayerController() { return FindPlayerControllerFromId(LeaderPlayerID); }

	UFUNCTION(BlueprintCallable)
	int32 GetLeaderPlayerID() {return LeaderPlayerID;}

	UFUNCTION(BlueprintCallable)
	bool SetLeaderPlayer(int32 LeaderID);

	UFUNCTION(BlueprintCallable,BlueprintPure)
	APlayerState* FindPlayerFromId(int32 OwnerID);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	APlayerController* FindPlayerControllerFromId(int32 OwnerID);
	// PhaseState, Manager

public:
	ADMSGameState(const FObjectInitializer& Initializer);

	virtual AActor* GetOwningPlayer() override { return this; }
};
