// Copyright JeongWoo Lee

#pragma once

#include "ProjectDMS.h"
#include "UObject/Interface.h"
#include "DMSLocatableInterface.generated.h"

class ADMSLocationBase;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UDMSLocatableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for convinience of using specialized effect that moves spawnable to container "Field.Location".
 * also can be used to connect spawnable to non-containable but conceptually "locatable" game object. 
 * ( The player's location will follow that of their character, but the 'query to find the current location' must also be able to be executed by the player. In this case, playerstate class can implement the LocatableInterface.)
 */
class PROJECTDMSGAME_API IDMSLocatableInterface
{
	GENERATED_BODY()
public:
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	//void SetCurrentLocation(ADMSLocationBase* iLoc);

	// Get current location of locatable with chained container.
	// if owner of the current located container implements the LocatableInterface, return container owner's current location.
	// Current location of the DMSLocationBase is itself.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	ADMSLocationBase* GetCurrentLocation();
	virtual ADMSLocationBase* GetCurrentLocation_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int GetDistanceWith(const TScriptInterface<IDMSLocatableInterface>& OtherObject);
	virtual int GetDistanceWith_Implementation(const TScriptInterface<IDMSLocatableInterface>& OtherObject);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool LocatingTo(ADMSLocationBase* TargetLocation);
	virtual bool LocatingTo_Implementation(ADMSLocationBase* TargetLocation);
};
