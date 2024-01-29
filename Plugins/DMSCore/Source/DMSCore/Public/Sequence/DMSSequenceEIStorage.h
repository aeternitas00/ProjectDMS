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
class ADMSActiveEffect;

/**
 * Each sequence can have multiple possibilities when it comes to targets. 
 * This structure is designed to handle cases where the main target may differ from the apply target that the EffectDefinition should affect. 
 * ex ) Target [1 Player]. That player discard top [3 cards] from their deck. 
 *		In this case, a player targeted by "Target 1 Player" will be MainTarget, "top 3 cards" will be ApplyTargets.
 */
USTRUCT(BlueprintType)
struct FDMSSequenceEIStorage
{
	GENERATED_BODY()

	FDMSSequenceEIStorage(){}
	FDMSSequenceEIStorage(TScriptInterface<IDMSEffectorInterface> iTarget) :MainTarget(iTarget) {}

	/**
	 * Main target of this storage
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TScriptInterface<IDMSEffectorInterface> MainTarget;
	
	/**
	 * Apply targets of this storage.
	 * Refer for attaching effect Instance.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TScriptInterface<IDMSEffectorInterface>> ApplyTargets;

	/**
	 * Ref or created effect instances;
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<ADMSActiveEffect>> EIs;
};
