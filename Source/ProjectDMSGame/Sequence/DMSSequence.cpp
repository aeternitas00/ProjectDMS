// Fill out your copyright notice in the Description page of Project Settings.


#include "Sequence/DMSSequence.h"
#include "Sequence/DMSSequenceStep.h"
#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEffectorInterface.h"
#include "Selector/DMSEffectElementSelectorWidget.h"
#include "Player/DMSPlayerState.h"
#include "Gamemodes/DMSGameState.h"

void UDMSSequence::AddToOnSequenceInitiated(const FOnSequenceStateChanged_Signature& iOnSequenceInitiated)
{
	OnSequenceInitiated_Dynamic.Add(iOnSequenceInitiated);
}

void UDMSSequence::AddToOnSequenceFinished(const FOnSequenceStateChanged_Signature& iOnSequenceFinished)
{
	DMS_LOG_SIMPLE(TEXT("==== %s : ADD TO SEQ FINISHIED ===="), *GetName());

	if (OnSequenceFinished_Dynamic.IsBound()) {
		DMS_LOG_SIMPLE(TEXT("==== %s : SEQ FINISHIED HAS MUILTIPLE DELEGATES ===="), *GetName());
	}
	OnSequenceFinished_Dynamic.Add(iOnSequenceFinished);
}

void UDMSSequence::InitializeSteps(const TArray<TSubclassOf<UDMSSequenceStep>>& StepClasses)
{
	CurrentStep=nullptr;
	if (StepClasses.Num()==0) return;
	InstancedSteps.Empty(StepClasses.Num());
	for (auto& StepClass : StepClasses)
		InstancedSteps.Add(NewObject<UDMSSequenceStep>(this,StepClass));

	CurrentStep = InstancedSteps[0];
	for (int i=0 ; i< InstancedSteps.Num()-1;i++)
		InstancedSteps[i]->NextStep = InstancedSteps[i+1];
}

void UDMSSequence::RunStepQueue()
{
	CurrentStep->RunStep();
}

FGameplayTagContainer UDMSSequence::GenerateTagContainer()
{
	FGameplayTagContainer rv;
	rv.AppendTags(OriginalEffectNode->GenerateTagContainer());
	rv.AddTagFast(CurrentStep->StepTag);
	return rv;
}

void UDMSSequence::AttachChildSequence(UDMSSequence* iSeq) 
{
	iSeq->ParentSequence = this;
	ChildSequence = iSeq;
}

bool UDMSSequence::SetupWidgetQueue(TArray<UDMSConfirmWidgetBase*> iWidgets)
{ 
	//for(auto Widget : iWidgets) Widget->SetOwningPlayer(WidgetOwner);
	SelectorQueue.SelectorQueue.Empty();
	SelectorQueue.SelectorQueue.Append(iWidgets); 
	bool rv= SelectorQueue.SetupQueue(this);
	return rv;
}

void UDMSSequence::OnSequenceInitiate()
{
	OnSequenceInitiated.Broadcast();
	OnSequenceInitiated_Dynamic.Broadcast();
}

void UDMSSequence::OnSequenceFinish()
{
	auto temp = std::move(OnSequenceFinished);
	auto temp_Dynamic = std::move(OnSequenceFinished_Dynamic);
	OnSequenceFinished = FSimpleMulticastEventSignature();
	OnSequenceFinished_Dynamic = FOnSequenceStateChanged_Dynamic();
	temp.Broadcast();
	temp_Dynamic.Broadcast();
	// Cleanup 
	
}

APlayerController* UDMSSequence::GetWidgetOwner()
{
	if (SourcePlayer->GetClass()->IsChildOf<ADMSPlayerState>())
		return Cast<ADMSPlayerState>(SourcePlayer)->GetPlayerController();
	
	//if (SourcePlayer->GetClass()->IsChildOf<ADMSGameState>())
	return Cast<ADMSGameState>(SourcePlayer)->GetLeaderPlayerController();

	
}

