// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Components/ActorComponent.h"
#include "DMSCardVisualComponent.generated.h"


UCLASS( ClassGroup=(Card), meta=(BlueprintSpawnableComponent) )
class PROJECTDMSGAME_API UDMSCardVisualComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDMSCardVisualComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
