// Copyright JeongWoo Lee

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
//class UDMSDataObjectSet;
class ADMSSequence;
class UDMSEffectorInterface;
class UDMSDecisionWidget;

//DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FOnSelectorFinished, UDMSDataObjectSet*, Datas);

USTRUCT(BlueprintType)
struct DMSCORE_API FDMSSequenceSpawnParameters
{
	GENERATED_BODY()

	FDMSSequenceSpawnParameters():SourceObject(nullptr),SourcePlayer(nullptr),Targets({}),ParentSequence(nullptr){}
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TObjectPtr<AActor> SourceObject;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TObjectPtr<AActor> SourcePlayer;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<TScriptInterface<IDMSEffectorInterface>> Targets;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TObjectPtr<ADMSSequence> ParentSequence;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool LinkAttributeWithParent;
};

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
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Request Create Sequence")/*, Server, Reliable*/)
	ADMSSequence* RequestCreateSequence(
		AActor* SourceObject, 
		AActor* SourcePlayer,
		UDMSEffectNode* EffectNode, 
		const TArray<TScriptInterface<IDMSEffectorInterface>>& Targets,
		bool LinkAttributeWithParent = false,
		ADMSSequence* ParentSequence = nullptr
	);

	UFUNCTION(BlueprintCallable, meta=(DisplayName="Request Create Sequence")/*, Server, Reliable*/)
	ADMSSequence* RequestCreateSequence_Param(const FDMSSequenceSpawnParameters SequenceSpawnParameters,UDMSEffectNode* EffectNode);

	UFUNCTION(BlueprintCallable, meta=(DisplayName="Request Create Sequence")/*, Server, Reliable*/)
	ADMSSequence* RequestCreateSequence_SD(const FDMSSequenceSpawnParameters& SequenceSpawnParameters,UDMSSequenceDefinition* SeqDefinition);

	// 멀티플레이 테스트용 임시 함수
	/**
	* Create a Sequence object.
	* @param	SourceObject				The object that triggers the sequence.
	* @param	SourcePlayer				Explicit controller of the SourceObject.
	* @param	EffectNode					Effect that will be applied when the 'sequence is applied'.
	* @param	Targets						Explicit targets of this sequence. Override EffectNode's preset target flag.
	* @param	Datas						Additional data for applying effect.
	* @param	ParentSequence				Explicit parent sequence. Default is Current sequence of SequenceManager.
	*/
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void RequestAppendNewSequence(
		AActor* SourceObject, 
		AActor* SourcePlayer,
		UDMSEffectNode* EffectNode, 
		const TArray<TScriptInterface<IDMSEffectorInterface>>& Targets,
		ADMSSequence* ParentSequence,
		bool LinkAttributeWithParent = false
	);	

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void RemoveSequence(ADMSSequence* Sequence);

	/**
	 * Run param sequence.
	 * @param	iSeq			Running sequence.
	 * @param	WithPreview		Run sequence with previewing.  
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void RunSequence(ADMSSequence* iSeq);

	/**
	 * Complete param sequence. sort of cleanup.
	 * @param	Sequence
	 */
	UFUNCTION(Server, Reliable)
	void CompleteSequence(ADMSSequence* Sequence, bool Succeeded = true);

protected:

	/**
	 * Clear sequence tree. 
	 * ( acutally it just nullifying root sequence. the rest is left to be handled by the gc. )
	 */
	//void CleanupSequenceTree();	
	void RegisterSequence(ADMSSequence* Sequence, ADMSSequence* ExplicitParent = nullptr);

public:
	/*
	 * Root of sequence tree. the first created sequence becomes root sequence.
	 */
	UPROPERTY(Replicated)
	TObjectPtr<ADMSSequence> RootSequence;

	/*
	 * Current running sequence.
	 * Sequences created while another sequence is in progress will be set as child of that.
	 */
	UPROPERTY(Replicated)
	TObjectPtr<ADMSSequence> CurrentSequence;

	/**
	 * Get depth of sequence in tree.
	 * @param	iSeq
	 * @return	Detpth : -1 if iSeq is nullptr.
	 */
	int GetDepth(ADMSSequence* iSeq);

	/*
	 * Delegate for resuming sequence flow from selector queue
	 */
	//FOnSelectorFinished OnSelectorFinished;

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
	//UFUNCTION(BlueprintCallable)
	//UDMSDataObjectSet* SearchNearestDataObject(ADMSSequence* StartingSequence, FGameplayTag SerachingTag) const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	friend class ADMSSequence;
};
