// Copyright JeongWoo Lee


#include "Sequence/DMSSequence.h"
#include "Sequence/DMSSequenceStep.h"
#include "Conditions/DMSConditionObject.h"
#include "Effect/DMSEIManagerComponent.h"
#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEffectDefinition.h"
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
	ActiveChildSequence = nullptr;
	InstancedStep = CreateDefaultSubobject<UDMSSequenceStep>(FName("StepInstance"));
}

FGameplayTag ADMSSequence::GetCurrentStepTag()
{
	return InstancedStep->GetCurrentStepTag();
}

FGameplayTagContainer ADMSSequence::GetCurrentProgressTags()
{
	return InstancedStep->GetCurrentProgressTags();
}

TArray<TScriptInterface<IDMSEffectorInterface>> ADMSSequence::GetCurrentMainTargets()
{
	auto rv = InstancedStep->GetCurrentMainTarget();
	
	return rv ? TArray<TScriptInterface<IDMSEffectorInterface>>{rv} : GetTargets();
}

//TArray<UDMSEffectDefinition*> ADMSSequence::GetCurrentResolveContext(ADMSActiveEffect* CurrentAE)
//{
//	return InstancedStep->GetCurrentResolveContext(CurrentAE);
//}

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
	const auto BFlag = InstancedStep->GetCurrentProgressData().ProgressBroadcastFlag;
	return BFlag != EDMSBroadCastFlag::BF_Unchainable && BFlag != EDMSBroadCastFlag::BF_NoBroadcast;
	
	return OriginalEffectNode->bIsChainableEffect;
}

//void ADMSSequence::InitializeStepProgress(const TSet<TObjectPtr<UDMSSequenceStepDefinition>>& StepDefinitions, const TArray<FGameplayTag>& ProgressOrder)
//{
//	InstancedStep->InitializeStepProgress(this,StepDefinitions,ProgressOrder);
//}

void ADMSSequence::InitializeStepProgress(const TArray<TObjectPtr<UDMSSequenceStepDefinition>>& StepDefinitions)
{
	InstancedStep->InitializeStepProgress(this,StepDefinitions);
}

void ADMSSequence::RunStepProgressQueue()
{
	InstancedStep->RunStepProgressQueue();
}

bool ADMSSequence::IsSequenceActive()
{
	return InstancedStep->IsProgressQueueFinished();
}

