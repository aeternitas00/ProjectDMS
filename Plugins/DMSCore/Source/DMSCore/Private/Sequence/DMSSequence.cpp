// Fill out your copyright notice in the Description page of Project Settings.


#include "Sequence/DMSSequence.h"
#include "Sequence/DMSSequenceStep.h"
#include "Effect/DMSEIManagerComponent.h"
#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEffectorInterface.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Effect/DMSEffectHandler.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "GameFramework/PlayerState.h"
#include "GameModes/DMSGameStateBase.h"


// Default Initializer

ADMSSequence::ADMSSequence() : SourcePlayer(nullptr), SourceObject(nullptr), bTargeted(false) {

	//Progress = EDMSTimingFlag::T_Decision; //EDMSTimingFlag::T_Null;
	SequenceState = EDMSSequenceState::SS_Default;

	AttributeComponent = CreateDefaultSubobject<UDMSAttributeComponent>("AttributesComponent");
	EffectManagerComponent = CreateDefaultSubobject<UDMSEIManagerComponent>("EffectManagerComponent");
	AttributeComponent->SetIsReplicated(true);
	EffectManagerComponent->SetIsReplicated(true);

	ParentSequence = nullptr;
	ChildSequence = nullptr;
	InstancedStep = CreateDefaultSubobject<UDMSSequenceStep>(FName("StepInstance"));
}

FGameplayTag ADMSSequence::GetCurrentProgressTag()
{
	return InstancedStep->GetCurrentProgressTag();
}

AActor* ADMSSequence::GetSourceObject() const
{
	return SourceObject;
}

AActor* ADMSSequence::GetSourcePlayer() const
{
	return SourcePlayer;
}

bool ADMSSequence::SetSourceObject(AActor* NewSourceObject)
{
	bool rv = NewSourceObject->Implements<UDMSEffectorInterface>();
	SourceObject = rv ? NewSourceObject : nullptr;
	return rv;
}

bool ADMSSequence::SetSourcePlayer(AActor* NewSourcePlayer)
{
	bool rv = NewSourcePlayer->Implements<UDMSEffectorInterface>();
	SourcePlayer= rv ? NewSourcePlayer : nullptr;
	return rv;
}


bool ADMSSequence::IsChainableSequence()
{
	return OriginalEffectNode->bIsChainableEffect;
}

void ADMSSequence::AddToOnSequenceInitiated(const FOnSequenceInitiatedDynamic_Signature& iOnSequenceInitiated)
{
	OnSequenceInitiated_Dynamic.Add(iOnSequenceInitiated);
}

void ADMSSequence::AddToOnSequenceFinished(const FOnSequenceFinishedDynamic_Signature& iOnSequenceFinished)
{
	DMS_LOG_SIMPLE(TEXT("==== %s : ADD TO SEQ FINISHIED ===="), *GetName());

	if (OnSequenceFinishedDynamic.IsBound()) {
		DMS_LOG_SIMPLE(TEXT("==== %s : SEQ FINISHIED HAS MUILTIPLE DELEGATES ===="), *GetName());
	}
	OnSequenceFinishedDynamic.Add(iOnSequenceFinished);
}

void ADMSSequence::InitializeStepProgress(const TSet<TObjectPtr<UDMSSequenceStepDefinition>>& StepDefinitions, const TArray<FGameplayTag>& ProgressOrder)
{
	InstancedStep->InitializeStepProgress(this,StepDefinitions,ProgressOrder);
}

void ADMSSequence::RunStepProgressQueue()
{
	InstancedStep->RunStepProgressQueue();
}


FGameplayTagContainer ADMSSequence::GetSequenceTags()
{
	FGameplayTagContainer rv;
	rv.AppendTags(OriginalEffectNode->GenerateTagContainer(this));
	rv.AddTag(GetCurrentProgressTag());
	if(InstancedStep->bFTFlag) rv.AddTag(FGameplayTag::RequestGameplayTag("Step.Arkham.FreeTrigger"));
	return rv;
}

void ADMSSequence::AttachChildSequence(ADMSSequence* iSeq) 
{
	if ( ChildSequence != nullptr && ChildSequence->IsSequenceActive() )
	{
		DMS_LOG_DETAIL(Warning,TEXT("==== Attempted to attach a child to sequence that already has children.  ===="))
	}
	iSeq->ParentSequence = this;
	ChildSequence = iSeq;
}

void ADMSSequence::SetTarget(TArray<TScriptInterface<IDMSEffectorInterface>> iTargets)
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
}

bool ADMSSequence::IsSequenceActive()
{
	return InstancedStep->IsProgressQueueFinished();
}

TArray<TScriptInterface<IDMSEffectorInterface>> ADMSSequence::GetTargets() const
{
	TArray<TScriptInterface<IDMSEffectorInterface>> rv;
	for (auto& Storage : TargetAndEIs) rv.Add(Storage.MainTarget);
	return rv;
}

TArray<FDMSSequenceEIStorage>& ADMSSequence::GetEIStorage()
{
	return TargetAndEIs;
}

TArray<ADMSActiveEffect*> ADMSSequence::GetAllActiveEffects()
{
	TArray<ADMSActiveEffect*> rv;

	for (auto& Storage : TargetAndEIs)
		rv.Append(Storage.EIs);

	return rv;
}



void ADMSSequence::OnSequenceInitiate()
{
	OnSequenceInitiated.Broadcast();
	OnSequenceInitiated_Dynamic.Broadcast();	
	
	OnSequenceInitiated.Clear();
	OnSequenceInitiated_Dynamic.Clear();
}

void ADMSSequence::OnSequenceFinish(bool Succeeded)
{
	//auto temp = OnSequenceFinished;
	//auto temp_Dynamic = OnSequenceFinishedDynamic;
	//OnSequenceFinished.Clear();
	//OnSequenceFinishedDynamic.Clear();
	//temp.Broadcast(Succeeded);
	//temp_Dynamic.Broadcast(Succeeded);
	// Cleanup 

	DMS_LOG_SIMPLE(TEXT("==== %s : Broadcast Pre Sequence Finished ===="), *GetName());
	PreSequenceFinished.Broadcast(Succeeded);

	DMS_LOG_SIMPLE(TEXT("==== %s : Broadcast Sequence Finished ===="), *GetName());
	OnSequenceFinished.Broadcast(Succeeded);
	OnSequenceFinishedDynamic.Broadcast(Succeeded);
	
	OnSequenceFinished.Clear();
	OnSequenceFinishedDynamic.Clear();

	MarkAsGarbage();
}

void ADMSSequence::OnStepQueueCompleted(bool Succeeded)
{
	auto SeqManager = UDMSCoreFunctionLibrary::GetDMSSequenceManager(this);		check(SeqManager);

	SeqManager->CompleteSequence(this, Succeeded); 

	OnSequenceFinish(Succeeded);
}

void ADMSSequence::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADMSSequence, EffectManagerComponent);
	DOREPLIFETIME(ADMSSequence, AttributeComponent);
}

FProgressExecutor::FProgressExecutor(UDMSSequenceStepDefinition* Definition, const FGameplayTag& ProgressTag, const FName& FunctionName) : ExactTag(ProgressTag)
{
	ExecutorDelegate.BindUFunction(Definition, FunctionName); 
}
