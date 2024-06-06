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
#include "Attribute/DMSAttributeComponent.h"
#include "Library/DMSCoreFunctionLibrary.h"



UDMSSeqManager::UDMSSeqManager() : RootSequence(nullptr), CurrentSequence(nullptr)
{
	
}



ADMSSequence* UDMSSeqManager::RequestCreateSequence(
	AActor* SourceObject,
	AActor* SourcePlayer, 
	UDMSEffectNode* EffectNode, 
	const TArray<TScriptInterface<IDMSEffectorInterface>>&  Targets,
	bool LinkAttributeWithParent, 
	ADMSSequence* ParentSequence
)
{
	if( !IsValid(EffectNode) || !IsValid(SourceObject) || !IsValid(SourcePlayer) ){ 
		DMS_LOG_SIMPLE(TEXT("==== Request Create Sequence Failed [Essential params not valid] ===="));
		return nullptr;
	}
	UDMSEffectHandler* EH = UDMSCoreFunctionLibrary::GetDMSEffectHandler(this);	check(EH);

	// Initialize new sequence
	ADMSSequence* Sequence = GetWorld()->SpawnActor<ADMSSequence>();
	Sequence->OriginalEffectNode = EffectNode;
	Sequence->SourceObject = SourceObject;
	Sequence->SourcePlayer = SourcePlayer;
	Sequence->SetTarget(Targets);
	
	Sequence->InitializeStepProgress(EffectNode->StepClassRequirements,EffectNode->ProgressOrder);
	//Sequence->InitializeStepProgress(EffectNode->StepDefinitions,EffectNode->ProgressOrder);

	// Add new seq to seq tree.
	if ( ParentSequence == nullptr ) {
		if ( RootSequence == nullptr ) {
			RootSequence = Sequence;
			//RootSequence->AddToOnSequenceFinished_Native([this](bool){OnSequenceTreeCompleted();UDMSCoreFunctionLibrary::GetDMSGameState(this)->NotifyNeedToCleanup();});
		}
		else if ( CurrentSequence != nullptr )
			Sequence->ParentSequence = CurrentSequence;
			//CurrentSequence->AttachChildSequence(Sequence);
	}
	else {
		Sequence->ParentSequence = ParentSequence;
		//ParentSequence->AttachChildSequence(Sequence);
	}

	if ( LinkAttributeWithParent ) Sequence->AttributeComponent->ParentComponent = Sequence->ParentSequence->AttributeComponent;

	DMS_LOG_SIMPLE(TEXT("==== %s : Request Create Sequence [%s] of [%s] to [%d : %s] ===="),  *SourceObject->GetName(), *Sequence->GetName(), *EffectNode->NodeTags.ToString(), GetDepth(Sequence), Sequence->ParentSequence == nullptr ? TEXT("EmptySequence") : *Sequence->ParentSequence->GetName());

	return Sequence;
}

void UDMSSeqManager::RequestAppendNewSequence_Implementation(
	AActor* SourceObject, 
	AActor* SourcePlayer,
	UDMSEffectNode* EffectNode, 
	const TArray<TScriptInterface<IDMSEffectorInterface>>& Targets,
	ADMSSequence* ParentSequence,
	bool LinkAttributeWithParent
)
{
	RequestCreateSequence(SourceObject,SourcePlayer,EffectNode,Targets,LinkAttributeWithParent,ParentSequence);
}

void UDMSSeqManager::RemoveSequence_Implementation(ADMSSequence* Sequence)
{
	if (Sequence==RootSequence) {
		RootSequence = Sequence->ActiveChildSequence!=nullptr ? Sequence->ActiveChildSequence : nullptr ;
		return;
	}
	
	if (Sequence->ParentSequence != nullptr){
		Sequence->ParentSequence->ActiveChildSequence = Sequence->ActiveChildSequence != nullptr ? Sequence->ActiveChildSequence : nullptr;
	}

	if (Sequence->ActiveChildSequence != nullptr) {
		Sequence->ActiveChildSequence->ParentSequence = Sequence->ParentSequence != nullptr ? Sequence->ParentSequence : nullptr;
	}
}

void UDMSSeqManager::RunSequence_Implementation(ADMSSequence* iSeq)
{
	if(!IsValid(iSeq)){
		DMS_LOG_SIMPLE(TEXT("==== RUN SEQUENCE FAILED [iSeq not vaild] ====")); return;
	}
	DMS_LOG_SIMPLE(TEXT("==== %s : RUN SEQUENCE ===="), *iSeq->GetName());

	UDMSEffectHandler* EH = UDMSCoreFunctionLibrary::GetDMSEffectHandler(this);	check(EH);

	CurrentSequence = iSeq;

	if(CurrentSequence==RootSequence)	
		OnSequenceTreeInitiated();
	
	CurrentSequence->OnSequenceInitiate();

	// 미리 시퀀스 타겟을 지정하지 않았고, 타겟 제너레이터까지 없으면 따로 AE를 만들지 않음. 스텝 진행중에 별도로 타겟을 정하는 과정이 없으면 타겟 미지정인 상태로 시퀀스 진행.
	if(iSeq->OriginalEffectNode->TargetGenerator != nullptr || iSeq->GetTargets().Num() > 0)
		EH->CreateApplyingActiveEffect(iSeq, iSeq->OriginalEffectNode);

	CurrentSequence->RunStepProgressQueue();
}

int UDMSSeqManager::GetDepth(ADMSSequence* iSeq) {
	if (iSeq == nullptr) return -1;
	if (iSeq == RootSequence) return 0;
	return GetDepth(iSeq->ParentSequence)+1;
}

void UDMSSeqManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDMSSeqManager, RootSequence);	
	DOREPLIFETIME(UDMSSeqManager, CurrentSequence);	
}

void UDMSSeqManager::CompleteSequence_Implementation(ADMSSequence* Sequence, bool Succeeded)
{
	if(!IsValid(Sequence)){DMS_LOG_SIMPLE(TEXT("==== Complete Sequence failed [Sequence is not valid] ===="));}

	DMS_LOG_SIMPLE(TEXT("==== %s : SM -> Complete Sequence ===="), *Sequence->GetName());

	if(CurrentSequence != Sequence)
	{
		DMS_LOG_SIMPLE(TEXT("==== %s : SM -> Attempt to close other sequence than current. ===="), *Sequence->GetName());
	}

	if (CurrentSequence == RootSequence) {
		OnSequenceTreeCompleted();
		UDMSCoreFunctionLibrary::GetDMSGameState(this)->NotifyNeedToCleanup();
		CurrentSequence = nullptr;
		RootSequence = nullptr;
	}
	else{
		CurrentSequence = Sequence->ParentSequence;
		CurrentSequence->ActiveChildSequence=nullptr;
	}

	Sequence->OnSequenceFinish(Succeeded);
}


//
//void UDMSSeqManager::CleanupSequenceTree()
//{
//	//...
//	RootSequence = nullptr;
//	
//	UDMSCoreFunctionLibrary::GetDMSEffectHandler(this)->CleanupNonPersistent();
//	//UDMSCoreFunctionLibrary::GetDMSNotifyManager(this)->CleanupNonPersistent();
//	// Else will be GCed.
//	//...
//}
