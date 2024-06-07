// Fill out your copyright notice in the Description page of Project Settings.


#include "Sequence/DMSSequenceStep.h"
#include "GameModes/DMSGameModeBase.h"
#include "GameModes/DMSGameStateBase.h"
#include "Notify/DMSNotifyManager.h"
#include "Library/DMSCoreFunctionLibrary.h"

UDMSSequenceStep::UDMSSequenceStep()
{
}

void UDMSSequenceStep::InitializeStepProgress(ADMSSequence* iOwnerSequence,const TSet<TObjectPtr<UDMSSequenceStepDefinition>>& iStepDefinitions, const TArray<FGameplayTag>& ProgressOrder)
{
	CurrentProgressIndex=0;
	OwnerSequence=iOwnerSequence;
	if(iStepDefinitions.Num()==0) return;

	StepDefinitions=iStepDefinitions.Array();
	//ProgressExecutors.Empty();

	for(auto& ProgressTag : ProgressOrder)	{

		TObjectPtr<UDMSSequenceStepDefinition> MatchingDef=nullptr;
		for(auto& Def : StepDefinitions) {
			if(ProgressTag.MatchesTag(Def->GetPureStepTag())){MatchingDef=Def; break;}
		}
		if(!IsValid(MatchingDef)) {
			// Tag to def class 테이블 만들어서 CDO 사용하게?
			continue;
		}
		MatchingDef->GetProgressOps(ProgressTag, ProgressExecutors);
	}
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
	if(ProgressExecutors.Num()==0) { OwnerSequence->OnStepQueueCompleted(true); return; }
	ExecuteNextProgress();
}

void UDMSSequenceStep::RunStepProgressQueue_Alter()
{
	if(StepDefinitions.Num()==0) { OwnerSequence->OnStepQueueCompleted(true); return; }
	ExecuteNextProgress_Alter();
}

void UDMSSequenceStep::ExecuteNextProgress()
{
	auto& CurrentExec = ProgressExecutors[CurrentProgressIndex];
	CurrentExec.ExecutorDelegate.ExecuteIfBound(this);
}

void UDMSSequenceStep::ExecuteNextStep_Alter()
{
	if(IsProgressQueueFinished_Alter()) {OwnerSequence->OnStepQueueCompleted(true); return;}

	const auto& CurrentDef = StepDefinitions[CurrentStepIndex];
	CurrentMainTargetIndex=0;
	CurrentProgressIndex=0;
	MainTargetQueue.Empty();
	if(CurrentDef->bExecuteStepByEachMainTarget)
		MainTargetQueue = OwnerSequence->GetTargets();
	
	ExecuteNextProgress_Alter();
}

void UDMSSequenceStep::ExecuteNextProgress_Alter()
{
	const auto& CurrentDef = StepDefinitions[CurrentStepIndex];
	const auto ProgressOrder = CurrentDef->GetOrderedProgressData();

	if(CurrentProgressIndex >= ProgressOrder.Num())
	{
		if(CurrentDef->bExecuteStepByEachMainTarget && CurrentMainTargetIndex < MainTargetQueue.Num())
		{
			CurrentMainTargetIndex++;
			CurrentProgressIndex=0;
		}
		else
		{
			CurrentStepIndex++;
			ExecuteNextStep_Alter();
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
	CurrentProgressIndex++; ExecuteNextProgress_Alter();

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

void UDMSSequenceStep::ProgressEnd_Alter(bool bSucceeded) 
{
	if(!bSucceeded){OwnerSequence->OnStepQueueCompleted(false); return;}
	CurrentProgressIndex++; ExecuteNextProgress_Alter();
}

void UDMSSequenceStep::SetNextProgress(int ProgressIdx)
{
	CurrentProgressIndex = ProgressIdx-1;
}

void UDMSSequenceStep::SetNextProgress(const FGameplayTag& ProgressTag)
{
	int N = ProgressExecutors.Num();
	for(int i=0;i<N;i++)
	{
		if(ProgressExecutors[i].ExactTag==ProgressTag){
			CurrentProgressIndex=i-1;break;
		}
	}
}

bool UDMSSequenceStep::IsProgressQueueFinished()
{
	return (CurrentProgressIndex+1)==ProgressExecutors.Num();
}

bool UDMSSequenceStep::IsProgressQueueFinished_Alter() const
{
	return CurrentStepIndex>=ProgressExecutors.Num();
}

FGameplayTag UDMSSequenceStep::GetCurrentProgressExactTag() const
{
	return StepDefinitions[CurrentStepIndex]->GetPureStepTag();
	//return ProgressExecutors[CurrentProgressIndex].ExactTag;
}

FGameplayTagContainer UDMSSequenceStep::GetCurrentProgressTags() const
{
	return StepDefinitions[CurrentStepIndex]->GetStepTag(this);
	//FGameplayTagContainer rv = ProgressExecutors[CurrentProgressIndex].ExactTag.GetSingleTagContainer();
	//rv.AppendTags(ProgressExecutors[CurrentProgressIndex].ExecutingStep->GetStepTag(this));
	//return rv;
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

void UDMSSequenceStepDefinition::BroadcastProgress(UDMSSequenceStep* InstancedStep, FName AfterFunctionName, bool bFT)
{
	DMS_LOG_SIMPLE(TEXT("==== %s : Broadcast Step progress [%s] ===="), *InstancedStep->OwnerSequence->GetName(), *GetClass()->GetName());

	auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager(InstancedStep);
	check(NotifyManager);
	//InstancedStep->bFTFlag=bFT;
	FOnTaskCompletedNative AfterBroadcast;
	if( AfterFunctionName.IsNone() ) {
		AfterBroadcast.BindLambda( [=](bool){
			FGameplayTag seqtag = InstancedStep->GetCurrentProgressExactTag();
			DMS_LOG_SIMPLE(TEXT("==== %s : broadcast end lambda [%s] ===="), *InstancedStep->OwnerSequence->GetName(),*seqtag.ToString());
			InstancedStep->ProgressEnd(true);
			DMS_LOG_SIMPLE(TEXT("==== %s : after broadcast end lambda [%s] ===="), *InstancedStep->OwnerSequence->GetName(),*seqtag.ToString());
			});	
	}
	else
	{
		AfterBroadcast.BindLambda( [=](bool){
			//InstancedStep->bFTFlag=0;
			FGameplayTag seqtag = InstancedStep->GetCurrentProgressExactTag();
			DMS_LOG_SIMPLE(TEXT("==== %s : broadcast end lambda [%s] ===="), *InstancedStep->OwnerSequence->GetName(),*seqtag.ToString());
			UDMSSequenceStep* Param = InstancedStep;
			this->ProcessEvent(this->FindFunction(AfterFunctionName),&Param);
			DMS_LOG_SIMPLE(TEXT("==== %s : after broadcast end lambda [%s] ===="), *InstancedStep->OwnerSequence->GetName(),*seqtag.ToString());
		});	
	}
	NotifyManager->Broadcast(InstancedStep->OwnerSequence, AfterBroadcast);

	DMS_LOG_SIMPLE(TEXT("==== %s : Broadcast Step progress end [%s] ===="), *InstancedStep->OwnerSequence->GetName(), *GetClass()->GetName());
}

bool UDMSSequenceStepDefinition::GetProgressOps_Implementation(const FGameplayTag& ProgressTag, TArray<FProgressExecutor>& OutExecutor)
{	
	// If step contains proper progress ops which matching with @ProgressTag. add a binded delegate to @OutExecutors and return true.

	// return false if there's no matching progress ops
	return false;
}
