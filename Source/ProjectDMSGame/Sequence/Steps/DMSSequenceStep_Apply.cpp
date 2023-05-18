// Fill out your copyright notice in the Description page of Project Settings.


#include "Sequence/Steps/DMSSequenceStep_Apply.h"

void UDMSSequenceStep_Apply::OnBefore_Implementation()
{
	//DMS_LOG_SIMPLE(TEXT("==== %s : Apply Sequence [ Depth : %d ] ===="), *Sequence->GetName(), GetDepth(Sequence));

	//DMS_LOG_SCREEN(TEXT("%s : ApplySequence [%s]"), *Sequence->SourceObject->GetName(),
	//	*Sequence->OriginalEffectNode->GenerateTagContainer().ToString());
	//// Before Notify

	//auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager();
	//auto EffectHandler = UDMSCoreFunctionLibrary::GetDMSEffectHandler();
	//check(NotifyManager);
	//check(EffectHandler);

	////Sequence->SetActive(true);

	//// TODO :: Migrate to ApplyStep

	//DMS_LOG_SCREEN(TEXT("==-- BEFORE [ Depth : %d ] --=="), GetDepth(Sequence));
	//// 'Before Timing' broadcast starts.
	//Sequence->Progress = EDMSTimingFlag::T_Before;
	//NotifyManager->BroadCast(Sequence,
	//	[=, BeforeSequence = Sequence]() {
	//		// ==== ON BEFORE TIMING RESPONSE ENDED ====
	//		DMS_LOG_SIMPLE(TEXT("==== %s : ON BEFORE TIMING RESPONSE ENDED [ Depth : %d ] ===="), *BeforeSequence->GetName(), GetDepth(BeforeSequence));
	//		// Proceed to 'During Timing'
	//		BeforeSequence->Progress = EDMSTimingFlag::T_During;

	//		auto SeqManager = UDMSCoreFunctionLibrary::GetDMSSequenceManager();
	//		DMS_LOG_SCREEN(TEXT("==-- DURING [ Depth : %d ] --=="), SeqManager->GetDepth(BeforeSequence));

	//		// Resolve First
	//		DMS_LOG_SIMPLE(TEXT("==== %s : RESOLVE START ===="), *BeforeSequence->GetName());

	//		EffectHandler->Resolve(BeforeSequence,
	//			[=, DuringSequence = BeforeSequence]() {
	//				// ==== ON RESOLVE COMPLETED ====

	//				DMS_LOG_SIMPLE(TEXT("==== %s : ON RESOLVE COMPLETED [ Depth : %d ] ===="), *DuringSequence->GetName(), SeqManager->GetDepth(DuringSequence));
	//				// 'During Timing' broadcast starts.
	//				auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager();
	//				NotifyManager->BroadCast(BeforeSequence,
	//					[=, DurningSequence = BeforeSequence]() __declspec(noinline) {
	//					// ==== ON DURING TIMING RESPONSE ENDED ====
	//					DMS_LOG_SIMPLE(TEXT("==== %s : ON DURING TIMING RESPONSE ENDED [ Depth : %d ] ===="), *DurningSequence->GetName(), SeqManager->GetDepth(DurningSequence));
	//					// Proceed to 'After Timing'
	//					DurningSequence->Progress = EDMSTimingFlag::T_After;

	//					DMS_LOG_SCREEN(TEXT("==-- AFTER [ Depth : %d ] --=="), SeqManager->GetDepth(DurningSequence));

	//					// 'After Timing' broadcast starts.
	//					NotifyManager->BroadCast(DurningSequence,
	//						[=, AfterSequence = DurningSequence]() __declspec(noinline) {
	//						// ==== ON AFTER TIMING RESPONSE ENDED ====
	//						DMS_LOG_SIMPLE(TEXT("==== %s : ON AFTER TIMING RESPONSE ENDED [ Depth : %d ] ===="), *AfterSequence->GetName(), SeqManager->GetDepth(AfterSequence));

	//						// Run child effect if exist.
	//						if (AfterSequence->SequenceState != EDMSSequenceState::SS_Canceled &&
	//							AfterSequence->OriginalEffectNode->ChildEffect != nullptr && AfterSequence->OriginalEffectNode->ChildEffect->GetEffectNode() != nullptr &&
	//							AfterSequence->OriginalEffectNode->ChildEffect->GetEffectNode()->Conditions->CheckCondition(AfterSequence->SourceObject, AfterSequence)) {

	//							// Proceed to run child effect sequence.
	//							DMS_LOG_SIMPLE(TEXT("==== %s : OnNotifyReceived -> Advance ===="), *AfterSequence->GetName());
	//							auto ChildNode = AfterSequence->OriginalEffectNode->ChildEffect->GetEffectNode();
	//							// follows parents data. 
	//							auto NewSeq = RequestCreateSequence(AfterSequence->SourceObject, AfterSequence->SourcePlayer, ChildNode,
	//								TArray<TScriptInterface<IDMSEffectorInterface>>(), AfterSequence->EIDatas, AfterSequence);

	//							// Set delegates when child effect sequence completed.
	//							NewSeq->AddToOnSequenceFinished_Native(
	//								[=, ParentSequence = AfterSequence]() __declspec(noinline) {
	//								// ==== ON CHILD EFFECT SEQUENCE COMPLETED ====
	//								DMS_LOG_SIMPLE(TEXT("==== %s : ON CHILD EFFECT SEQUENCE COMPLETED [ Depth : %d ] ==== "), *ParentSequence->GetName(), SeqManager->GetDepth(ParentSequence));

	//								auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager();

	//								// Resume parent sequence closing
	//								SeqManager->CompleteSequence(ParentSequence);
	//							});

	//							// Run setuped child effect sequence.
	//							SeqManager->RunSequence(NewSeq);
	//						}
	//						else //Complete this sequence.
	//							SeqManager->CompleteSequence(AfterSequence);
	//					});
	//				});

	//			});
	//	});
}

void UDMSSequenceStep_Apply::OnDuring_Implementation()
{
	// Behavior
	ProgressComplete();
}

void UDMSSequenceStep_Apply::OnAfter_Implementation()
{
	// Behavior
	ProgressComplete();
}
