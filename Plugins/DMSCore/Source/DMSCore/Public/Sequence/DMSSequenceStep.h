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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDMSSequenceStepDefinition> StepDefinition;

	/**
	 * Current timing of this step.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EDMSTimingFlag Progress;

	/**
	 * Reference of next step for Pseudo list,
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDMSSequenceStep> NextStep;

	/**
	 * Reference owner sequence.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDMSSequence> OwnerSequence;

	void Initialize(UDMSSequenceStepDefinition* Definition,UDMSSequence* iOwnerSequence);
	/**
	 * Initiate step delegates.
	 * @param	StepInitiated			
	 * @param	StepFinished			
	 */
	template<typename FuncInitiated, typename FuncFinished>
	void InitializeDelegates(FuncInitiated&& StepInitiated, FuncFinished&& StepFinished);

	/**
	 * Run step. will be executed in sequence's step flow control.
	 */
	void RunStep();

	/**
	 * Close step. will be executed in sequence's step flow control.
	 * @param	bSucceeded				Whether the step was successful or not.
	 */
	void CloseStep(bool bSucceeded = true);

	/**
	 * Current timing of this step.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FGameplayTagContainer GetCurrentProgressTag() {return FGameplayTagContainer::EmptyContainer;}

	/**
	 * Get tag of step.
	 */
	UFUNCTION(BlueprintNativeEvent,BlueprintPure)
	FGameplayTagContainer GetStepTag() const;
	virtual FGameplayTagContainer GetStepTag_Implementation() const {return FGameplayTagContainer::EmptyContainer;}

	DECLARE_MULTICAST_DELEGATE(FOnStepInitiated);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnStepFinished, bool);

	FOnStepInitiated OnStepInitiated_Delegate;
	FOnStepFinished OnStepFinished_Delegate;
	
	void ProgressComplete(bool bSucceeded = true);
	void ProgressEnd(bool bSucceeded = true);

	// ======== MIGRATE TO DEF and Progress ======== //
	// 
	// IMPLEMENTS :: Step Behaviour
	virtual void OnStepInitiated();
	virtual void OnStepFinished(bool bSucceeded = true);

	virtual void Progress_Before();
	virtual void Progress_During();
	virtual void Progress_After();

	// Actual step's behavior, Must call ProgressComplete while progress ending.
	UFUNCTION(BlueprintNativeEvent)
	void OnBefore();
	virtual void OnBefore_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void OnDuring();
	virtual void OnDuring_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void OnAfter();
	virtual void OnAfter_Implementation();

	// =================================== //

	friend class UDMSSequence;
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
	FGameplayTagContainer GetStepTag() const;
	virtual FGameplayTagContainer GetStepTag_Implementation() const {return FGameplayTagContainer::EmptyContainer;}

};

