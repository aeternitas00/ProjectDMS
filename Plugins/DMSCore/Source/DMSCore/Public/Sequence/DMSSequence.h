// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *	Class for objectifying "Events" that happen in the game.
 *
 *	========================================
 *	objectifying 이 맞는 표현인가?
 */

#include "DMSCoreIncludes.h"
#include "Sequence/DMSSequenceEIStorage.h"
#include "Common/DMSCommons.h"
#include "Common/DMSCommonDelegates.h"
#include "Selector/DMSSelectorQueue.h"
#include "UObject/NoExportTypes.h"
#include "DMSSequence.generated.h"


class UDMSSequenceStep;
class UDMSSequenceStepDefinition;
class ADMSActiveEffect;
class UDMSDataObjectSet;
class UDMSEffectorInterface;
class UDMSEffectNode;
class UDMSConfirmWidgetBase;

UENUM(/*BlueprintType*/)
enum class EDMSSequenceState : uint8
{
	SS_Default UMETA(DisplayName = "Default"),
	SS_Canceled UMETA(DisplayName = "Canceled"),
	SS_Ignored UMETA(DisplayName = "Ignored")
};


DECLARE_DYNAMIC_DELEGATE(FOnSequenceInitiatedDynamic_Signature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSequenceInitiatedDynamic);

DECLARE_DELEGATE_OneParam(FOnSequenceFinished_Signature, bool);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSequenceFinished, bool);

DECLARE_DYNAMIC_DELEGATE_OneParam(FProgressExecutor_Signature, UDMSSequenceStep*, InstancedStep);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnProgressFinished, bool, Succeeded);

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSequenceFinishedDynamic_Signature, bool, Succeeded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSequenceFinishedDynamic,bool,Succeeded);

// move to step instance
USTRUCT(BlueprintType)
struct DMSCORE_API FProgressExecutor
{
	GENERATED_BODY()

	FProgressExecutor(){}
	FProgressExecutor(const FProgressExecutor_Signature& iExecutor,const FGameplayTag& ProgressTag):ExecutorDelegate(iExecutor),ExactTag(ProgressTag){};
	FProgressExecutor(UDMSSequenceStepDefinition* Definition,const FGameplayTag& ProgressTag, const FName& FunctionName);

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FProgressExecutor_Signature ExecutorDelegate;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TObjectPtr<UDMSSequenceStep> ExecutingStep;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FGameplayTag ExactTag;
};
/** 
 * 	========================================
 *
 *	초고 : 일반적으로 게임에서 일어나는 일들이 모두 '이펙트'의 결과이며 이 이펙트가 어떻게 진행 되는지에 대한 정보를 오브젝트화 하는 형태로 설계.
 *	게임 중 어떤 사건이 일어나면 각 게임 오브젝트들이 이 시퀀스 정보를 받아서 각자가 적절한 반응을 할 수 있게 만들기 위함. ( 컨디션 객체가 시퀀스를 받아서 체크하는 형태 )
 *	 
 *	========================================
 */
UCLASS(ClassGroup = (Sequence))
class DMSCORE_API UDMSSequence : public UObject
{
	GENERATED_BODY()

protected:

	// === COMMENT TO REFERENCES OF SOURCE === //
	// Originally, IDMSEffectorInterface should be used.
	// But due to the inconvenience of TScriptInterface, it has been implemented using pointers of UObject-based classes instead.

	/**
	 * The player or actor that triggers the sequence.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, BlueprintGetter = GetSourcePlayer)
	TObjectPtr<AActor> SourcePlayer;
	//TScriptInterface<IDMSEffectorOwnableInterface> SourcePlayer;

	/**
	 * The object that triggers the sequence.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, BlueprintGetter = GetSourceObject)
	TObjectPtr<AActor> SourceObject; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,BlueprintGetter = IsTargetted)
	bool bTargeted;

	/**
	 * Explicit targets of this sequence. Override EffectNode's preset target flag.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FDMSSequenceEIStorage> TargetAndEIs;

public:
	// Default Initializer
	UDMSSequence();

	/**
	 * Current state of this sequence.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EDMSSequenceState SequenceState;

	//UFUNCTION(BlueprintCallable)
	void SetTargetted(const bool& iTargetted) {bTargeted=iTargetted;}

	/**
	 * Effect that will be applied when the 'sequence is applied'.
	 * Sequence : EffectNode = 1 : 1 / Corresponds to the actual meaning of this sequence.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDMSEffectNode> OriginalEffectNode;

	/** 
	 * Data needed for sequence flow, such as 'Damage' by numerical values.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDMSDataObjectSet> SequenceDatas;

	/** 
	 * 서로 체인되어 시퀀스 트리 진행중 GC 되는것을 막아주는 역할을 하게 하는 용도
	 * ++ 상위 시퀀스와 연관된 컨디션, 이펙트 같은 것을 구현 하기 위해
	 */
	UPROPERTY()
	TObjectPtr<UDMSSequence> ParentSequence;
	UPROPERTY()
	TObjectPtr<UDMSSequence> ChildSequence;

	/**
	 * Simple getter of SourceObject
	 */
	UFUNCTION(BlueprintCallable)
	AActor* GetSourceObject() const;

	/**
	 * Simple getter of SourcePlayer
	 */
	UFUNCTION(BlueprintCallable)
	AActor* GetSourcePlayer() const;

	/**
	 * Simple setter of SourceObject
	 * @return	True if NewSourceObject implements IDMSEffectorInterface.
	 */
	UFUNCTION(BlueprintCallable)
	bool SetSourceObject(AActor* NewSourceObject);