FGameplayTagContainer ADMSSequence::GetSequenceTags()
{
	FGameplayTagContainer rv;
	rv.AppendTags(OriginalEffectNode->GenerateTagContainer(this));
	//rv.AddTag(GetCurrentStepTag());
	rv.AppendTags(GetCurrentProgressTags());
	if ( InstancedStep->GetCurrentProgressData().ProgressBroadcastFlag == EDMSBroadCastFlag::BF_FreeActionWindow ) 
		rv.AddTag(FGameplayTag::RequestGameplayTag("Step.Arkham.FreeTrigger"));
	return rv;
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

TArray<ADMSActiveEffect*> ADMSSequence::GetCurrentActiveEffects()
{
	if(InstancedStep->GetCurrentStepDefinition()->bExecuteStepByEachMainTarget){
		for (auto& Storage : TargetAndEIs)
			if ( Storage.MainTarget == InstancedStep->GetCurrentMainTarget() )
				return Storage.EIs;
	}
	return GetAllActiveEffects();
}

void ADMSSequence::AttachChildSequence(ADMSSequence* iSeq) 
{
	if ( ActiveChildSequence != nullptr && ActiveChildSequence->IsSequenceActive() )
	{
		DMS_LOG_DETAIL(Warning,TEXT("==== Attempted to attach a child to sequence that already has children.  ===="))
	}
	//iSeq->ParentSequence = this;
	ActiveChildSequence = iSeq;
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

void ADMSSequence::OnSequenceInitiate()
{
	if(ParentSequence)
		ParentSequence->AttachChildSequence(this);

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

	// Check AE -> CreateApplyingSequence
	//if(OriginalEffectNode->TerminateConditions){
	//	for(auto& EIStorage : TargetAndEIs)		
	//		for(auto& AE : EIStorage.EIs)
	//			AE->ToggleEIState(EDMSAEState::AES_NotifyClosed);
	//}
	
	OnSequenceFinished.Clear();
	OnSequenceFinishedDynamic.Clear();

	MarkAsGarbage();
}

//void ADMSSequence::AddEffectsToChildQueue(TArray<ADMSSequence*>& iChildSequences, const FSimpleDelegate& iOnChildQueueFinished)
//{
//	for(auto& ChildSeq : iChildSequences){
//		ChildSeq->AddToPreSequenceFinished_Native(
//			[=, ParentSequence = this](bool Succeeded){ParentSequence->RunNextQueuedEffect();});
//		ChildEffectQueue.Enqueue(ChildSeq);
//	}
//	OnChildEffectQueueCompleted = iOnChildQueueFinished;
//}
//
//void ADMSSequence::AddEffectsToChildQueue(TArray<UDMSEffectNodeWrapper*>& iChildEffects, const FSimpleDelegate& iOnChildQueueFinished)
//{
//	auto SeqManager = UDMSCoreFunctionLibrary::GetDMSSequenceManager(this);		check(SeqManager);
//	auto NotifyManager	=	UDMSCoreFunctionLibrary::GetDMSNotifyManager(this); 		check(NotifyManager);
//	auto EffectHandler	=	UDMSCoreFunctionLibrary::GetDMSEffectHandler(this);		check(EffectHandler);
//
//	// Run child effect if exist and condition check succeeded.
//	for(auto& lChildEffect : iChildEffects)
//	{
//		if (lChildEffect != nullptr && lChildEffect->GetEffectNode() != nullptr &&
//			lChildEffect->GetEffectNode()->Conditions->CheckCondition(GetSourceObject(), this)) {
//
//			// Proceed to run child effect sequence.
//			auto ChildNode = lChildEffect->GetEffectNode();
//			// follows parents data. 
//			ADMSSequence* NewSeq = SeqManager->RequestCreateSequence(GetSourceObject(), GetSourcePlayer(), ChildNode,
//				TArray<TScriptInterface<IDMSEffectorInterface>>(), true, this);
//
//			// Set delegates when child effect sequence completed.
//			NewSeq->AddToPreSequenceFinished_Native(
//				[=, ParentSequence = this](bool Succeeded) __declspec(noinline) {
//				// ==== ON CHILD EFFECT SEQUENCE COMPLETED ====
//				DMS_LOG_SIMPLE(TEXT("==== %s : ON CHILD EFFECT SEQUENCE COMPLETED [ Depth : %d ] ==== "), *ParentSequence->GetName(), SeqManager->GetDepth(ParentSequence));
//
//				ParentSequence->RunNextQueuedEffect();
//
//				DMS_LOG_SIMPLE(TEXT("==== %s : after activate child effect lambda ends ===="),*ParentSequence->GetName());
//			});
//
//			ChildEffectQueue.Enqueue(NewSeq);
//		}
//	}
//
//	OnChildEffectQueueCompleted = iOnChildQueueFinished;
//}

void ADMSSequence::OnStepQueueCompleted(bool Succeeded)
{
	SequenceState = Succeeded ? EDMSSequenceState::SS_Succeed : EDMSSequenceState::SS_Failed;

	FOnTaskCompletedNative NewOnChildQueueFinished;
	NewOnChildQueueFinished.BindLambda([this,Succeeded](bool){
		auto SeqManager = UDMSCoreFunctionLibrary::GetDMSSequenceManager(this);		check(SeqManager);
		SeqManager->CompleteSequence(this, Succeeded);
	});
	TArray<UDMSEffectNodeWrapper*> ParamChild;

	for(auto& CE : OriginalEffectNode->ChildEffects)ParamChild.Add(CE.Get());

	RunChildEffectQueue(ParamChild,NewOnChildQueueFinished);

	//FSimpleDelegate NewOnChildQueueFinished;
	//NewOnChildQueueFinished.BindLambda([this,Succeeded](){
	//	auto SeqManager = UDMSCoreFunctionLibrary::GetDMSSequenceManager(this);		check(SeqManager);
	//	SeqManager->CompleteSequence(this, Succeeded);
	//});

	//AddEffectsToChildQueue(OriginalEffectNode->ChildEffects, NewOnChildQueueFinished);
	//RunChildEffectQueue();	
}

//void ADMSSequence::RunNextQueuedEffect()
//{	
//	if(ChildEffectQueue.IsEmpty())
//	{
//		auto LocalCopy = OnChildEffectQueueCompleted;
//		OnChildEffectQueueCompleted.Unbind();
//		LocalCopy.ExecuteIfBound();
//	}
//	else
//	{
//		auto SeqManager = UDMSCoreFunctionLibrary::GetDMSSequenceManager(this);		check(SeqManager);
//		auto NewSeq = *ChildEffectQueue.Peek();
//		ChildEffectQueue.Pop();
//	
//		SeqManager->RunSequence(NewSeq);
//	}
//}

void ADMSSequence::RunChildEffectQueue(TArray<UDMSEffectNodeWrapper*>& iChildEffects, const FOnTaskCompletedNative& OnChildQueueCompleted,AActor* SourceTweak, bool AbortOption)
{
	UDMSChildSequenceWorker* ChildSeqWorker = NewObject<UDMSChildSequenceWorker>(this);

	TArray<UObject*> Contexts;
	for(auto& CE : iChildEffects) Contexts.Add(CE);

	ChildSeqWorker->SetupTaskWorkerDelegate_Native(Contexts, OnChildQueueCompleted);
	ChildSeqWorker->SetupChildSequenceWorker(this,SourceTweak);

	ChildSeqWorker->RunTaskWorker(AbortOption);
}

void ADMSSequence::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADMSSequence, EffectManagerComponent);
	DOREPLIFETIME(ADMSSequence, AttributeComponent);
}

void UDMSChildSequenceWorker::Work_Implementation()
{
	auto SeqManager = UDMSCoreFunctionLibrary::GetDMSSequenceManager(this);		check(SeqManager);
	auto CurrentSequenceDefinition = Cast<UDMSEffectNodeWrapper>(GetCurrentContext());
	UDMSEffectNode* ChildEffectNode = CurrentSequenceDefinition ? CurrentSequenceDefinition->GetEffectNode() : nullptr;
	ADMSSequence* NewSeq = nullptr;

	if (ChildEffectNode != nullptr && ChildEffectNode->Conditions->CheckCondition((SourceTweak!=nullptr ? SourceTweak.Get() : ParentSequence->GetSourceObject()), ParentSequence)) {
		NewSeq = SeqManager->RequestCreateSequence((SourceTweak!=nullptr ? SourceTweak.Get() : ParentSequence->GetSourceObject()), ParentSequence->GetSourcePlayer(), ChildEffectNode,
			TArray<TScriptInterface<IDMSEffectorInterface>>(), true, ParentSequence);
	}

	if (NewSeq == nullptr) {CompleteSingleTask(true);return;}

	NewSeq->AddToOnSequenceFinished_Native(
		[=,this](bool Succeeded){CompleteSingleTask(Succeeded);
	});

	SeqManager->RunSequence(NewSeq);
}
