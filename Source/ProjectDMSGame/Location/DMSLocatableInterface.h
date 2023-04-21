// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "UObject/Interface.h"
#include "DMSLocatableInterface.generated.h"

class ADMSLocationBase;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDMSLocatableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTDMSGAME_API IDMSLocatableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//UFUNCTION(BlueprintCallable)
	//virtual ADMSLocationBase* GetCurrentLocation();

	//UFUNCTION(BlueprintCallable)
	//int GetDistanceWith(IDMSLocatableInterface* OtherObject);
};
