// Copyright JeongWoo Lee

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
#include "Common/DMSSynchronousTaskWorker.h"
#include "DMSEffectHandler.generated.h"

class ADMSActiveEffect;
class ADMSSequence;
class UDMSEffectNode;
class IDMSEffectorInterface;

//DECLARE_DYNAMIC_DELEGATE_OneParam(FOnResolveCompleted, bool, Succeed);


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
	ADMSActiveEffect* CreatePersistentActiveEffect(AActor* SourceObject, AActor* SourcePlayer, AActor* Target, UDMSEffectNode* EffectNode);
	
	/**
	 * Creating method for general purpose.
	 * @param	Sequence
	 * @param	EffectNode
	 * @return	Created effect instances.
	 */
	TArray<ADMSActiveEffect*> CreateApplyingActiveEffect(ADMSSequence* Sequence, UDMSEffectNode* EffectNode);

	/**
	 * Resolve param sequence and executed paramed lambda function when resolve completed.
	 * @param	Sequence					Resolving sequence.
	 * @param	OnResolveCompleted			Lambda executed when resolve completed.
	 */
	//void Resolve(ADMSSequence* Sequence, const FOnTaskCompleted& OnResolveCompleted);
};

