// Fill out your copyright notice in the Description page of Project Settings.


#include "Sequence/DMSSequenceStep.h"
#include "GameModes/DMSGameModeBase.h"
#include "GameModes/DMSGameStateBase.h"
#include "Notify/DMSNotifyManager.h"
#include "Library/DMSCoreFunctionLibrary.h"

UDMSSequenceStep::UDMSSequenceStep():bFTFlag(0)
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

void UDMSSequenceStep::RunStepProgressQueue()
{
	if(ProgressExecutors.Num()==0) { OwnerSequence->OnStepQueueCompleted(true); return; }
	ExecuteNextProgress();
}

void UDMSSequenceStep::ExecuteNextProgress()
{
	auto& CurrentExec = ProgressExecutors[CurrentProgressIndex];
	CurrentExec.ExecutorDelegate.ExecuteIfBound(this);
}

void UDMSSequenceStep::SetNextProgress(int ProgressIdx)
{
	CurrentProgressIndex=ProgressIdx-1;
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

void UDMSSequenceStep::ProgressEnd(bool bSucceeded)
{
	if(!bSucceeded){OwnerSequence->OnStepQueueCompleted(false); return;}
	if(IsProgressQueueFinished()) {OwnerSequence->OnStepQueueCompleted(true); return;}
	CurrentProgressIndex++; ExecuteNextProgress();
}

bool UDMSSequenceStep::IsProgressQueueFinished()
{
	return (CurrentProgressIndex+1)==ProgressExecutors.Num();
}

FGameplayTag UDMSSequenceStep::GetCurrentProgressExactTag()
{
	return ProgressExecutors[CurrentProgressIndex].ExactTag;
}

FGameplayTagContainer UDMSSequenceStep::GetCurrentProgressTags()
{
	FGameplayTagContainer rv = ProgressExecutors[CurrentProgressIndex].ExactTag.GetSingleTagContainer();
	rv.AppendTags(ProgressExecutors[CurrentProgressIndex].ExecutingStep->GetStepTag(this));
	return rv;
}

//TArray<UDMSEffectDefinition*> UDMSSequenceStep::GetCurrentResolveContext(ADMSActiveEffect* CurrentAE)
//{
//	return ProgressExecutors[CurrentProgressIndex].ExecutingStep->GetStepResolvingContext(CurrentAE, this);
//}

void UDMSSequenceStepDefinition::BroadcastProgress(UDMSSequenceStep* InstancedStep, FName AfterFunctionName, bool bFT)
{
	DMS_LOG_SIMPLE(TEXT("==== %s : Broadcast Step progress [%s] ===="), *InstancedStep->OwnerSequence->GetName(), *GetClass()->GetName());

	auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager(InstancedStep);
	check(NotifyManager);
	InstancedStep->bFTFlag=bFT;
	FSimpleDelegate AfterBroadcast;
	if( AfterFunctionName.IsNone() ) {
		AfterBroadcast.BindLambda( [=](){ 
			FGameplayTag seqtag = InstancedStep->GetCurrentProgressExactTag();
			InstancedStep->bFTFlag=0;
			DMS_LOG_SIMPLE(TEXT("==== %s : broadcast end lambda [%s] ===="), *InstancedStep->OwnerSequence->GetName(),*seqtag.ToString());
			InstancedStep->ProgressEnd(true);
			DMS_LOG_SIMPLE(TEXT("==== %s : after broadcast end lambda [%s] ===="), *InstancedStep->OwnerSequence->GetName(),*seqtag.ToString());
		});
	}
	else{
		AfterBroadcast.BindLambda( [=](){
			FGameplayTag seqtag = InstancedStep->GetCurrentProgressExactTag();
			InstancedStep->bFTFlag=0;
			DMS_LOG_SIMPLE(TEXT("==== %s : broadcast end lambda [%s] ===="), *InstancedStep->OwnerSequence->GetName(),*seqtag.ToString());
			UDMSSequenceStep* Param = InstancedStep;
			this->ProcessEvent(this->FindFunction(AfterFunctionName),&Param);
			DMS_LOG_SIMPLE(TEXT("==== %s : after broadcast end lambda [%s] ===="), *InstancedStep->OwnerSequence->GetName(),*seqtag.ToString());
		});
	}
	NotifyManager->Broadcast(InstancedStep->OwnerSequence,AfterBroadcast);
	DMS_LOG_SIMPLE(TEXT("==== %s : Broadcast Step progress end [%s] ===="), *InstancedStep->OwnerSequence->GetName(), *GetClass()->GetName());
}

bool UDMSSequenceStepDefinition::GetProgressOps_Implementation(const FGameplayTag& ProgressTag, TArray<FProgressExecutor>& OutExecutor)
{	
	// If step contains proper progress ops which matching with @ProgressTag. add a binded delegate to @OutExecutors and return true.

	// return false if there's no matching progress ops
	return false;
}
