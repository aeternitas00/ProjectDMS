// Fill out your copyright notice in the Description page of Project Settings.


#include "Sequence/Steps/DMSSequenceStep_Apply.h"
#include "Sequence/DMSSeqManager.h"

#include "Conditions/DMSConditionObject.h"
#include "Notify/DMSNotifyManager.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Effect/DMSEffectDefinition.h"
#include "Effect/DMSEffectHandler.h"

void UDMSSequenceStep_Apply::OnBefore_Implementation()
{
	auto SM = UDMSCoreFunctionLibrary::GetDMSSequenceManager(); check(SM);

	DMS_LOG_SIMPLE(TEXT("==== %s : Apply Sequence [ Depth : %d ] ===="), *OwnerSequence->GetName(), SM->GetDepth(OwnerSequence));

	DMS_LOG_SCREEN(TEXT("%s : ApplySequence [%s]"), *OwnerSequence->GetSourceObject()->GetName(),
		*OwnerSequence->OriginalEffectNode->GenerateTagContainer().ToString());
	// Before Notify

	auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager();
	auto EffectHandler = UDMSCoreFunctionLibrary::GetDMSEffectHandler();
	check(NotifyManager);
	check(EffectHandler);

	//OwnerSequence->SetActive(true);

	// TODO :: Migrate to ApplyStep

	DMS_LOG_SCREEN(TEXT("==-- BEFORE [ Depth : %d ] --=="), SM->GetDepth(OwnerSequence));
	// 'Before Timing' broadcast starts.
	OwnerSequence->Progress = EDMSTimingFlag::T_Before;
	NotifyManager->Broadcast(OwnerSequence,
	[=, BeforeSequence = OwnerSequence]() {
		// ==== ON BEFORE TIMING RESPONSE ENDED ====

		auto SeqManager = UDMSCoreFunctionLibrary::GetDMSSequenceManager();

		DMS_LOG_SIMPLE(TEXT("==== %s : ON BEFORE TIMING RESPONSE ENDED [ Depth : %d ] ===="), *BeforeSequence->GetName(), SeqManager->GetDepth(BeforeSequence));
		// Proceed to 'During Timing'
		BeforeSequence->Progress = EDMSTimingFlag::T_During;
	
		DMS_LOG_SCREEN(TEXT("==-- DURING [ Depth : %d ] --=="), SeqManager->GetDepth(BeforeSequence));

		// Resolve First
		DMS_LOG_SIMPLE(TEXT("==== %s : RESOLVE START ===="), *BeforeSequence->GetName());

		EffectHandler->Resolve(BeforeSequence,
		[=, DuringSequence = BeforeSequence](bool ResolveSuccessed) {
			// ==== ON RESOLVE COMPLETED ====

			if (ResolveSuccessed) {

				DMS_LOG_SIMPLE(TEXT("==== %s : ON RESOLVE COMPLETED [ Depth : %d ] ===="), *DuringSequence->GetName(), SeqManager->GetDepth(DuringSequence));
				// 'During Timing' broadcast starts.
				auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager();
				NotifyManager->Broadcast(BeforeSequence,
					[=, DurningSequence = BeforeSequence]() __declspec(noinline) {
					// ==== ON DURING TIMING RESPONSE ENDED ====
					DMS_LOG_SIMPLE(TEXT("==== %s : ON DURING TIMING RESPONSE ENDED [ Depth : %d ] ===="), *DurningSequence->GetName(), SeqManager->GetDepth(DurningSequence));
					// Proceed to 'After Timing'
					DurningSequence->Progress = EDMSTimingFlag::T_After;

					DMS_LOG_SCREEN(TEXT("==-- AFTER [ Depth : %d ] --=="), SeqManager->GetDepth(DurningSequence));

					// 'After Timing' broadcast starts.
					NotifyManager->Broadcast(DurningSequence,
						[=, AfterSequence = DurningSequence]() __declspec(noinline) {
						// ==== ON AFTER TIMING RESPONSE ENDED ====
						DMS_LOG_SIMPLE(TEXT("==== %s : ON AFTER TIMING RESPONSE ENDED [ Depth : %d ] ===="), *AfterSequence->GetName(), SeqManager->GetDepth(AfterSequence));

						// Run child effect if exist.
						if (AfterSequence->SequenceState != EDMSSequenceState::SS_Canceled &&
							AfterSequence->OriginalEffectNode->ChildEffect != nullptr && AfterSequence->OriginalEffectNode->ChildEffect->GetEffectNode() != nullptr &&
							AfterSequence->OriginalEffectNode->ChildEffect->GetEffectNode()->Conditions->CheckCondition(AfterSequence->GetSourceObject(), AfterSequence)) {

							// Proceed to run child effect sequence.
							DMS_LOG_SIMPLE(TEXT("==== %s : OnNotifyReceived -> Advance ===="), *AfterSequence->GetName());
							auto ChildNode = AfterSequence->OriginalEffectNode->ChildEffect->GetEffectNode();
							// follows parents data. 
							auto NewSeq = SeqManager->RequestCreateSequence(AfterSequence->GetSourceObject(), AfterSequence->GetSourcePlayer(), ChildNode,
								TArray<TScriptInterface<IDMSEffectorInterface>>(), AfterSequence->EIDatas, AfterSequence);

							// Set delegates when child effect sequence completed.
							NewSeq->AddToOnSequenceFinished_Native(
								[=, ParentSequence = AfterSequence](bool Successed) __declspec(noinline) {
								// ==== ON CHILD EFFECT SEQUENCE COMPLETED ====
								DMS_LOG_SIMPLE(TEXT("==== %s : ON CHILD EFFECT SEQUENCE COMPLETED [ Depth : %d ] ==== "), *ParentSequence->GetName(), SeqManager->GetDepth(ParentSequence));

								auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager();

								// Resume parent sequence closing
								SeqManager->CompleteSequence(ParentSequence);
							});

							// Run setup child effect sequence.
							SeqManager->RunSequence(NewSeq);
						}
						else //Complete this sequence.
							SeqManager->CompleteSequence(AfterSequence);
					});
				});
			}
			else {
				SeqManager->CompleteSequence(DuringSequence, false);
			}
		});
	});
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
