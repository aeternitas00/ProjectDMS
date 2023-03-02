// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *	Combinator string parsing helper library
 *
 *	========================================
 */

#include "ProjectDMS.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DMSCombinatorLibrary.generated.h"

/**
 * Combinator 관련 함수 라이브러리
 */
UCLASS()
class PROJECTDMSGAME_API UDMSCombinatorLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Combinator")
	static bool ValidateCombinator(const FString& Combinator);

	//UFUNCTION(BlueprintCallable, Category = "Combinator")
	static void ExecOperation(bool& iRv, const bool& NewValue, const uint8& Operator);

	UFUNCTION(BlueprintCallable, Category = "Combinator")
	static bool ParseCombinator(const TArray<bool>& Values, const FString& Combinator); /*const*/

};

