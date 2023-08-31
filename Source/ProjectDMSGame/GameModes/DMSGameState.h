// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "GameModes/DMSGameStateBase.h"
#include "DMSGameState.generated.h"

//class UDMSEIManagerComponent;
class ADMSLocationBase;
class ADMSPlayerState;
/**
 * 
 */
UCLASS()
class PROJECTDMSGAME_API ADMSGameState : public ADMSGameStateBase
{
	GENERATED_BODY()
	
protected:
	//UPROPERTY(BlueprintReadOnly)
	//UDMSEIManagerComponent* EffectManagerComponent;

	//UPROPERTY(BlueprintReadOnly)
	//int32 LeaderPlayerID;

public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<ADMSLocationBase*> Locations;

	//UFUNCTION(BlueprintCallable)
	//APlayerController* GetLeaderPlayerController() { return FindPlayerControllerFromId(LeaderPlayerID); }

	//UFUNCTION(BlueprintCallable)
	//int32 GetLeaderPlayerID() {return LeaderPlayerID;}

	//UFUNCTION(BlueprintCallable)
	//bool SetLeaderPlayer(int32 LeaderID);

	UFUNCTION(BlueprintCallable)
	TArray<ADMSPlayerState*> GetDMSPlayers();

	UFUNCTION(BlueprintCallable)
	TArray<ADMSPlayerController*> GetDMSPlayerControllers();

	//UFUNCTION(BlueprintCallable,BlueprintPure)
	//APlayerState* FindPlayerFromId(int32 OwnerID);

	//UFUNCTION(BlueprintCallable, BlueprintPure)
	//APlayerController* FindPlayerControllerFromId(int32 OwnerID);
	//// PhaseState, Manager

	UFUNCTION(BlueprintCallable)
	void SetupDefaults();

	void SetPlayersFocusTarget(AActor* Target);
public:
	//ADMSGameState(const FObjectInitializer& Initializer);
};
