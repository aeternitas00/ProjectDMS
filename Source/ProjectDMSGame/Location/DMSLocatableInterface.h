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
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCurrentLocation(ADMSLocationBase* iLoc);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	ADMSLocationBase* GetCurrentLocation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int GetDistanceWith(const TScriptInterface<IDMSLocatableInterface>& OtherObject);
	virtual int GetDistanceWith_Implementation(const TScriptInterface<IDMSLocatableInterface>& OtherObject);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool LocatingTo(ADMSLocationBase* TargetLocation);
	virtual bool LocatingTo_Implementation(ADMSLocationBase* TargetLocation);
};
