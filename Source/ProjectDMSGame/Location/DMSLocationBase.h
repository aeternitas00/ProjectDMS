// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Effect/DMSEffectorActorBase.h"
#include "Location/DMSLocatableInterface.h"
#include "DMSLocationBase.generated.h"

class UDMSLocationData;

/**
 * 
 */
UCLASS()
class PROJECTDMSGAME_API ADMSLocationBase : public ADMSEffectorActorBase, public IDMSLocatableInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UDMSLocationData> LocationData;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<TObjectPtr<ADMSLocationBase>> ConnectingLocations;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<TScriptInterface<IDMSLocatableInterface>> ActorsOnLocation;

public:
	ADMSLocationBase();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnActorEntered(const TScriptInterface<IDMSLocatableInterface>& Locatable);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnActorLeaved(const TScriptInterface<IDMSLocatableInterface>& Locatable);

	void InitializeLocation_Native(const FDMSScenarioLocatingData& iLocData);

	UFUNCTION(BlueprintImplementableEvent)
	void InitializeLocation(const FDMSScenarioLocatingData& iLocData);

	UFUNCTION(BlueprintCallable)
	static void MoveActorToDMSLocation(ADMSLocationBase* Dest, const TScriptInterface<IDMSLocatableInterface>& Locatable);

	UFUNCTION(BlueprintCallable)
	static void ConnectLocations(ADMSLocationBase* Start,ADMSLocationBase* Dest,const bool& IsOneWay);

public:
	ADMSLocationBase* GetCurrentLocation_Implementation(){return this;}
	bool LocatingTo_Implementation(ADMSLocationBase* TargetLocation){return false;}
};
