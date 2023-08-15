// Fill out your copyright notice in the Description page of Project Settings.


#include "Sequence/DMSSeqManager.h"
#include "Sequence/DMSSequence.h"
#include "Kismet/GameplayStatics.h"
#include "GameModes/DMSGameMode.h"
#include "Effect/DMSEffectHandler.h"
#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEffectDefinition.h"
#include "Effect/DMSEffectorInterface.h"
#include "Conditions/DMSConditionObject.h"
#include "Notify/DMSNotifyManager.h"
#include "Selector/DMSDecisionWidget.h"
#include "Library/DMSCoreFunctionLibrary.h"



UDMSSeqManager::UDMSSeqManager() : bUsingSteps(false), RootSequence(nullptr), CurrentSequence(nullptr)
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
	Sequence->EIDatas = NewData;
	Sequence->Targets = Targets;
	
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
	
	// ====== Decision Making Step ====== //
	// ( ex. User Choose target, ... )
		
	// TODO :: Migrate to Decision Step 
	//if(!bUsingSteps)
	//{

	//	auto WidgetOwner = iSeq->GetWidgetOwner();

	//	if (WidgetOwner == nullptr) {
	//		//	it's from GAMEMODE or SYSTEM THINGs. LEADER PLAYER own this selector 
	//		//	추후 멀티 플레이 환경에서 어떻게 할 지 생각해보기.
	//	}
	//	TArray<UDMSConfirmWidgetBase*> Widgets;
	//	//if (!iSeq->OriginalEffectNode->bForced && DefaultYNWidget != nullptr)
	//	//	Widgets.Add(NewObject<UDMSConfirmWidgetBase>(this, DefaultYNWidget.Get()));
	//	Widgets.Append(TArray<UDMSConfirmWidgetBase*>(iSeq->OriginalEffectNode->CreateDecisionWidgets(iSeq,WidgetOwner)));
	//	if (!iSeq->SetupWidgetQueue(Widgets))
	//	{
	//		DMS_LOG_SIMPLE(TEXT("Can't setup Decision selector"));
	//		CompleteSequence(iSeq, false);
	//		return;
	//	}

	//	iSeq->RunWidgetQueue(
	//	[&, WidgetOwner](UDMSSequence* pSequence) {
	//		// Decision confirmed or no decision widget

	//		// Create EI first for preview.
	//		UDMSCoreFunctionLibrary::GetDMSEffectHandler()->CreateEffectInstance(pSequence, pSequence->OriginalEffectNode);

	//		// ====== Effect Data Selection Step ====== //
	//		// ( ex. User set value of effect's damage amount , ... )
	//		if (!pSequence->SetupWidgetQueue(TArray<UDMSConfirmWidgetBase*>(pSequence->OriginalEffectNode->CreateSelectors(pSequence, WidgetOwner))))
	//		{
	//			DMS_LOG_SIMPLE(TEXT("Can't setup Effect selector"));
	//			CompleteSequence(pSequence, false);
	//			return;
	//		}

	//		pSequence->RunWidgetQueue(
	//		[this](UDMSSequence* pSequenceN) {
	//			// Selection completed

	//			// Run sequence ( Notifying steps and apply )
	//			DMS_LOG_SIMPLE(TEXT("==== %s : EI Data Selection Completed  ===="),*pSequenceN->GetName());

	//			auto EffectHandler = UDMSCoreFunctionLibrary::GetDMSEffectHandler();
	//		
	//			ApplySequence(pSequenceN);

	//			for (auto EI : pSequenceN->EIs)
	//			{
	//				EI->ChangeEIState(EDMSEIState::EIS_Default);
	//			}
	//		}, 
	//		[this](UDMSSequence* pSequenceN) {
	//			// Selection Canceled
	//			DMS_LOG_SIMPLE(TEXT("Selection Canceled"));
	//			CompleteSequence(pSequenceN, false);
	//			// Cleanup this seq
	//		} // Runwidgetqueue end.
	//		);
	//	}, 

	//	[this](UDMSSequence* pSequence) {
	//		// Decision canceled
	//		DMS_LOG_SIMPLE(TEXT("Decision canceled"));
	//		// Cleanup this seq
	//		CompleteSequence(pSequence, false);
	//		//...
	//	}
	//	);
	//}
	//else
	//{
	//	
	//	CurrentSequence->RunStepQueue();
	//}
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
		if (It->EIDatas->ContainData(SerachingTag)) return It->EIDatas;
		It = It->ParentSequence;
	}
	return nullptr;
}