	/**
	 * Simple setter of SourcePlayer
	 * @return	true if NewSourcePlayer implements IDMSEffectorInterface.
	 */
	UFUNCTION(BlueprintCallable)
	bool SetSourcePlayer(AActor* NewSourcePlayer);

	/**
	 * Simple getter of Targets.
	 */
	UFUNCTION(BlueprintCallable)
	TArray<TScriptInterface<IDMSEffectorInterface>> GetTargets() const;

	/**
	 * Attach child sequence.
	 * @param	iSeq						Attaching sequence.
	 */
	void AttachChildSequence(UDMSSequence* iSeq);

	/**
	 * Setter of Targets.
	 * @param	iTargets					New targets.
	 */
	UFUNCTION(BlueprintCallable)
	void SetTarget(TArray<TScriptInterface<IDMSEffectorInterface>> iTargets);

	UFUNCTION(BlueprintCallable)
	TArray<FDMSSequenceEIStorage>& GetEIStorage();

	UFUNCTION(BlueprintCallable)
	TArray<ADMSActiveEffect*> GetAllEIs();

	/**
	 * Executed when sequence is initiated.
	 */
	void OnSequenceInitiate();

	/**
	 * Executed when sequence is finished.
	 * @param	Succeeded				Whether the sequence was successful or not.
	 */
	void OnSequenceFinish(bool Succeeded);

public:
	// ======================== //
	//		Sequence query
	// ======================== //
	/**
	* Check sequence is chainable
	*/
	UFUNCTION(BlueprintCallable)
	bool IsChainableSequence();

	UFUNCTION(BlueprintCallable)
	bool IsSequenceActive();

	// DEPRECATED
	//UFUNCTION(BlueprintCallable)
	//EDMSTimingFlag GetCurrentProgress();

	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool IsTargetted() const {return bTargeted;}

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetCurrentProgressTag();

	/**
	 * Generate tag container for context description.
	 */
	UFUNCTION(BlueprintCallable)
	FGameplayTagContainer GetSequenceTags();

	// ================================ //
	//		Delegates and binders.
	// ================================ //
protected:
	FSimpleMulticastDelegate OnSequenceInitiated;
	FOnSequenceFinished OnSequenceFinished;
	FOnSequenceFinished PreSequenceFinished;

	FOnSequenceInitiatedDynamic OnSequenceInitiated_Dynamic;
	FOnSequenceFinishedDynamic OnSequenceFinishedDynamic;

public:
	template<typename FuncInitiated>
	void AddToOnSequenceInitiated_Native(FuncInitiated&& iOnSequenceInitiated);

	template<typename FuncFinished>
	void AddToPreSequenceFinished_Native(FuncFinished&& iOnSequenceFinished);

	template<typename FuncFinished>
	void AddToOnSequenceFinished_Native(FuncFinished&& iOnSequenceFinished);

	UFUNCTION(BlueprintCallable)
	void AddToOnSequenceInitiated(const FOnSequenceInitiatedDynamic_Signature& iOnSequenceInitiated);

	UFUNCTION(BlueprintCallable)
	void AddToOnSequenceFinished(const FOnSequenceFinishedDynamic_Signature& OnSequenceFinished);

	// STEP 
public:
	/**
	* Instancing & initiate steps with effect node.
	* @param	StepClasses				Step classes to instantiate & initialize.
	*/
	void InitializeStepProgress(const TSet<TObjectPtr<UDMSSequenceStepDefinition>>& StepDefinitions,const TArray<FGameplayTag>& ProgressOrder);

	/**
	 * Run registered steps synchronously.
	 */
	void RunStepProgressQueue();

	/**
	 * Executed when step queue completed.
	 * @param	Succeeded				Whether the step queue was successful or not..
	 */
	void OnStepQueueCompleted(bool Succeeded);

protected:	
	/**
	 * Store instanced steps.
	 */
	UPROPERTY()
	TObjectPtr<UDMSSequenceStep> InstancedStep;

private:

public:
	friend class UDMSSequenceStep;
	friend class UDMSSeqManager;
};

template<typename FuncInitiated>
void UDMSSequence::AddToOnSequenceInitiated_Native(FuncInitiated&& iOnSequenceInitiated)
{
	OnSequenceInitiated.AddLambda(iOnSequenceInitiated);
}

template<typename FuncFinished>
void UDMSSequence::AddToOnSequenceFinished_Native(FuncFinished&& iOnSequenceFinished)
{
	DMS_LOG_SIMPLE(TEXT("==== %s : ADD TO SEQ FINISHIED ===="), *GetName());

	if (OnSequenceFinished.IsBound()) {
		DMS_LOG_SIMPLE(TEXT("==== %s : SEQ FINISHIED HAS MUILTIPLE DELEGATES ===="), *GetName());
	}
	OnSequenceFinished.AddLambda(iOnSequenceFinished);

}

template<typename FuncFinished>
void UDMSSequence::AddToPreSequenceFinished_Native(FuncFinished&& iOnSequenceFinished)
{
	DMS_LOG_SIMPLE(TEXT("==== %s : ADD TO Pre SEQ FINISHIED ===="), *GetName());

	if (PreSequenceFinished.IsBound()) {
		DMS_LOG_SIMPLE(TEXT("==== %s : Pre SEQ FINISHIED HAS MUILTIPLE DELEGATES ===="), *GetName());
	}
	PreSequenceFinished.AddLambda(iOnSequenceFinished);

}
