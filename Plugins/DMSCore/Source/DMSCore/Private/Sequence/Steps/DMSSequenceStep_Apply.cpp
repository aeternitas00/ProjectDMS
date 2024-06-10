// Fill out your copyright notice in the Description page of Project Settings.


#include "Sequence/Steps/DMSSequenceStep_Apply.h"
#include "Sequence/DMSSeqManager.h"

#include "Conditions/DMSConditionObject.h"
#include "Notify/DMSNotifyManager.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Effect/DMSEffectDefinition.h"
#include "Effect/DMSEffectHandler.h"
#include "Effect/DMSEffectOption.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Step_Apply, "Step.Apply");

UDMSSequenceStepDefinition_Apply::UDMSSequenceStepDefinition_Apply()
{
}

void UDMSSequenceStepDefinition_Apply::Progress_Before(UDMSSequenceStep* InstancedStep)
{
	DMS_LOG_SIMPLE(TEXT("==== %s : Step progress Before ===="), *GetClass()->GetName());
	BroadcastProgress(InstancedStep,FName(NAME_None));
}

void UDMSSequenceStepDefinition_Apply::Progress_During(UDMSSequenceStep* InstancedStep)
{
	auto NotifyManager	=	UDMSCoreFunctionLibrary::GetDMSNotifyManager(InstancedStep);		check(NotifyManager);
	auto EffectHandler	=	UDMSCoreFunctionLibrary::GetDMSEffectHandler(InstancedStep);		check(EffectHandler);
	auto SeqManager		=	UDMSCoreFunctionLibrary::GetDMSSequenceManager(InstancedStep);		check(SeqManager);
	auto Seq = InstancedStep->OwnerSequence;
	auto ActiveEffects = Seq->GetCurrentActiveEffects();
	DMS_LOG_SIMPLE(TEXT("==== %s : Step progress During ===="), *GetClass()->GetName());

	DMS_LOG_SIMPLE(TEXT("==== %s : ON BEFORE TIMING RESPONSE ENDED [ Depth : %d ] ===="), *Seq->GetName(), SeqManager->GetDepth(Seq));

	// Proceed to 'During Timing'
	// Resolve 
	DMS_LOG_SIMPLE(TEXT("==== %s : RESOLVE START ===="), *Seq->GetName());

	FOnTaskCompleted OnResolveCompleted;

	OnResolveCompleted.BindDynamic(InstancedStep, &UDMSSequenceStep::ProgressEnd);

	if(ActiveEffects.Num() == 0 || !Seq->IsTargetted()) {
		DMS_LOG_SIMPLE(TEXT("Resolve : No Resolve Target"));
		OnResolveCompleted.ExecuteIfBound(true);
		return;
	}
	// seperate for logging
	if(Seq->SequenceState != EDMSSequenceState::SS_Default) {
		DMS_LOG_SIMPLE(TEXT("EffectHandler::Resolve : Sequence is canceled or ignored"));
		OnResolveCompleted.ExecuteIfBound(true);
		return;
	}

	UDMSEffectResolveWorker* NewWorker = NewObject<UDMSEffectResolveWorker>(InstancedStep);

	TArray<UObject*> Contexts;

	// TODO :: Only for AE of current MainTarget
	for(auto& AE : ActiveEffects) 
		Contexts.Add(AE);
	NewWorker->SetupTaskWorkerDelegate(Contexts, OnResolveCompleted);	
	NewWorker->SetupResolveWorker(Seq, this);
	NewWorker->RunTaskWorker(true);
}

void UDMSSequenceStepDefinition_Apply::Progress_After(UDMSSequenceStep* InstancedStep)
{
	DMS_LOG_SIMPLE(TEXT("==== %s : Step progress After ===="), *GetClass()->GetName());
	BroadcastProgress(InstancedStep,FName(NAME_None));
}

TArray<FDMSStepProgressMetaData> UDMSSequenceStepDefinition_Apply::GetOrderedProgressData_Implementation() const 
{ 
	return {
		{"Progress_Before",BroadcastFlag_Before,FGameplayTag::RequestGameplayTag("Step.Arkham.Apply.Before")},
		{"Progress_During",EDMSBroadCastFlag::BF_NoBroadcast,FGameplayTag::RequestGameplayTag("Step.Arkham.Apply.During")},
		{"Progress_After",BroadcastFlag_After,FGameplayTag::RequestGameplayTag("Step.Arkham.Apply.After")}
	}; 
}

// Implementations
FGameplayTag UDMSSequenceStepDefinition_Apply::GetPureStepTag_Implementation() const
{
	return FGameplayTag::RequestGameplayTag("Step.Arkham.Apply");
}

FGameplayTagContainer UDMSSequenceStepDefinition_Apply::GetStepTag_Implementation(const UDMSSequenceStep* InstancedStep) const
{
	// combine tag with applying context?
	FGameplayTagContainer rv = GetPureStepTag().GetSingleTagContainer();
	
	for(auto& ED : EffectDefinitions)
		rv.AppendTags(ED->GetEffectTags());

	return rv;
	//return FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Step.Arkham.Apply"));
}

