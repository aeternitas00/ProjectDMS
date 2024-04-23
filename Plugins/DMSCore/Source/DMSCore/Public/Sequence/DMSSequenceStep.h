// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "DMSCoreIncludes.h"
#include "Sequence/DMSSequence.h"
#include "Common/DMSCommons.h"
#include "DMSSequenceStep.generated.h"

/**
 * progress unit of a sequence.
 */
UCLASS(NotBlueprintable, DefaultToInstanced, EditInlineNew)
class DMSCORE_API UDMSSequenceStep : public UObject
{
	GENERATED_BODY()

public:
	UDMSSequenceStep();

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//TObjectPtr<UDMSSequenceStepDefinition> StepDefinition;

	/**
	 *
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<UDMSSequenceStepDefinition>> StepDefinitions;

	/**
	* 
	*/
	TArray<FProgressExecutor> ProgressExecutors;

	/**
	* 
	*/
	int CurrentProgressIndex;

	/**
	 * Reference owner sequence.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<ADMSSequence> OwnerSequence;

	void InitializeStepProgress(ADMSSequence* iOwnerSequence,const TSet<TObjectPtr<UDMSSequenceStepDefinition>>& StepDefinitions, const TArray<FGameplayTag>& ProgressOrder);

	void RunStepProgressQueue();

	void ExecuteNextProgress();

	void SetNextProgress(int ProgressIdx);
	void SetNextProgress(const FGameplayTag& ProgressTag);

	__declspec(noinline) void ProgressEnd(bool bSucceeded = true);

	FORCEINLINE bool IsProgressQueueFinished();

	/**
	 * Initiate step delegates.
	 * @param	StepInitiated			
	 * @param	StepFinished			
	 */
	template<typename FuncInitiated, typename FuncFinished>
	void InitializeDelegates(FuncInitiated&& StepInitiated, FuncFinished&& StepFinished);

	/**
	 * Current timing of this step.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FGameplayTag GetCurrentProgressTag();


	// ======== MIGRATE TO DEF and Progress ======== //

	//UFUNCTION(BlueprintNativeEvent,BlueprintPure)
	//FGameplayTagContainer GetStepTag() const;
	//virtual FGameplayTagContainer GetStepTag_Implementation() const {return FGameplayTagContainer::EmptyContainer;}
	//virtual void OnStepInitiated(){}
	//virtual void OnStepFinished(bool bSucceeded = true){}
	//virtual void Progress_Before(){}
	//virtual void Progress_During(){}
	//virtual void Progress_After(){}
	//void ProgressComplete(bool bSucceeded = true){}
	//// Actual step's behavior, Must call ProgressComplete while progress ending.
	//UFUNCTION(BlueprintNativeEvent)
	//void OnBefore();
	//virtual void OnBefore_Implementation(){}
	//UFUNCTION(BlueprintNativeEvent)
	//void OnDuring();
	//virtual void OnDuring_Implementation(){}
	//UFUNCTION(BlueprintNativeEvent)
	//void OnAfter();
	//virtual void OnAfter_Implementation(){}
	//void RunStep();
	//void CloseStep(bool bSucceeded = true);
	//DECLARE_MULTICAST_DELEGATE(FOnStepInitiated);
	//DECLARE_MULTICAST_DELEGATE_OneParam(FOnStepFinished, bool);
	//FOnStepInitiated OnStepInitiated_Delegate;
	//FOnStepFinished OnStepFinished_Delegate;

	// =================================== //

	friend class ADMSSequence;
};

template<typename FuncInitiated, typename FuncFinished>
void UDMSSequenceStep::InitializeDelegates(FuncInitiated&& StepInitiated, FuncFinished&& StepFinished)
{
	OnStepInitiated_Delegate.AddLambda(StepInitiated);
	OnStepFinished_Delegate.AddLambda(StepFinished);
}

/**
 * progress unit of a sequence.
 */
UCLASS(Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew, Abstract)
class DMSCORE_API UDMSSequenceStepDefinition : public UObject
{
	GENERATED_BODY()

public:
	UDMSSequenceStepDefinition(){}

	UFUNCTION(BlueprintCallable)
	void BroadcastProgress(UDMSSequenceStep* InstancedStep, FName AfterFunctionName);

	UFUNCTION(BlueprintNativeEvent)
	bool GetProgressOps(const FGameplayTag& ProgressTag,UPARAM(ref) TArray<FProgressExecutor>& OutExecutors);
	virtual bool GetProgressOps_Implementation(const FGameplayTag& ProgressTag,TArray<FProgressExecutor>& OutExecutors);

	//UFUNCTION()
	//void Progress_Sample(UDMSSequenceStep* InstancedStep){ /*SOME BEHAVIORS ...*/ InstancedStep->ProgressEnd(false); }

	/**
	 * Get tag of step.
	 */
	UFUNCTION(BlueprintNativeEvent,BlueprintPure)
	FGameplayTag GetPureStepTag() const;
	virtual FGameplayTag GetPureStepTag_Implementation() const {return FGameplayTag::EmptyTag;}

	/**
	 * Get tags of step ( with additional datas ).
	 */
	UFUNCTION(BlueprintNativeEvent,BlueprintPure)
	FGameplayTagContainer GetStepTag() const;
	virtual FGameplayTagContainer GetStepTag_Implementation() const {return FGameplayTagContainer(GetPureStepTag());}

};

