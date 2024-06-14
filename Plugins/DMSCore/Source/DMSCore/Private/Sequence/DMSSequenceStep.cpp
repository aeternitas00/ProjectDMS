// Fill out your copyright notice in the Description page of Project Settings.


#include "Sequence/DMSSequenceStep.h"
#include "GameModes/DMSGameModeBase.h"
#include "GameModes/DMSGameStateBase.h"
#include "Notify/DMSNotifyManager.h"
#include "Library/DMSCoreFunctionLibrary.h"

UDMSSequenceStep::UDMSSequenceStep()
{
}

void UDMSSequenceStep::InitializeStepProgress(ADMSSequence* iOwnerSequence,const TArray<TObjectPtr<UDMSSequenceStepDefinition>>& iStepDefinitions)
{
	CurrentStepIndex=0;
	
	OwnerSequence=iOwnerSequence;
	if(iStepDefinitions.Num()==0) return;

	StepDefinitions=iStepDefinitions;
}

void UDMSSequenceStep::RunStepProgressQueue()
{
	if(StepDefinitions.Num()==0) { OwnerSequence->OnStepQueueCompleted(true); return; }
	ExecuteNextStep();
}

void UDMSSequenceStep::ExecuteNextStep()
{
	if(IsProgressQueueFinished()) {OwnerSequence->OnStepQueueCompleted(true); return;}

	const auto& CurrentDef = StepDefinitions[CurrentStepIndex];
	CurrentMainTargetIndex=0;
	CurrentProgressIndex=0;
	MainTargetQueue.Empty();
	if(CurrentDef->bExecuteStepByEachMainTarget)
		MainTargetQueue = OwnerSequence->GetTargets();
	
	ExecuteNextProgress();
}

void UDMSSequenceStep::ExecuteNextProgress()
{
	const auto& CurrentDef = StepDefinitions[CurrentStepIndex];
	const auto ProgressOrder = CurrentDef->GetOrderedProgressData();

	if(CurrentProgressIndex >= ProgressOrder.Num())
	{
		if(CurrentDef->bExecuteStepByEachMainTarget && CurrentMainTargetIndex+1 < MainTargetQueue.Num())
		{
			CurrentMainTargetIndex++;
			CurrentProgressIndex=0;
		}
		else
		{
			CurrentStepIndex++;
			ExecuteNextStep();
			return;
		}
	}

	const FName& CurrentProgressName = ProgressOrder[CurrentProgressIndex].ProgressFunctionName;

	auto Param=this;
	CurrentDef->ProcessEvent(CurrentDef->FindFunction(CurrentProgressName),&Param);
}

void UDMSSequenceStep::ProgressEnd(bool bSucceeded) 
{
	if(!bSucceeded){OwnerSequence->OnStepQueueCompleted(false); return;}
	CurrentProgressIndex++; ExecuteNextProgress();

	//if (bSucceeded) {
	//	//DMS_LOG_SIMPLE(TEXT("==== %s : ON Progress COMPLETED [ Depth : %d ] ===="), *Seq->GetName(), SeqManager->GetDepth(Seq));
	//}
	//else {
	//	//DMS_LOG_SIMPLE(TEXT("==== %s : ON Progress FAILED [ Depth : %d ] ===="), *Seq->GetName(), SeqManager->GetDepth(Seq));
	//}
	////bFTFlag=0;
	//if(!bSucceeded){OwnerSequence->OnStepQueueCompleted(false); return;}
	//if(IsProgressQueueFinished()) {OwnerSequence->OnStepQueueCompleted(true); return;}
	//CurrentProgressIndex++; ExecuteNextProgress();
	
	//DMS_LOG_SIMPLE(TEXT("==== %s : after progress end ===="),*Seq->GetName());
}

void UDMSSequenceStep::SetNextStep(int ProgressIdx)
{
	CurrentProgressIndex = ProgressIdx-1;
}

