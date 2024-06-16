// Copyright JeongWoo Lee

#pragma once

/**
 * 	========================================
 *
 *	DMS_GAME_MODULE ?
 *
 *	Simple singleton getter for DMS ones.
 *
 *	========================================
 */

#include "ProjectDMS.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DMSGameFunctionLibrary.generated.h"

class UDMSPhaseManager;
class UDMSConditionObjectBase;
struct FDMSCardData;

/**
 * 
 */
UCLASS()
class PROJECTDMSGAME_API UDMSGameFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static void SetCardDataOwner(const FDMSCardData& Data, UObject* Owner);

	// Helper functions
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Project DMS")
	static UDMSAttribute* GetAttributeFromActor(AActor* iActor, const FGameplayTagContainer& AttributeTag);
};
