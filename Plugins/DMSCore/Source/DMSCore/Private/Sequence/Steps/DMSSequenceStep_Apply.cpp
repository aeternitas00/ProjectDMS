// Fill out your copyright notice in the Description page of Project Settings.


#include "Sequence/Steps/DMSSequenceStep_Apply.h"
#include "Sequence/DMSSeqManager.h"

#include "Conditions/DMSConditionObject.h"
#include "Notify/DMSNotifyManager.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Effect/DMSEffectDefinition.h"
#include "Effect/DMSEffectHandler.h"

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

	DMS_LOG_SIMPLE(TEXT("==== %s : Step progress During ===="), *GetClass()->GetName());

	DMS_LOG_SIMPLE(TEXT("==== %s : ON BEFORE TIMING RESPONSE ENDED [ Depth : %d ] ===="), *Seq->GetName(), SeqManager->GetDepth(Seq));

	// Proceed to 'During Timing'
	// Resolve 
	DMS_LOG_SIMPLE(TEXT("==== %s : RESOLVE START ===="), *Seq->GetName());

	FOnTaskCompleted OnResolveCompleted;

	OnResolveCompleted.BindDynamic(InstancedStep, &UDMSSequenceStep::ProgressEnd);

	EffectHandler->Resolve(Seq, OnResolveCompleted);
}

void UDMSSequenceStepDefinition_Apply::Progress_After(UDMSSequenceStep* InstancedStep)
{
	DMS_LOG_SIMPLE(TEXT("==== %s : Step progress After ===="), *GetClass()->GetName());
	BroadcastProgress(InstancedStep,FName(NAME_None));
	//BroadcastProgress(InstancedStep,FName("ApplyChildEffect"));
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
	
	return rv;
	//return FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Step.Arkham.Apply"));
}

bool UDMSSequenceStepDefinition_Apply::GetProgressOps_Implementation(const FGameplayTag& ProgressTag, TArray<FProgressExecutor>& OutExecutor)
{
	auto BaseTag = FGameplayTag::RequestGameplayTag("Step.Arkham.Apply");
	if(ProgressTag.MatchesTagExact(BaseTag)){
		OutExecutor.Append({
			{this,FGameplayTag::RequestGameplayTag("Step.Arkham.Apply.Before"),FName(TEXT("Progress_Before"))},
			{this,FGameplayTag::RequestGameplayTag("Step.Arkham.Apply.During"),FName(TEXT("Progress_During"))},
			{this,FGameplayTag::RequestGameplayTag("Step.Arkham.Apply.After"),FName(TEXT("Progress_After"))}
		});	
		return true;
	}
	else if(ProgressTag.MatchesTag(BaseTag)){
		// Alt :: Using native defined tag 
		TArray<FString> Words;	ProgressTag.ToString().ParseIntoArray(Words,TEXT("."));
		FName FunctionName(FString::Printf(TEXT("Progress_%s"), *Words.Last()));
		if ( FindFunction(FunctionName) == nullptr ) return false;

		OutExecutor.Add({this,ProgressTag,FunctionName});		
	}

	return false;
}
