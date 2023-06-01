// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Common/DMSCommons.h"
#include "DMSSequenceStep.generated.h"

/**
 * Smallest unit of a sequence.
 */
UCLASS()
class PROJECTDMSGAME_API UDMSSequenceStep : public UObject
{
	GENERATED_BODY()

public:
	UDMSSequenceStep();
	/**
	 * Current timing of this step.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EDMSTimingFlag Progress;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayTag StepTag;

	/**
	 * Pseudo list
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDMSSequenceStep> NextStep;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDMSSequence> OwnerSequence;

	template<typename Func>
	void InitializeDelegates(Func&& FuncStepInitiated, Func&& StepFinished);

	void RunStep();

	// IMPLEMENTS :: Step Behaviour
	virtual void OnStepInitiated();
	virtual void OnStepFinished();

	void Progress_Before();
	void Progress_During();
	void Progress_After();

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

	DECLARE_MULTICAST_DELEGATE(FOnStepProgressChanged);

	FOnStepProgressChanged OnStepInitiated_Delegate;
	FOnStepProgressChanged OnStepFinished_Delegate;
	
	void ProgressComplete();
};

template<typename Func>
void UDMSSequenceStep::InitializeDelegates(Func&& FuncStepInitiated, Func&& StepFinished)
{
	OnStepInitiated_Delegate.AddLambda(FuncStepInitiated);
	OnStepFinished_Delegate.AddLambda(StepFinished);
}