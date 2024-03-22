// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *	Handling Effect Instances.
 *
 *	========================================
 */

#include "DMSCoreIncludes.h"
#include "Sequence/DMSSequenceEIStorage.h"
#include "Components/ActorComponent.h"
#include "Common/DMSCommonDelegates.h"
#include "DMSEffectHandler.generated.h"

class ADMSActiveEffect;
class UDMSSequence;
class UDMSEffectNode;
class UDMSDataObjectSet;
class IDMSEffectorInterface;

/**
* Structure for implementing chained delegates to ensure synchronization in the Resolve step.
*/
USTRUCT()
struct FResolveDelegateCounter {
	GENERATED_BODY()
	
	uint8 Count=0;
	FResolveIteratingDelegate Iterator;
	TArray<ADMSActiveEffect*> ApplyingEIs;
};

/**
 *	working...
 */
UCLASS(BlueprintType, Blueprintable/*ClassGroup = (Effect)*/)
class DMSCORE_API UDMSEffectHandler : public UActorComponent // or ActorComponent to attach GM
{
	GENERATED_BODY()
	
protected:
	
	/**
	 * Store all of effect instances in level.
	 * Used in GC and somewhere else.
	 */
	UPROPERTY()
	TArray<ADMSActiveEffect*> EIList;

	/**
	 * Store FResolveDelegateCounters for each sequences.
	 */
	UPROPERTY()
	TMap<TObjectPtr<UDMSSequence>, FResolveDelegateCounter> OnResolveCompletedMap;

	/**
	 * Function for sequential execution of Resolving EI in a sequence.
	 * Work with OnResolveCompletedMap.
	 * @param	Sequence					Resolving sequence.
	 */
	void ApplyNextEffectInstance(UDMSSequence* SourceSequence, const FOnResolveCompleted& OnResolveCompleted, bool PrevSucceeded);
	//void ApplyNextEffectInstance_Preview(UDMSSequence* SourceSequence, bool PrevSucceeded);

	ADMSActiveEffect* ApplyAEGetter(UDMSSequence* OwnerSequence);

	//FResolveIteratingDelegate ResolveIteratingDelegate;

public:
	UDMSEffectHandler();

	/**
	 * Remove non persistent EI from EIList.
	 */
	void CleanupNonPersistent();
	
	/** 
	 * Creating method without sequence. ( ex. setup object's owning effect. )
 	 * @param	SourceObject
	 * @param	SourcePlayer
	 * @param	Target			Attaching target
	 * @param	EffectNode
	 * @param	iSet
	 * @return	Created effect instances.
	 */
	ADMSActiveEffect* CreatePersistentActiveEffect(AActor* SourceObject, AActor* SourcePlayer, AActor* Target, UDMSEffectNode* EffectNode, UDMSDataObjectSet* iSet = nullptr);
	
	/**
	 * Creating method for general purpose.
	 * @param	Sequence
	 * @param	EffectNode
	 * @return	Created effect instances.
	 */
	TArray<ADMSActiveEffect*> CreateApplyingActiveEffect(UDMSSequence* Sequence, UDMSEffectNode* EffectNode);

	/**
	 * Resolve param sequence and executed paramed lambda function when resolve completed.
	 * @param	Sequence					Resolving sequence.
	 * @param	OnResolveCompleted			Lambda executed when resolve completed.
	 */
	void Resolve(UDMSSequence* Sequence, const FOnResolveCompleted& OnResolveCompleted);
};

