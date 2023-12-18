// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *	"Handling Sequences"
 *
 *	========================================
 */

#include "DMSCoreIncludes.h"
#include "Selector/DMSSelectorQueue.h"
#include "Common/DMSCommons.h"
#include "GameModes/DMSGameModeBase.h"
#include "UObject/NoExportTypes.h"
#include "DMSSeqManager.generated.h"

class UDMSEffectDefinition;
class UDMSEffectNode;
class UDMSDataObjectSet;
class UDMSSequence;
class UDMSEffectorInterface;
class UDMSDecisionWidget;

DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FOnSelectorFinished, UDMSDataObjectSet*, Datas);


/**
 *	========================================
 * 
 *	Managemnet seq. flow. tossing to effect handler.
 * 
 *	========================================
 */
UCLASS(Blueprintable, ClassGroup = (Sequence))
class DMSCORE_API UDMSSeqManager : public UActorComponent // or ActorComponent to attach GM
{
	GENERATED_BODY()

private:

protected:

public:
	UDMSSeqManager();

	/**
	 * Create a Sequence object.
	 * @param	SourceObject				The object that triggers the sequence.
	 * @param	SourcePlayer				Explicit controller of the SourceObject.
	 * @param	EffectNode					Effect that will be applied when the 'sequence is applied'.
	 * @param	Targets						Explicit targets of this sequence. Override EffectNode's preset target flag.
	 * @param	Datas						Additional data for applying effect.
	 * @param	ParentSequence				Explicit parent sequence. Default is Current sequence of SequenceManager.
	 * @return	Created sequence : nullptr if request was failed.
	 */
	UFUNCTION(BlueprintCallable/*, Server*/)
	UDMSSequence* RequestCreateSequence(
		UObject* SourceObject, 
		AActor* SourcePlayer,
		UDMSEffectNode* EffectNode, 
		TArray<TScriptInterface<IDMSEffectorInterface>> Targets,
		UDMSDataObjectSet* Datas = nullptr, 
		UDMSSequence* ParentSequence = nullptr
	);	

	UFUNCTION(BlueprintCallable)
	void RemoveSequence(UDMSSequence* Sequence);

	/**
	 * Run param sequence.
	 * @param	iSeq			Running sequence.
	 * @param	WithPreview		Run sequence with previewing.  
	 */
	UFUNCTION(BlueprintCallable)
	void RunSequence(UDMSSequence* iSeq);

	/**
	 * Complete param sequence. sort of cleanup.
	 * @param	Sequence
	 */
	void CompleteSequence(UDMSSequence* Sequence, bool Successed = true);

protected:

	/**
	 * Clear sequence tree. 
	 * ( acutally it just nullifying root sequence. the rest is left to be handled by the gc. )
	 * @param	Sequence
	 */
	void CleanupSequenceTree();	
	
public:
	/*
	 * Root of sequence tree. the first created sequence becomes root sequence.
	 */
	UPROPERTY()
	TObjectPtr<UDMSSequence> RootSequence;

	/*
	 * Current running sequence.
	 * Sequences created while another sequence is in progress will be set as child of that.
	 */
	UPROPERTY()
	TObjectPtr<UDMSSequence> CurrentSequence;

	/**
	 * Get depth of sequence in tree.
	 * @param	iSeq
	 * @return	Detpth : -1 if iSeq is nullptr.
	 */
	int GetDepth(UDMSSequence* iSeq);

	/*
	 * Delegate for resuming sequence flow from selector queue
	 */
	FOnSelectorFinished OnSelectorFinished;

	/**
	 * Executed when sequence tree is initiated.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnSequenceTreeInitiated();

	/**
	 * Executed when sequence tree is completed.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnSequenceTreeCompleted();

	/**
	 * Starting with a parameter sequence, traversing the sequence tree to a data set containing a given tag as a key.
	 * @param	StartingSequence			Starting node of sequence tree.
	 * @param	SerachingTag				Searching target tag.
	 */
	UFUNCTION(BlueprintCallable)
	UDMSDataObjectSet* SearchNearestDataObject(UDMSSequence* StartingSequence, FGameplayTag SerachingTag) const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	friend class UDMSSequence;
};
