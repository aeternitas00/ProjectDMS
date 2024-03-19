// Fill out your copyright notice in the Description page of Project Settings.


#include "Sequence/DMSSequence.h"
#include "Sequence/DMSSequenceStep.h"
#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEffectorInterface.h"
#include "Selector/DMSEffectElementSelectorWidget.h"
#include "Effect/DMSEffectHandler.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "GameFramework/PlayerState.h"
#include "GameModes/DMSGameStateBase.h"


// Default Initializer

UDMSSequence::UDMSSequence() : SourcePlayer(nullptr), SourceObject(nullptr),bTargeted(false) {

	//Progress = EDMSTimingFlag::T_Decision; //EDMSTimingFlag::T_Null;
	SequenceState = EDMSSequenceState::SS_Default;

	ParentSequence = nullptr;
	ChildSequence = nullptr;
	// 
}

EDMSTimingFlag UDMSSequence::GetCurrentProgress() 
{
	return CurrentStep->Progress;
}

AActor* UDMSSequence::GetSourceObject() const
{
	return SourceObject;
}

AActor* UDMSSequence::GetSourcePlayer() const
{
	return SourcePlayer;
}

bool UDMSSequence::SetSourceObject(AActor* NewSourceObject)
{
	bool rv = NewSourceObject->Implements<UDMSEffectorInterface>();
	SourceObject = rv ? NewSourceObject : nullptr;
	return rv;
}

