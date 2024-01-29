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

	DECLARE_DELEGATE_RetVal_OneParam(ADMSActiveEffect*, FEIGetter, UDMSSequence*);
	/**
	 * Structure for implementing chained delegates to ensure synchronization in the Resolve step.
	 */
	struct FResolveDelegateCounter {
		FOnResolveCompleted Delegate;
		uint8 Count=0;
		FResolveIteratingDelegate IteratingDelegate;
		FEIGetter Getter;
		TArray<ADMSActiveEffect*> ApplyingEIs;
		bool bIsPreview;
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
	void ApplyNextEffectInstance(UDMSSequence* SourceSequence, bool PrevSucceeded);
	//void ApplyNextEffectInstance_Preview(UDMSSequence* SourceSequence, bool PrevSucceeded);
	/**
	 * 
	 * @param	Sequence					Resolving sequence.
	 */
	//void InitiateApplyTraversal(UDMSSequence* Sequence, bool ForPreview = false);

public:

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
	TArray<ADMSActiveEffect*>/*EIHandle?*/ CreateEffectInstance(AActor* SourceObject, AActor* SourcePlayer, AActor* Target, UDMSEffectNode* EffectNode, UDMSDataObjectSet* iSet = nullptr);
	
	/**
	 * Creating method for general purpose.
	 * @param	Sequence
	 * @param	EffectNode
	 * @return	Created effect instances.
	 */
	TArray<ADMSActiveEffect*>/*EIHandle?*/ CreateEffectInstance(UDMSSequence* Sequence, UDMSEffectNode* EffectNode);

	/**
	 * Resolve param sequence and executed paramed lambda function when resolve completed.
	 * @param	Sequence					Resolving sequence.
	 * @param	OnResolveCompleted			Lambda executed when resolve completed.
	 */
	template <typename FuncFinished>
	void Resolve(UDMSSequence* Sequence, FuncFinished&& OnResolveCompleted);

};

template <typename FuncFinished>
void UDMSEffectHandler::Resolve(UDMSSequence* Sequence, FuncFinished&& OnResolveCompleted)
{
	//DMS_LOG_SCREEN(TEXT("EH : Resolve %s"), *Sequence->GetName());

	if (Sequence->GetAllEIs().Num() == 0) {
		DMS_LOG_SIMPLE(TEXT("EffectHandler::Resolve : No Resolve Target"));
		goto ResolveSkipped;
	}
	// seperate for logging
	if (Sequence->SequenceState != EDMSSequenceState::SS_Default) {
		DMS_LOG_SIMPLE(TEXT("EffectHandler::Resolve : Sequence is canceled or ignored"));
	ResolveSkipped:
		OnResolveCompleted(true);
		return;
	}


	OnResolveCompletedMap.Add(Sequence);
	OnResolveCompletedMap[Sequence].Delegate.BindLambda(OnResolveCompleted);
	OnResolveCompletedMap[Sequence].Count = 0;
	OnResolveCompletedMap[Sequence].IteratingDelegate.BindUObject(this, &UDMSEffectHandler::ApplyNextEffectInstance);
	OnResolveCompletedMap[Sequence].ApplyingEIs = Sequence->GetAllEIs();

	OnResolveCompletedMap[Sequence].Getter.BindLambda( [this,Sequence](UDMSSequence* SourceSequence){
		return OnResolveCompletedMap[Sequence].ApplyingEIs[OnResolveCompletedMap[SourceSequence].Count++];
	});

	ApplyNextEffectInstance(Sequence, true);
}
