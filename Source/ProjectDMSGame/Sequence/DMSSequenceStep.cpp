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
	OnStepInitiated();
	Progress_Before();
}


void UDMSSequenceStep::OnStepInitiated()
{
	OnStepInitiated_Delegate.Broadcast();
}

void UDMSSequenceStep::OnStepFinished()
{
	OnStepFinished_Delegate.Broadcast();
}

void UDMSSequenceStep::Progress_Before()
{
	auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager();

	check(NotifyManager);

	NotifyManager->Broadcast(OwnerSequence,[this](){OnBefore();});
	
}

void UDMSSequenceStep::Progress_During()
{
	auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager();

	check(NotifyManager);

	NotifyManager->Broadcast(OwnerSequence, [this]() {OnDuring();});
	
}

void UDMSSequenceStep::Progress_After()
{
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

void UDMSSequenceStep::ProgressComplete()
{
	if (Progress == EDMSTimingFlag::T_After) { OnStepFinished();return; }
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

