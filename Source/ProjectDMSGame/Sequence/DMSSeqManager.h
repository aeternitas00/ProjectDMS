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

#include "ProjectDMS.h"
#include "Common/DMSCommons.h"
#include "GameModes/DMSGameMode.h"
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
class PROJECTDMSGAME_API UDMSSeqManager : public UActorComponent // or ActorComponent to attach GM
{
	GENERATED_BODY()


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
	UFUNCTION(BlueprintCallable)
	UDMSSequence* RequestCreateSequence(
		UObject* SourceObject, 
		AActor* SourcePlayer,
		UDMSEffectNode* EffectNode, 
		TArray<TScriptInterface<IDMSEffectorInterface>> Targets, 
		UDMSDataObjectSet* Datas = nullptr, 
		UDMSSequence* ParentSequence = nullptr
	);	
	
	/**
	 * Run param sequence.
	 * @param	iSeq
	 */
	UFUNCTION(BlueprintCallable)
	void RunSequence(UDMSSequence* iSeq);

protected:
	/**
	 * Apply param sequence.
	 * @param	Sequence				
	 */
	void ApplySequence(UDMSSequence* Sequence);

	/**
	 * Complete param sequence. sort of cleanup.
	 * @param	Sequence
	 */
	void CompleteSequence(UDMSSequence* Sequence);	

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

	/*
	 * Default YN widget class will be added as final confirmation with preview to the decision step.
	 * TEST
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UDMSDecisionWidget> DefaultYNWidget;


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

	friend class UDMSSequence;
};
