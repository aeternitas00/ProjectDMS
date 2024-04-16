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
public:

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int TestPlayerNum = 0;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<ADMSLocationBase*> Locations;

	UFUNCTION(BlueprintCallable)
	TArray<ADMSPlayerState*> GetDMSPlayers();

	UFUNCTION(BlueprintCallable)
	TArray<ADMSPlayerController*> GetDMSPlayerControllers();

	UFUNCTION(BlueprintCallable)
	void SetupDefaults();

	UFUNCTION(BlueprintImplementableEvent)
	ADMSSequence* CreateChaosTokenDrawSeq(ADMSSequence* ParentSeq);

	//Temp
	void SetPlayersFocusTarget(AActor* Target);

public:
	//ADMSGameState(const FObjectInitializer& Initializer);
};