void UDMSEffectResolveWorker::SetupResolveWorker(ADMSSequence* iSequence, UDMSSequenceStepDefinition_Apply* iApplyDefinition)
{
	SourceSequence = iSequence; 
	ApplyDefinition = iApplyDefinition;
	IteratingDelegate.BindDynamic(this, &UDMSEffectResolveWorker::CompleteSingleTask);
}

void UDMSEffectResolveWorker::Work_Implementation()
{
	ADMSActiveEffect* CurrentAE = Cast<ADMSActiveEffect>(GetCurrentContext());

	//CurrentAE->Apply(SourceSequence, IteratingDelegate);

	//DMS_LOG_SCREEN(TEXT("%s : EI Apply [%s]"), *GetName(), *EffectNode->GenerateTagContainer().ToString());

	if (ApplyDefinition->EffectDefinitions.Num() == 0) {
		IteratingDelegate.ExecuteIfBound(true);
		//OnApplyCompleted.ExecuteIfBound(true);
		return;
	}

	TArray<UObject*> ApplyContexts;
	// If any of the Effects is predicted to fail, the sequence is handled as a failure.
	for (auto& CurrentDef : ApplyDefinition->EffectDefinitions)
	{
		if (!CurrentDef->Predict(SourceSequence, CurrentAE)){
			IteratingDelegate.ExecuteIfBound(false);
			//OnApplyCompleted.ExecuteIfBound(false);
			return;
		}
		ApplyContexts.Add(CurrentDef);
	}

	UDMSEffectApplyWorker* NewWorker = NewObject<UDMSEffectApplyWorker>(this);
	NewWorker->SetupApplyWorker(SourceSequence, CurrentAE);
	NewWorker->SetupTaskWorkerDelegate(ApplyContexts, IteratingDelegate);
	NewWorker->RunTaskWorker(true);
}

//void UDMSEffectResolveWorker::OnAllTaskCompleted_Implementation(bool WorkerSucceeded)
//{
//}


void UDMSEffectApplyWorker::SetupApplyWorker(ADMSSequence* iSequence, ADMSActiveEffect* iEI)
{
	SourceSequence = iSequence;
	OwnerInstance = iEI;
	IteratingDelegate.BindDynamic(this, &UDMSEffectApplyWorker::CompleteSingleTask);
	EffectOptionCompleted.BindDynamic(this, &UDMSEffectApplyWorker::OnEffectOptionCompleted);
}

void UDMSEffectApplyWorker::Work_Implementation()
{
	// ====================== //
	//    Effect Canceling    //
	// ====================== // 
	// Have to think about more complicated situations.

	// Check CurrentDef which is a part of EI's effect has to be ignored.
	//FGameplayTagQuery Query;
	//if (SourceSequence->SequenceDatas->ContainData(TAG_DMS_Effect_IgnoreEffect) &&
	//	SourceSequence->SequenceDatas->GetData(TAG_DMS_Effect_IgnoreEffect)->TypeCheck<FGameplayTagQuery>())
	//	Query = SourceSequence->SequenceDatas->GetData(TAG_DMS_Effect_IgnoreEffect)->Get<FGameplayTagQuery>();

	//if (Query.IsEmpty() || !Query.Matches(FGameplayTagContainer(CurrentDef->GetEffectTags()))) {
	//	
	//// Predict check moved to before than setupworkers
	//ExecutedOptionNum = 0;
	//EffectOptionCompleted.Unbind();
	//EffectOptionCompleted.BindDynamic(this, &UDMSEffectApplyWorker::OnEffectOptionCompleted);
	//CurrentDef->ExecuteEffectOptions(SourceSequence, OwnerInstance, EffectOptionCompleted);
	//}

	//else {

	//	// Ignored effect is considered to Succeeded.
	//	ApplyNextEffectDef(true);
	//}

	ExecutedOptionNum = 0;

	CurrentDef = Cast<UDMSEffectDefinition>(GetCurrentContext());
	CurrentDef->ExecuteEffectOptions(SourceSequence, OwnerInstance, EffectOptionCompleted);
	FString DebugStr = CurrentDef->GetEffectTags().ToString();
	DMS_LOG_SIMPLE(TEXT("==== %s : ApplyNextEffectDef closed [%s] ===="),*SourceSequence->GetName(),*DebugStr);
}

void UDMSEffectApplyWorker::OnEffectOptionCompleted(UDMSEffectOption* CompletedOption)
{
	if (CompletedOption!=nullptr && CompletedOption->NextOption != nullptr)
	{
		CompletedOption->NextOption->ExecuteOption(SourceSequence, OwnerInstance, EffectOptionCompleted);
		return;
	}
	if (CurrentDef->EffectOptions.Num() <= ++ExecutedOptionNum)
	{
		CurrentDef->ExecuteEffectDefinition(SourceSequence, OwnerInstance, IteratingDelegate);
	}
}

void UDMSEffectApplyWorker::OnAllTaskCompleted_Implementation(bool WorkerSucceeded)
{
	OwnerInstance->OnApplyComplete();
	//OwnerInstance->CleanupWorker(this);
}