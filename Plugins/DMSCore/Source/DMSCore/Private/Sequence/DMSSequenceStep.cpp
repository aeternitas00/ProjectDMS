// Fill out your copyright notice in the Description page of Project Settings.


#include "Sequence/DMSSequenceStep.h"

#include "Notify/DMSNotifyManager.h"
#include "Library/DMSCoreFunctionLibrary.h"

UDMSSequenceStep::UDMSSequenceStep()
{
	Progress=EDMSTimingFlag::T_Before;
}

void UDMSSequenceStep::RunStep()
{
	Progress=EDMSTimingFlag::T_Before;
	OnStepInitiated();
}

void UDMSSequenceStep::CloseStep(bool bSucceeded)
{
	OnStepFinished(bSucceeded);
}

void UDMSSequenceStep::OnStepInitiated()
{
	OnStepInitiated_Delegate.Broadcast();
	Progress_Before();
}

void UDMSSequenceStep::OnStepFinished(bool bSucceeded)
{
	OnStepFinished_Delegate.Broadcast(bSucceeded);
	
	// DEBUG
	if ( NextStep != nullptr ) {
		auto NOuter = NextStep->OwnerSequence;
		auto tOuter = this->OwnerSequence;
		if ( NOuter != tOuter )
				DMS_LOG_SIMPLE(TEXT("==== %s : Step Outer is different %s %s===="),NOuter,tOuter);
	}

	if (bSucceeded && NextStep != nullptr)	{ 
		OwnerSequence->CurrentStep = NextStep; NextStep->RunStep(); 
	}
	
	else {
		OwnerSequence->CurrentStep = nullptr;
		OwnerSequence->OnStepQueueCompleted(bSucceeded);
	}
		
}

void UDMSSequenceStep::Progress_Before()
{
	DMS_LOG_SIMPLE(TEXT("==== %s : Step progress Before ===="), *GetClass()->GetName());

	auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager();

	check(NotifyManager);

	NotifyManager->Broadcast(OwnerSequence, [this]() {OnBefore();});
}

void UDMSSequenceStep::Progress_During()
{
	DMS_LOG_SIMPLE(TEXT("==== %s : Step progress During ===="), *GetClass()->GetName());

	auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager();

	check(NotifyManager);

	NotifyManager->Broadcast(OwnerSequence, [this]() {OnDuring();});
}

void UDMSSequenceStep::Progress_After()
{
	DMS_LOG_SIMPLE(TEXT("==== %s : Step progress After ===="), *GetClass()->GetName());

	auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager();

	check(NotifyManager);

	NotifyManager->Broadcast(OwnerSequence, [this]() {OnAfter();});
}

void UDMSSequenceStep::OnBefore_Implementation()
{
	// Behavior
	ProgressComplete();
}

void UDMSSequenceStep::OnDuring_Implementation()
{
	// Behavior
	ProgressComplete();
}

void UDMSSequenceStep::OnAfter_Implementation()
{
	// Behavior
	ProgressComplete();
}

void UDMSSequenceStep::ProgressComplete(bool bSucceeded)
{
	//auto SeqManager = UDMSCoreFunctionLibrary::GetDMSSequenceManager();		check(SeqManager);
	//
	//DEBUG
	if (OwnerSequence == nullptr)
	{
		DMS_LOG_SIMPLE(TEXT("==== %s : SequenceStep has no owner ===="), *GetName());
		return;
	}

	if (!bSucceeded || (OwnerSequence->SequenceState == EDMSSequenceState::SS_Canceled)){
		DMS_LOG_SIMPLE(TEXT("==== %s : [%s] Failed ===="), *OwnerSequence->GetName(), *GetName());
		CloseStep(false); return;
	}

	if (Progress == EDMSTimingFlag::T_After) { CloseStep(); return; }
	Progress = EDMSTimingFlag((uint8)Progress + 1);
	switch (Progress)
	{
		case EDMSTimingFlag::T_During:
			Progress_During();
			break;
		case EDMSTimingFlag::T_After:
			Progress_After();
			break;
		default:
			break;
	}
}

//void UDMSSequenceStep::ExecuteNextProgress(const EDMSTimingFlag& CurrentProgress)
//{
//	ProgressDelegatesMap[CurrentProgress].Broadcast();
//}