bool UDMSSequence::SetSourcePlayer(AActor* NewSourcePlayer)
{
	bool rv = NewSourcePlayer->Implements<UDMSEffectorInterface>();
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

void UDMSSequence::InitializeSteps(const TArray<TObjectPtr<UDMSSequenceStep>>& OriginalSteps)
{
	CurrentStep=nullptr;
	if (OriginalSteps.Num()==0) return;
	InstancedSteps.Empty(OriginalSteps.Num());

	for (auto& OriginalStep : OriginalSteps) {
		auto InstancedStep = DuplicateObject<UDMSSequenceStep>(OriginalStep, this);
		InstancedStep->OwnerSequence = this;
		InstancedSteps.Add(InstancedStep);
	}

	CurrentStep = InstancedSteps[0];
	for (int i=0 ; i< InstancedSteps.Num()-1;i++)
		InstancedSteps[i]->NextStep = InstancedSteps[i+1];
}

void UDMSSequence::RunStepQueue()
{
	DMS_LOG_SIMPLE(TEXT("==== %s : RUN Step Queue ===="), *GetName());
	if (CurrentStep==nullptr) 
	{
		DMS_LOG_SIMPLE(TEXT("==== %s : Current Step was nullptr ===="), *GetName());
		OnStepQueueCompleted(false); return;
	}
	CurrentStep->RunStep();
}

void UDMSSequence::InitializeStepProgress(const TSet<TObjectPtr<UDMSSequenceStepDefinition>>& StepDefinitions, const TArray<FGameplayTag>& ProgressOrder)
{
	CurrentProgressIndex=0;
	if (StepDefinitions.Num()==0) return;
	InstancedSteps.Empty(StepDefinitions.Num());
	ProgressExecutors.Empty();

	for (auto& OriginalStep : StepDefinitions) {
		auto InstancedStep = NewObject<UDMSSequenceStep>(this);
		InstancedStep->Initialize(OriginalStep,this);
		InstancedSteps.Add(InstancedStep);
	}
	
	for (auto& ProgressTag : ProgressOrder)	{	
		for(auto& InstancedStep : InstancedSteps){
			TArray<FProgressExecutor> tExecutors;
			if(InstancedStep->StepDefinition->GetProgressOps_Implementation(ProgressTag,tExecutors)) {
				for (auto& tExec : tExecutors ) tExec.ExecutingStep = InstancedStep;
				ProgressExecutors.Append(tExecutors); break;
			}
		}
	}
}

void UDMSSequence::ProgressEnd(bool Succeeded)
{
	if(!Succeeded){OnStepQueueCompleted(false); return;}
	if(CurrentProgressIndex==ProgressExecutors.Num()) {OnStepQueueCompleted(true); return;}
	ExecuteNextProgress();
}

void UDMSSequence::RunStepProgressQueue()
{
	if (ProgressExecutors.Num()==0) { OnStepQueueCompleted(false); return; }
	ExecuteNextProgress();
}

void UDMSSequence::ExecuteNextProgress()
{
	auto& CurrentExec = ProgressExecutors[CurrentProgressIndex];
	CurrentExec.ExecutorDelegate.ExecuteIfBound(CurrentExec.ExecutingStep);
	CurrentProgressIndex++;
}

FGameplayTagContainer UDMSSequence::GenerateTagContainer()
{
	FGameplayTagContainer rv;
	rv.AppendTags(OriginalEffectNode->GenerateTagContainer(this));
	rv.AppendTags(CurrentStep->GetStepTag());
	return rv;
}

void UDMSSequence::AttachChildSequence(UDMSSequence* iSeq) 
{
	if ( ChildSequence != nullptr && ChildSequence->IsSequenceActive() )
	{
		DMS_LOG_DETAIL(Warning,TEXT("==== Attempted to attach a child to sequence that already has children.  ===="))
	}
	iSeq->ParentSequence = this;
	ChildSequence = iSeq;
}

void UDMSSequence::SetTarget(TArray<TScriptInterface<IDMSEffectorInterface>> iTargets)
{
	for ( auto& TargetToEI : TargetAndEIs )
	{ 
		for (auto& EI : TargetToEI.EIs) EI->DetachFromOwner();
	}
	TargetAndEIs.Reset();

	for (auto& Target : iTargets) 
	{
		if ( Target.GetInterface() == nullptr ) {
			DMS_LOG_SIMPLE(TEXT("Sequence::SetTarget _ iTarget Is Not Effector [Name : %s]"),*Target.GetObject()->GetName());
			continue;
		}
		TargetAndEIs.Add(FDMSSequenceEIStorage(Target));
	}

	//if (CreateEIs)
	//{
	//	auto EH = UDMSCoreFunctionLibrary::GetDMSEffectHandler();	check(EH);
	//	EH->CreateApplyingActiveEffect(this,OriginalEffectNode);
	//}
}

bool UDMSSequence::IsSequenceActive()
{
	return CurrentStep != nullptr;
}

TArray<TScriptInterface<IDMSEffectorInterface>> UDMSSequence::GetTargets() const
{
	TArray<TScriptInterface<IDMSEffectorInterface>> rv;
	for (auto& Storage : TargetAndEIs) rv.Add(Storage.MainTarget);
	return rv;
}

TArray<FDMSSequenceEIStorage>& UDMSSequence::GetEIStorage()
{
	return TargetAndEIs;
}

TArray<ADMSActiveEffect*> UDMSSequence::GetAllEIs()
{
	TArray<ADMSActiveEffect*> rv;

	for (auto& Storage : TargetAndEIs)
		rv.Append(Storage.EIs);

	return rv;
}



void UDMSSequence::OnSequenceInitiate()
{
	OnSequenceInitiated.Broadcast();
	OnSequenceInitiated_Dynamic.Broadcast();	
	
	OnSequenceInitiated.Clear();
	OnSequenceInitiated_Dynamic.Clear();
}

void UDMSSequence::OnSequenceFinish(bool Succeeded)
{
	//auto temp = OnSequenceFinished;
	//auto temp_Dynamic = OnSequenceFinishedDynamic;
	//OnSequenceFinished.Clear();
	//OnSequenceFinishedDynamic.Clear();
	//temp.Broadcast(Succeeded);
	//temp_Dynamic.Broadcast(Succeeded);
	// Cleanup 
	OnSequenceFinished.Broadcast(Succeeded);
	OnSequenceFinishedDynamic.Broadcast(Succeeded);
	
	OnSequenceFinished.Clear();
	OnSequenceFinishedDynamic.Clear();
}

void UDMSSequence::OnStepQueueCompleted(bool Succeeded)
{
	auto SeqManager = UDMSCoreFunctionLibrary::GetDMSSequenceManager(this);		check(SeqManager);

	SeqManager->CompleteSequence(this, Succeeded); 
}

//bool UDMSSequence::SetupWidgetQueue(TArray<UDMSConfirmWidgetBase*> iWidgets)
//{ 
//	//for(auto Widget : iWidgets) Widget->SetOwningPlayer(WidgetOwner);
//	SelectorQueue.SelectorQueue.Empty();
//	SelectorQueue.SelectorQueue.Append(iWidgets); 
//	bool rv= SelectorQueue.SetupQueue(this);
//	return rv;
//}
// 
//void UDMSSequence::RedoWidgetQueue()
//{
//	SelectorQueue.RedoWidgetQueue();
//}
//
//APlayerController* UDMSSequence::GetWidgetOwner()
//{
//	if (SourcePlayer->GetClass()->IsChildOf<APlayerState>())
//		return Cast<APlayerState>(SourcePlayer)->GetPlayerController();
//	
//	//if (SourcePlayer->GetClass()->IsChildOf<ADMSGameState>())
//	return Cast<ADMSGameStateBase>(SourcePlayer)->GetLeaderPlayerController();
//}

