// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Effect/DMSEffectorActorBase.h"
#include "Location/DMSLocatableInterface.h"
#include "DMSLocationBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDMSGAME_API ADMSLocationBase : public ADMSEffectorActorBase, public IDMSLocatableInterface
{
	GENERATED_BODY()

public:
	ADMSLocationBase();

	// TObjectPtr<DMSLocationData> LocationData;
	// TArray<TObjectPtr<ADMSLocationBase>> ConnectingLocations;
};
