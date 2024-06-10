// Fill out your copyright notice in the Description page of Project Settings.


#include "Sequence/Steps/DMSSequenceStep_Decision.h"
#include "Sequence/DMSSeqManager.h"
#include "Common/DMSTargetGenerator.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Effect/DMSEffectDefinition.h"
#include "Effect/DMSEffectorOwnableInterface.h"
#include "Effect/DMSEffectHandler.h"
#include "Effect/DMSEffectInstance.h"
#include "Player/DMSPlayerControllerBase.h"
#include "Notify/DMSNotifyManager.h"
#include "GameFramework/PlayerController.h"
#include "GameModes/DMSGameStateBase.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Step_Decision, "Step.Decision");


UDMSSequenceStepDefinition_Decision::UDMSSequenceStepDefinition_Decision()
{
}

void UDMSSequenceStepDefinition_Decision::Progress_Decision(UDMSSequenceStep* InstancedStep)
{
	BroadcastProgress(InstancedStep, FName("MakeDecision"));
}

void UDMSSequenceStepDefinition_Decision::MakeDecision(UDMSSequenceStep* InstancedStep)
{
	auto GS = UDMSCoreFunctionLibrary::GetDMSGameState(InstancedStep); check(GS);
	auto SM = GS->GetSequenceManager(); check(SM);
	auto EH = GS->GetEffectHandler(); check(EH);
	auto SelM = GS->GetSelectorManager(); check(SelM);

	ADMSPlayerControllerBase* WidgetOwner = nullptr;

	auto DecisionMakers = DecisionMaker->GetTargets(InstancedStep->OwnerSequence->GetSourceObject(), InstancedStep->OwnerSequence);

	TArray<ADMSPlayerControllerBase*> CastedDecisionMakers;

	for (auto DM : DecisionMakers)
	{
		if (!DM->Implements<UDMSEffectorInterface>()) continue;
		CastedDecisionMakers.AddUnique(Cast<IDMSEffectorInterface>(DM)->GetOwningPlayerController());
	}

	// TODO :: Implementing for cases where multiple players make selections sequentially.
	WidgetOwner = CastedDecisionMakers.Num() == 0 ? nullptr : CastedDecisionMakers[0];

	TArray<UDMSSelectorRequestForm*> DecisionForms;

	for ( auto& DD : DecisionDefinitions__ )
		DecisionForms.Append(DD->CreateRequestForm(InstancedStep->OwnerSequence));

	if ( WidgetOwner == nullptr ){ InstancedStep->ProgressEnd(false); return;}

	UDMSSelectorWorker* NewWorker = NewObject<UDMSSelectorWorker>(GS);	
	//NewWorker->AddToRoot();
	
	TArray<UObject*> Contexts;

	for(auto Handle : SelM->RequestCreateSelectors(DecisionForms))
	{ 
		if ( !Handle->SetupSelector(WidgetOwner) ) { InstancedStep->ProgressEnd(false); return; }
		Contexts.Add(Handle);
	}

	FOnTaskCompleted OnDecisionsCompleted;
	OnDecisionsCompleted.BindDynamic(InstancedStep,&UDMSSequenceStep::ProgressEnd);
	NewWorker->SetupSelectorWorker(InstancedStep->OwnerSequence, WidgetOwner);
	NewWorker->SetupTaskWorkerDelegate(Contexts, OnDecisionsCompleted);
	NewWorker->RunTaskWorker(true);
}

FGameplayTag UDMSSequenceStepDefinition_Decision::GetPureStepTag_Implementation() const
{
	return FGameplayTag::RequestGameplayTag("Step.Arkham.Decision");
}
FGameplayTagContainer UDMSSequenceStepDefinition_Decision::GetStepTag_Implementation(const UDMSSequenceStep* InstancedStep) const
{
	FGameplayTagContainer rv;
	rv.AddTag(GetPureStepTag());

	for(auto& DD : DecisionDefinitions__)
		rv.AppendTags(DD->GetDecisionTags());
	// Get tags from decision context
	return rv;
}

TArray<FDMSStepProgressMetaData> UDMSSequenceStepDefinition_Decision::GetOrderedProgressData_Implementation() const
{
	return {{"Progress_Decision",BroadcastFlag_Decision,FGameplayTag::RequestGameplayTag("Step.Arkham.Decision")}}; 
}





void UDMSSequenceStepDefinition_TargetSelect::Progress_TargetSelect(UDMSSequenceStep* InstancedStep)
{
	BroadcastProgress(InstancedStep, FName("TargetSelect"));
}

void UDMSSequenceStepDefinition_TargetSelect::TargetSelect(UDMSSequenceStep* InstancedStep)
{
	auto GS = UDMSCoreFunctionLibrary::GetDMSGameState(InstancedStep); check(GS);
	auto SM = GS->GetSequenceManager(); check(SM);
	auto EH = GS->GetEffectHandler(); check(EH);
	auto SelM = GS->GetSelectorManager(); check(SelM);

	ADMSPlayerControllerBase* WidgetOwner = nullptr;

	auto DecisionMakers = DecisionMaker->GetTargets(InstancedStep->OwnerSequence->GetSourceObject(), InstancedStep->OwnerSequence);

	TArray<ADMSPlayerControllerBase*> CastedDecisionMakers;

	for (auto DM : DecisionMakers)
	{
		if (!DM->Implements<UDMSEffectorInterface>()) continue;
		CastedDecisionMakers.AddUnique(Cast<IDMSEffectorInterface>(DM)->GetOwningPlayerController());
	}

	// TODO :: Implementing for cases where multiple players make selections sequentially.
	WidgetOwner = CastedDecisionMakers.Num() == 0 ? nullptr : CastedDecisionMakers[0];

	if ( WidgetOwner == nullptr ){ InstancedStep->ProgressEnd(false); return;}


}

TArray<FDMSStepProgressMetaData> UDMSSequenceStepDefinition_TargetSelect::GetOrderedProgressData_Implementation() const
{
	return {{"Progress_TargetSelect",BroadcastFlag_TargetSelect,FGameplayTag::RequestGameplayTag("Step.Arkham.TargetSelect")}}; 
}

FGameplayTag UDMSSequenceStepDefinition_TargetSelect::GetPureStepTag_Implementation() const
{
	return FGameplayTag::RequestGameplayTag("Step.Arkham.TargetSelect");
}

