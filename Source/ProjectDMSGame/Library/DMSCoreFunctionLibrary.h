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
#include "DMSCoreFunctionLibrary.generated.h"

class ADMSGameMode;
class UDMSSeqManager;
class UDMSNotifyManager;
class UDMSEffectHandler;

/**
 * 
 */
UCLASS()
class PROJECTDMSGAME_API UDMSCoreFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Project DMS")
	static ADMSGameMode* GetDMSGameMode();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Project DMS")
	static UDMSSeqManager* GetDMSSequenceManager();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Project DMS")
	static UDMSNotifyManager* GetDMSNotifyManager();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Project DMS")
	static UDMSEffectHandler* GetDMSEffectHandler();

	// Helper function
	// Attribute Library?
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Project DMS")
	static bool GetAttributeFromActor(AActor* iActor, const FName& Name, float& outValue);

	// Helper function
	// Card Library?
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Project DMS")
	static bool CheckCardIsCommitable(ADMSCardBase* iCard);
};
