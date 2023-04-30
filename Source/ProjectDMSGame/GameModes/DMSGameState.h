// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "GameFramework/GameStateBase.h"
#include "DMSGameState.generated.h"

class ADMSLocationBase;
/**
 * 
 */
UCLASS()
class PROJECTDMSGAME_API ADMSGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<ADMSLocationBase*> Locations;

	// PhaseState, Manager
};
