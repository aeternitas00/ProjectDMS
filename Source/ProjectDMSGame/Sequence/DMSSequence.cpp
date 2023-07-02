// Fill out your copyright notice in the Description page of Project Settings.


#include "Sequence/DMSSequence.h"
#include "Sequence/DMSSequenceStep.h"
#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEffectorInterface.h"
#include "Selector/DMSEffectElementSelectorWidget.h"
#include "Player/DMSPlayerState.h"
#include "Gamemodes/DMSGameState.h"


// Default Initializer

UDMSSequence::UDMSSequence() : SourcePlayer(nullptr), SourceObject(nullptr) {

	Progress = EDMSTimingFlag::T_Decision; //EDMSTimingFlag::T_Null;
	SequenceState = EDMSSequenceState::SS_Default;

	ParentSequence = nullptr;
	ChildSequence = nullptr;
	// 
}

UObject* UDMSSequence::GetSourceObject() const
{
	return SourceObject;
}

AActor* UDMSSequence::GetSourcePlayer() const
{
	return SourcePlayer;
}

TArray<TScriptInterface<IDMSEffectorInterface>> UDMSSequence::GetTargets() const
{
	//if (!bIsPreviewSequence)
		return Targets;

	//TArray<TScriptInterface<IDMSEffectorInterface>> rv;

	//for (auto& T : Targets)
	//	rv.Add(T->GetPreviewObject()->_getUObject());
	//return rv;
	
}

bool UDMSSequence::SetSourceObject(UObject* NewSourceObject)
{
	bool rv = SourceObject->Implements<UDMSEffectorInterface>();
	SourceObject = rv ? NewSourceObject : nullptr;
	return rv;
}

bool UDMSSequence::SetSourcePlayer(AActor* NewSourcePlayer)
{
	bool rv = SourceObject->Implements<UDMSEffectorInterface>();
	SourcePlayer= rv ? NewSourcePlayer : nullptr;
	return false;
}


bool UDMSSequence::IsChainableSequence()
{
	return OriginalEffectNode->bIsChainableEffect;
}

void UDMSSequence::AddToOnSequenceInitiated(const FOnSequenceInitiatedDynamic_Signature& iOnSequenceInitiated)
{
	OnSequenceInitiated_Dynamic.Add(iOnSequenceInitiated);
}

void UDMSSequence::AddToOnSequenceFinished(const FOnSequenceFinishedDynamic_Signature& iOnSequenceFinished)
{
	DMS_LOG_SIMPLE(TEXT("==== %s : ADD TO SEQ FINISHIED ===="), *GetName());

	if (OnSequenceFinishedDynamic.IsBound()) {
		DMS_LOG_SIMPLE(TEXT("==== %s : SEQ FINISHIED HAS MUILTIPLE DELEGATES ===="), *GetName());
	}
	OnSequenceFinishedDynamic.Add(iOnSequenceFinished);
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

void UDMSSequence::OnSequenceFinish(bool Successed)
{
	auto temp = std::move(OnSequenceFinished);
	auto temp_Dynamic = std::move(OnSequenceFinishedDynamic);
	OnSequenceFinished = FOnSequenceFinished();
	OnSequenceFinishedDynamic = FOnSequenceFinishedDynamic();
	temp.Broadcast(Successed);
	temp_Dynamic.Broadcast(Successed);
	// Cleanup 
	
}

void UDMSSequence::RedoWidgetQueue()
{
	SelectorQueue.RedoWidgetQueue();
}

APlayerController* UDMSSequence::GetWidgetOwner()
{
	if (SourcePlayer->GetClass()->IsChildOf<ADMSPlayerState>())
		return Cast<ADMSPlayerState>(SourcePlayer)->GetPlayerController();
	
	//if (SourcePlayer->GetClass()->IsChildOf<ADMSGameState>())
	return Cast<ADMSGameState>(SourcePlayer)->GetLeaderPlayerController();

	
}

