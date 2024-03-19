// Fill out your copyright notice in the Description page of Project Settings.


#include "Sequence/DMSSeqManager.h"
#include "Sequence/DMSSequence.h"
#include "Kismet/GameplayStatics.h"
#include "GameModes/DMSGameModeBase.h"
#include "GameModes/DMSGameStateBase.h"
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
	AActor* SourceObject,
	AActor* SourcePlayer, 
	UDMSEffectNode* EffectNode, 
	const TArray<TScriptInterface<IDMSEffectorInterface>>&  Targets,
	UDMSDataObjectSet* Datas, 
	UDMSSequence* ParentSequence
)
{
	//auto GS = Cast<ADMSGameModeBase>(GetWorld()->GetAuthGameMode())->GetDMSGameState();
	//UDMSEffectHandler* EH = GS->GetEffectHandler();
	UDMSEffectHandler* EH = UDMSCoreFunctionLibrary::GetDMSEffectHandler(this);	check(EH);
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
	Sequence->InitializeStepProgress(EffectNode->StepClassRequirements,EffectNode->ProgressOrder);

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

void UDMSSeqManager::RequestAppendNewSequence_Implementation(
	AActor* SourceObject, 
	AActor* SourcePlayer,
	UDMSEffectNode* EffectNode, 
	const TArray<TScriptInterface<IDMSEffectorInterface>>& Targets,
	UDMSSequence* ParentSequence,
	UDMSDataObjectSet* Datas 
)
{
	RequestCreateSequence(SourceObject,SourcePlayer,EffectNode,Targets,Datas,ParentSequence);
}

void UDMSSeqManager::RemoveSequence_Implementation(UDMSSequence* Sequence)
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

void UDMSSeqManager::RunSequence_Implementation(UDMSSequence* iSeq)
{
	DMS_LOG_SIMPLE(TEXT("==== %s : RUN SEQUENCE ===="), *iSeq->GetName());

	//auto GS = Cast<ADMSGameModeBase>(GetWorld()->GetAuthGameMode())->GetDMSGameState();
	//UDMSEffectHandler* EH = GS->GetEffectHandler();
	UDMSEffectHandler* EH = UDMSCoreFunctionLibrary::GetDMSEffectHandler(this);	check(EH);

	CurrentSequence = iSeq;
	CurrentSequence->OnSequenceInitiate();

	// 미리 시퀀스 타겟을 지정하지 않았고, 타겟 제너레이터까지 없으면 따로 AE를 만들지 않음. 스텝 진행중에 별도로 타겟을 정하는 과정이 없으면 타겟 미지정인 상태로 시퀀스 진행.
	if(iSeq->OriginalEffectNode->TargetGenerator != nullptr || iSeq->GetTargets().Num() > 0)
		EH->CreateApplyingActiveEffect(iSeq, iSeq->OriginalEffectNode);
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

	DOREPLIFETIME(UDMSSeqManager, RootSequence);	
	DOREPLIFETIME(UDMSSeqManager, CurrentSequence);	
}

void UDMSSeqManager::CompleteSequence_Implementation(UDMSSequence* Sequence, bool Succeeded)
{
	DMS_LOG_SIMPLE(TEXT("==== %s : Complete Sequence ===="), *Sequence->GetName());

	// 이렇게 분기 안걸어도 되는데 혹시 몰라서 일단.
	if (CurrentSequence == RootSequence) {
		CurrentSequence = nullptr;
		RootSequence = nullptr;
	}
	else
		CurrentSequence = Sequence->ParentSequence;

	Sequence->OnSequenceFinish(Succeeded);

}



void UDMSSeqManager::CleanupSequenceTree()
{
	//...
	RootSequence = nullptr;
	UDMSCoreFunctionLibrary::GetDMSEffectHandler(this)->CleanupNonPersistent();
	// Else will be GCed.
	//...
}
