// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "DMSCoreIncludes.h"
#include "Sequence/DMSSequence.h"
#include "Common/DMSCommons.h"
#include "DMSSequenceStep.generated.h"

/**
 * Smallest unit of a sequence.
 */
UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew, Abstract)
class DMSCORE_API UDMSSequenceStep : public UObject
{
	GENERATED_BODY()

public:
	UDMSSequenceStep();

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
	 * @param	bSuccessed				Whether the step was successful or not.
	 */
	void CloseStep(bool bSuccessed = true);

	/**
	 * Get tag of step.
	 */
	UFUNCTION(BlueprintNativeEvent,BlueprintPure)
	FGameplayTag GetStepTag() const;
	virtual FGameplayTag GetStepTag_Implementation() const {return FGameplayTag::EmptyTag;}

	// IMPLEMENTS :: Step Behaviour
	virtual void OnStepInitiated();
	virtual void OnStepFinished(bool bSuccessed = true);

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

	DECLARE_MULTICAST_DELEGATE(FOnStepInitiated);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnStepFinished, bool);

	FOnStepInitiated OnStepInitiated_Delegate;
	FOnStepFinished OnStepFinished_Delegate;
	
	void ProgressComplete(bool bSuccessed = true);

	friend class UDMSSequence;
};

template<typename FuncInitiated, typename FuncFinished>
void UDMSSequenceStep::InitializeDelegates(FuncInitiated&& StepInitiated, FuncFinished&& StepFinished)
{
	OnStepInitiated_Delegate.AddLambda(StepInitiated);
	OnStepFinished_Delegate.AddLambda(StepFinished);
}