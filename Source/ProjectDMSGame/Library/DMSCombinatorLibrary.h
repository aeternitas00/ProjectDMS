// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DMSCombinatorLibrary.generated.h"

/**
 * Combinator ���� �Լ� ���̺귯��
 */
UCLASS()
class PROJECTDMSGAME_API UDMSCombinatorLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Combinator")
	static bool GetCombinatedResult(const TArray<bool>& iBoolArr,const FString& Combinator);

	UFUNCTION(BlueprintCallable, Category = "Combinator")
	static bool ValidateCombinator(const FString& Combinator);
};

