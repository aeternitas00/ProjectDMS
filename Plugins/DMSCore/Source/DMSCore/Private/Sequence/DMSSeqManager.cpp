// Fill out your copyright notice in the Description page of Project Settings.


#include "Sequence/DMSSeqManager.h"
#include "Sequence/DMSSequence.h"
#include "Kismet/GameplayStatics.h"
#include "GameModes/DMSGameModeBase.h"
#include "Effect/DMSEffectHandler.h"
#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEffectDefinition.h"
#include "Effect/DMSEffectorInterface.h"
#include "Conditions/DMSConditionObject.h"
#include "Notify/DMSNotifyManager.h"
#include "Library/DMSCoreFunctionLibrary.h"



UDMSSeqManager::UDMSSeqManager() : /*bUsingSteps(false),*/ RootSequence(nullptr), CurrentSequence(nullptr)
{
	
}



UDMSSequence* UDMSSeqManager::RequestCreateSequence(
	UObject* SourceObject,
	AActor* SourcePlayer, 
	UDMSEffectNode* EffectNode, 
	TArray<TScriptInterface<IDMSEffectorInterface>> Targets,
	UDMSDataObjectSet* Datas, 
	UDMSSequence* ParentSequence
)
{
	UDMSEffectHandler* EH = UDMSCoreFunctionLibrary::GetDMSEffectHandler();	check(EH);
	//if (EH==nullptr) { return nullptr; }

	// Initialize new sequence
	UDMSSequence* Sequence = NewObject<UDMSSequence>(this/*, FName(*SeqName)*/);
	UDMSDataObjectSet* NewData = NewObject<UDMSDataObjectSet>(Sequence);
	NewData->Inherit(Datas);
	Sequence->OriginalEffectNode = EffectNode;
	Sequence->SourceObject = SourceObject;
	Sequence->SourcePlayer = SourcePlayer;
	Sequence->SequenceDatas = NewData;
	Sequence->SetTarget(Targets);
	
	// Test Feature
	Sequence->InitializeSteps(EffectNode->StepRequirements);

	// Add new seq to seq tree.
	if (ParentSequence == nullptr) {
		if (RootSequence == nullptr) {
			RootSequence = Sequence;

			RootSequence->OnSequenceInitiated.AddUObject(this, &UDMSSeqManager::OnSequenceTreeInitiated);
			RootSequence->OnSequenceFinished.AddLambda([this](bool){OnSequenceTreeCompleted();});
			RootSequence->OnSequenceFinished.AddLambda([this](bool){CleanupSequenceTree();});
		}
		else if (CurrentSequence != nullptr)
			CurrentSequence->AttachChildSequence(Sequence);
	}
	else {
		ParentSequence->AttachChildSequence(Sequence);
	}

	DMS_LOG_SIMPLE(TEXT("==== %s : Request Create Sequence [%s] of [%s] to [%d : %s] ===="),  *SourceObject->GetName(), *Sequence->GetName(), *EffectNode->NodeTag.ToString(), GetDepth(Sequence), Sequence->ParentSequence == nullptr ? TEXT("EmptySequence") : *Sequence->ParentSequence->GetName());

	return Sequence;
}

void UDMSSeqManager::RemoveSequence(UDMSSequence* Sequence)
{
	if (Sequence==RootSequence) {
		RootSequence = Sequence->ChildSequence!=nullptr ? Sequence->ChildSequence : nullptr ;
		return;
	}
	
	if (Sequence->ParentSequence != nullptr){
		Sequence->ParentSequence->ChildSequence = Sequence->ChildSequence != nullptr ? Sequence->ChildSequence : nullptr;
	}

	if (Sequence->ChildSequence != nullptr) {
		Sequence->ChildSequence->ParentSequence = Sequence->ParentSequence != nullptr ? Sequence->ParentSequence : nullptr;
	}
}

void UDMSSeqManager::RunSequence(UDMSSequence* iSeq)
{
	DMS_LOG_SIMPLE(TEXT("==== %s : RUN SEQUENCE ===="), *iSeq->GetName());

	UDMSEffectHandler* EH = UDMSCoreFunctionLibrary::GetDMSEffectHandler();	check(EH);

	CurrentSequence = iSeq;
	CurrentSequence->OnSequenceInitiate();

	EH->CreateEffectInstance(iSeq, iSeq->OriginalEffectNode);
	CurrentSequence->RunStepQueue();
}

int UDMSSeqManager::GetDepth(UDMSSequence* iSeq) {
	if (iSeq == nullptr) return -1;
	if (iSeq == RootSequence) return 0;
	return GetDepth(iSeq->ParentSequence)+1;
}

UDMSDataObjectSet* UDMSSeqManager::SearchNearestDataObject(UDMSSequence* StartingSequence, FGameplayTag SerachingTag) const
{
	UDMSSequence* It= StartingSequence;
	while (It!=nullptr)
	{
		if (It->SequenceDatas->ContainData(SerachingTag)) return It->SequenceDatas;
		It = It->ParentSequence;
	}
	return nullptr;
}

void UDMSSeqManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(UDMSSeqManager, TypeWidgetMap);	
}

void UDMSSeqManager::CompleteSequence(UDMSSequence* Sequence, bool Successed)
{
	DMS_LOG_SIMPLE(TEXT("==== %s : Complete Sequence ===="), *Sequence->GetName());

	// 이렇게 분기 안걸어도 되는데 혹시 몰라서 일단.
	if (CurrentSequence == RootSequence) {
		CurrentSequence = nullptr;
		RootSequence = nullptr;
	}
	else
		CurrentSequence = Sequence->ParentSequence;

	Sequence->OnSequenceFinish(Successed);

}


void UDMSSeqManager::CleanupSequenceTree()
{
	//...
	RootSequence = nullptr;
	UDMSCoreFunctionLibrary::GetDMSEffectHandler()->CleanupNonPersistent();
	// Else will be GCed.
	//...
}
