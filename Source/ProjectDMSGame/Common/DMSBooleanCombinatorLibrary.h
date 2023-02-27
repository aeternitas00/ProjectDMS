// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DMSBooleanCombinatorLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDMSGAME_API UDMSBooleanCombinatorLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintCallable, Category = "Combinator")
	static void ExecOperation(bool& iRv, const bool& NewValue, const uint8& Operator);

	UFUNCTION(BlueprintCallable, Category = "Combinator")
	static bool ParseCombinator(const TArray<bool>& Values, const FString& Combinator); /*const*/

};