//void UDMSSeqManager::ApplySequence(UDMSSequence* Sequence)
//{
//	DMS_LOG_SIMPLE(TEXT("==== %s : Apply Sequence [ Depth : %d ] ===="), *Sequence->GetName(), GetDepth(Sequence));
//
//	//DMS_LOG_SCREEN(TEXT("%s : ApplySequence [%s]"), *Sequence->SourceObject->GetName(), 
//	//*Sequence->OriginalEffectNode->GenerateTagContainer().ToString());
//	// Before Notify
//
//	auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager();
//	auto EffectHandler = UDMSCoreFunctionLibrary::GetDMSEffectHandler();
//	check(NotifyManager);
//	check(EffectHandler);
//
//	// TODO :: Migrate to ApplyStep
//
//	//DMS_LOG_SCREEN(TEXT("==-- BEFORE [ Depth : %d ] --=="),GetDepth(Sequence));
//	// 'Before Timing' broadcast starts.
//	Sequence->Progress = EDMSTimingFlag::T_Before;
//	NotifyManager->Broadcast(Sequence, 
//	[=, BeforeSequence=Sequence]() {
//			// ==== ON BEFORE TIMING RESPONSE ENDED ====
//		DMS_LOG_SIMPLE(TEXT("==== %s : ON BEFORE TIMING RESPONSE ENDED [ Depth : %d ] ===="), *BeforeSequence->GetName(), GetDepth(BeforeSequence));
//		// Proceed to 'During Timing'
//		BeforeSequence->Progress = EDMSTimingFlag::T_During;
//
//		auto SeqManager = UDMSCoreFunctionLibrary::GetDMSSequenceManager();
//		//DMS_LOG_SCREEN(TEXT("==-- DURING [ Depth : %d ] --=="), SeqManager->GetDepth(BeforeSequence));
//		
//		// Resolve First
//		DMS_LOG_SIMPLE(TEXT("==== %s : RESOLVE START ===="), *BeforeSequence->GetName());
//
//		EffectHandler->Resolve(BeforeSequence,
//		[=, DuringSequence=BeforeSequence](bool ResolveSuccessed) {
//			// ==== ON RESOLVE COMPLETED ====
//
//			if ( ResolveSuccessed )
//			{
//				DMS_LOG_SIMPLE(TEXT("==== %s : ON RESOLVE COMPLETED [ Depth : %d ] ===="), *DuringSequence->GetName(), SeqManager->GetDepth(DuringSequence));
//				// 'During Timing' broadcast starts.
//				auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager();
//				NotifyManager->Broadcast(BeforeSequence, 
//				[=, DurningSequence = BeforeSequence]() __declspec(noinline) {
//					// ==== ON DURING TIMING RESPONSE ENDED ====
//
//					DMS_LOG_SIMPLE(TEXT("==== %s : ON DURING TIMING RESPONSE ENDED [ Depth : %d ] ===="), *DurningSequence->GetName(), SeqManager->GetDepth(DurningSequence));
//					// Proceed to 'After Timing'
//					DurningSequence->Progress = EDMSTimingFlag::T_After;
//
//					//DMS_LOG_SCREEN(TEXT("==-- AFTER [ Depth : %d ] --=="), SeqManager->GetDepth(DurningSequence));
//
//					// 'After Timing' broadcast starts.
//					NotifyManager->Broadcast(DurningSequence, 
//					[=, AfterSequence = DurningSequence]() __declspec(noinline) {
//							// ==== ON AFTER TIMING RESPONSE ENDED ====
//						DMS_LOG_SIMPLE(TEXT("==== %s : ON AFTER TIMING RESPONSE ENDED [ Depth : %d ] ===="), *AfterSequence->GetName(), SeqManager->GetDepth(AfterSequence));
//					
//						// Run child effect if exist.
//						if (AfterSequence->SequenceState!=EDMSSequenceState::SS_Canceled &&
//							AfterSequence->OriginalEffectNode->ChildEffect != nullptr && AfterSequence->OriginalEffectNode->ChildEffect->GetEffectNode() != nullptr &&
//							AfterSequence->OriginalEffectNode->ChildEffect->GetEffectNode()->Conditions->CheckCondition(AfterSequence->SourceObject, AfterSequence))	{
//
//							// Proceed to run child effect sequence.
//							DMS_LOG_SIMPLE(TEXT("==== %s : OnNotifyReceived -> Advance ===="), *AfterSequence->GetName());
//							auto ChildNode = AfterSequence->OriginalEffectNode->ChildEffect->GetEffectNode();
//							// follows parents data. 
//							auto NewSeq = RequestCreateSequence(AfterSequence->SourceObject, AfterSequence->SourcePlayer, ChildNode,
//								TArray<TScriptInterface<IDMSEffectorInterface>>(), AfterSequence->EIDatas, AfterSequence);
//				
//							// Set delegates when child effect sequence completed.
//							NewSeq->AddToOnSequenceFinished_Native(
//							[=, ParentSequence= AfterSequence](bool Successed) __declspec(noinline) {
//								// ==== ON CHILD EFFECT SEQUENCE COMPLETED ====
//								DMS_LOG_SIMPLE(TEXT("==== %s : ON CHILD EFFECT SEQUENCE COMPLETED [ Depth : %d ] ==== "), *ParentSequence->GetName(), SeqManager->GetDepth(ParentSequence));
//
//								auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager();
//					
//								// Resume parent sequence closing
//								SeqManager->CompleteSequence(ParentSequence);
//							});
//
//							// Run setup child effect sequence.
//							SeqManager->RunSequence(NewSeq);
//						}
//						else //Complete this sequence.
//							SeqManager->CompleteSequence(AfterSequence);
//					});
//				});
//			}
//			else {
//				SeqManager->CompleteSequence(DuringSequence, false);
//			}
//		});
//	});
//
//}

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
