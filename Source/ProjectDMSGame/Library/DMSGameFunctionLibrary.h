// Fill out your copyright notice in the Description page of Project Settings.

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
	static bool GetAttributeFromActor(AActor* iActor, const FGameplayTag& Name, float& outValue);

	//UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Project DMS")
	//static bool CheckCardIsCommitable(ADMSCardBase* iCard);
};
