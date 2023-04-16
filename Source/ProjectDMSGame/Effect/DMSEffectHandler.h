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

#include "ProjectDMS.h"
#include "UObject/NoExportTypes.h"
#include "Common/DMSCommonDelegates.h"
#include "DMSEffectHandler.generated.h"

class UDMSEffectInstance;
class UDMSSequence;
class UDMSEffectNode;
class UDMSDataObjectSet;
class IDMSEffectorInterface;

/**
 *	working...
 */
UCLASS(/*ClassGroup = (Effect)*/)
class PROJECTDMSGAME_API UDMSEffectHandler : public UObject // or ActorComponent to attach GM
{
	GENERATED_BODY()
	
protected:
	
	/**
	 * Store all of effect instances in level.
	 * Used in GC and somewhere else.
	 */
	UPROPERTY()
	TArray<UDMSEffectInstance*> EIList;

	/**
	 * Structure for implementing chained delegates to ensure synchronization in the Resolve step.
	 */
	struct FResolveDelegateCounter {
		FOnResolveCompleted Delegate;
		uint8 Count=0;
		FResolveIteratingDelegate IteratingDelegate;
	};

	/**
	 * Store FResolveDelegateCounters for each sequences.
	 */
	TMap<UDMSSequence*, FResolveDelegateCounter> OnResolveCompletedMap;

	/**
	 * Function for sequential execution of Resolving EI in a sequence.
	 * Work with OnResolveCompletedMap.
	 * @param	Sequence					Resolving sequence.
	 */
	void ApplyNextEffectInstance(UDMSSequence* Sequence);

	

public:

	/**
	 * Remove non persistent EI from EIList.
	 */
	void CleanupNonPersistent();
	
	/** 
	 * Creating method without sequence. ( ex. setup object's owning effect. )
 	 * @param	SourceObject
	 * @param	SourceController
	 * @param	EffectNode
	 * @param	iSet
	 * @return	Created effect instances.
	 */
	TArray<UDMSEffectInstance*>/*EIHandle?*/ CreateEffectInstance(UObject* SourceObject, AActor* SourceController, UDMSEffectNode* EffectNode, UDMSDataObjectSet* iSet = nullptr);
	
	/**
	 * Creating method for general purpose.
	 * @param	Sequence
	 * @param	EffectNode
	 * @return	Created effect instances.
	 */
	TArray<UDMSEffectInstance*>/*EIHandle?*/ CreateEffectInstance(UDMSSequence* Sequence, UDMSEffectNode* EffectNode);

	/**
	 * Resolve param sequence and executed paramed lambda function when resolve completed.
	 * @param	Sequence					Resolving sequence.
	 * @param	OnResolveCompleted			Lambda executed when resolve completed.
	 */
	template <typename FuncFinished>
	void Resolve(UDMSSequence* Sequence, FuncFinished&& OnResolveCompleted);

};

