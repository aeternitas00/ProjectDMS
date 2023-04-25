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

public:
	ADMSLocationBase();

	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void InitializeLocation(const FDMSScenarioLocatingData& iLocData);

};
