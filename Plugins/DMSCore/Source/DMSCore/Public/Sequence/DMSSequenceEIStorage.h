// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *	
 *
 *	========================================
 */

#include "DMSCoreIncludes.h"
#include "DMSSequenceEIStorage.generated.h"

class IDMSEffectorInterface;
class UDMSEffectInstance;

USTRUCT(BlueprintType)
struct FDMSSequenceEIStorage
{
	GENERATED_BODY()

	FDMSSequenceEIStorage(){}
	FDMSSequenceEIStorage(TScriptInterface<IDMSEffectorInterface> iTarget) :MainTarget(iTarget) {}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TScriptInterface<IDMSEffectorInterface> MainTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TScriptInterface<IDMSEffectorInterface>> ApplyTargets;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<UDMSEffectInstance>> EIs;
};