void UDMSSequenceStep::SetNextStep(const FGameplayTag& ProgressTag)
{
	int N = StepDefinitions.Num();
	for(int i=0;i<N;i++)
	{
		if(StepDefinitions[i]->GetPureStepTag()==ProgressTag){
			CurrentStepIndex=i-1;break;
		}
	}
}

bool UDMSSequenceStep::IsProgressQueueFinished() const
{
	return CurrentStepIndex >= StepDefinitions.Num();
}

FGameplayTag UDMSSequenceStep::GetCurrentStepTag() const
{
	return StepDefinitions[CurrentStepIndex]->GetPureStepTag();
	//return ProgressExecutors[CurrentProgressIndex].ExactTag;
}

FGameplayTag UDMSSequenceStep::GetCurrentProgressExactTag() const
{
	return GetCurrentProgressData().ProgressTag;
	//return ProgressExecutors[CurrentProgressIndex].ExactTag;
}

FGameplayTagContainer UDMSSequenceStep::GetCurrentProgressTags() const
{
	FGameplayTagContainer rv = StepDefinitions[CurrentStepIndex]->GetStepTag(this);
	rv.AddTag(GetCurrentProgressData().ProgressTag);
	//rv.AppendTags(ProgressExecutors[CurrentProgressIndex].ExecutingStep->GetStepTag(this));
	return rv;
}

FDMSStepProgressMetaData UDMSSequenceStep::GetCurrentProgressData() const
{
	const auto& CurrentDef = StepDefinitions[CurrentStepIndex];
	const auto ProgressDatas = CurrentDef->GetOrderedProgressData();
	return ProgressDatas[CurrentProgressIndex];
}

TScriptInterface<IDMSEffectorInterface> UDMSSequenceStep::GetCurrentMainTarget() const
{
	/* && CurrentMainTargetIndex < MainTargetQueue.Num() */
	return CurrentMainTargetIndex >= 0 ? MainTargetQueue[CurrentMainTargetIndex] : nullptr;
}

void UDMSSequenceStepDefinition::BroadcastProgress(UDMSSequenceStep* InstancedStep, FName AfterFunctionName)
{
	DMS_LOG_SIMPLE(TEXT("==== %s : Broadcast Step progress [%s] ===="), *InstancedStep->OwnerSequence->GetName(), *GetClass()->GetName());

	auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager(InstancedStep);
	check(NotifyManager);
	//InstancedStep->bFTFlag=bFT;
	FOnTaskCompletedNative AfterBroadcast;
	if( AfterFunctionName.IsNone() ) {
		AfterBroadcast.BindLambda( [=](bool){
			FGameplayTagContainer seqtag = InstancedStep->GetCurrentProgressTags();
			DMS_LOG_SIMPLE(TEXT("==== %s : broadcast end lambda [%s] ===="), *InstancedStep->OwnerSequence->GetName(),*seqtag.ToString());
			InstancedStep->ProgressEnd(true);
			DMS_LOG_SIMPLE(TEXT("==== %s : after broadcast end lambda [%s] ===="), *InstancedStep->OwnerSequence->GetName(),*seqtag.ToString());
			});	
	}
	else
	{
		AfterBroadcast.BindLambda( [=](bool){
			//InstancedStep->bFTFlag=0;
			FGameplayTagContainer seqtag = InstancedStep->GetCurrentProgressTags();
			DMS_LOG_SIMPLE(TEXT("==== %s : broadcast end lambda [%s] ===="), *InstancedStep->OwnerSequence->GetName(),*seqtag.ToString());
			UDMSSequenceStep* Param = InstancedStep;
			this->ProcessEvent(this->FindFunction(AfterFunctionName),&Param);
			DMS_LOG_SIMPLE(TEXT("==== %s : after broadcast end lambda [%s] ===="), *InstancedStep->OwnerSequence->GetName(),*seqtag.ToString());
		});	
	}
	NotifyManager->Broadcast(InstancedStep->OwnerSequence, AfterBroadcast);

	DMS_LOG_SIMPLE(TEXT("==== %s : Broadcast Step progress end [%s] ===="), *InstancedStep->OwnerSequence->GetName(), *GetClass()->GetName());
}

