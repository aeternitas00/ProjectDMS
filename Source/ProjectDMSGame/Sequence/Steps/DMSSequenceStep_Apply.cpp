// Fill out your copyright notice in the Description page of Project Settings.


#include "Sequence/Steps/DMSSequenceStep_Apply.h"
#include "Sequence/DMSSeqManager.h"

#include "Conditions/DMSConditionObject.h"
#include "Notify/DMSNotifyManager.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Effect/DMSEffectDefinition.h"
#include "Effect/DMSEffectHandler.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Step_Apply, "Step.Apply");

UDMSSequenceStep_Apply::UDMSSequenceStep_Apply()
{
	StepTag = TAG_DMS_Step_Apply;
}

void UDMSSequenceStep_Apply::OnBefore_Implementation()
{
	// Behavior
	auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager(); 	check(NotifyManager);
	auto EffectHandler = UDMSCoreFunctionLibrary::GetDMSEffectHandler();	check(EffectHandler);
	auto SeqManager = UDMSCoreFunctionLibrary::GetDMSSequenceManager();		check(SeqManager);

	DMS_LOG_SIMPLE(TEXT("==== %s : Apply Sequence [ Depth : %d ] ===="), *OwnerSequence->GetName(), SeqManager->GetDepth(OwnerSequence));

	DMS_LOG_SCREEN(TEXT("==-- ApplyStep Starts by %s : [%s]"), *OwnerSequence->GetSourceObject()->GetName(),
		*OwnerSequence->OriginalEffectNode->GenerateTagContainer().ToString());

	DMS_LOG_SCREEN(TEXT("==-- ApplyStep_BEFORE [ Depth : %d ] --=="), SeqManager->GetDepth(OwnerSequence));

	NotifyManager->Broadcast(OwnerSequence,	[=]() {

		// ==== ON BEFORE TIMING RESPONSE ENDED ====
		ProgressComplete();

	});

}

void UDMSSequenceStep_Apply::OnDuring_Implementation()
{
	// Behavior
	auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager(); 	check(NotifyManager);
	auto EffectHandler = UDMSCoreFunctionLibrary::GetDMSEffectHandler();	check(EffectHandler);
	auto SeqManager = UDMSCoreFunctionLibrary::GetDMSSequenceManager();		check(SeqManager);

	DMS_LOG_SIMPLE(TEXT("==== %s : ON BEFORE TIMING RESPONSE ENDED [ Depth : %d ] ===="), *OwnerSequence->GetName(), SeqManager->GetDepth(OwnerSequence));
	// Proceed to 'During Timing'
	//BeforeSequence->Progress = EDMSTimingFlag::T_During;

	DMS_LOG_SCREEN(TEXT("==-- ApplyStep_DURING [ Depth : %d ] --=="), SeqManager->GetDepth(OwnerSequence));

	// Resolve First
	DMS_LOG_SIMPLE(TEXT("==== %s : RESOLVE START ===="), *OwnerSequence->GetName());

	EffectHandler->Resolve(OwnerSequence,
		[=](bool ResolveSuccessed) {
			// ==== ON RESOLVE COMPLETED ====

			if (ResolveSuccessed) {
				DMS_LOG_SCREEN(TEXT("==-- ApplyStep_DURING : Successed [ Depth : %d ] --=="), SeqManager->GetDepth(OwnerSequence));
				DMS_LOG_SIMPLE(TEXT("==== %s : ON RESOLVE COMPLETED [ Depth : %d ] ===="), *OwnerSequence->GetName(), SeqManager->GetDepth(OwnerSequence));
				// 'During Timing' broadcast starts.
				auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager();
				NotifyManager->Broadcast(OwnerSequence,
					[=]() __declspec(noinline) {
					// ==== ON DURING TIMING RESPONSE ENDED ====
					DMS_LOG_SIMPLE(TEXT("==== %s : ON DURING TIMING RESPONSE ENDED [ Depth : %d ] ===="), *OwnerSequence->GetName(), SeqManager->GetDepth(OwnerSequence));
					// Proceed to 'After Timing'
					//DurningSequence->Progress = EDMSTimingFlag::T_After;
					ProgressComplete();
				});
					
			}
			else {
				DMS_LOG_SCREEN(TEXT("==-- ApplyStep_DURING : Failed [ Depth : %d ] --=="), SeqManager->GetDepth(OwnerSequence));
				ProgressComplete(false);
			}
		});

}

void UDMSSequenceStep_Apply::OnAfter_Implementation()
{
	// Behavior
	auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager(); 	check(NotifyManager);
	auto EffectHandler = UDMSCoreFunctionLibrary::GetDMSEffectHandler();	check(EffectHandler);
	auto SeqManager = UDMSCoreFunctionLibrary::GetDMSSequenceManager();		check(SeqManager);

	DMS_LOG_SCREEN(TEXT("==-- ApplyStep_AFTER [ Depth : %d ] --=="), SeqManager->GetDepth(OwnerSequence));

	// 'After Timing' broadcast starts.
	NotifyManager->Broadcast(OwnerSequence,
		[=]() __declspec(noinline) {
		// ==== ON AFTER TIMING RESPONSE ENDED ====
		DMS_LOG_SIMPLE(TEXT("==== %s : ON AFTER TIMING RESPONSE ENDED [ Depth : %d ] ===="), *OwnerSequence->GetName(), SeqManager->GetDepth(OwnerSequence));

		// Run child effect if exist.
		if (OwnerSequence->SequenceState != EDMSSequenceState::SS_Canceled &&
			OwnerSequence->OriginalEffectNode->ChildEffect != nullptr && OwnerSequence->OriginalEffectNode->ChildEffect->GetEffectNode() != nullptr &&
			OwnerSequence->OriginalEffectNode->ChildEffect->GetEffectNode()->Conditions->CheckCondition(OwnerSequence->GetSourceObject(), OwnerSequence)) {

			DMS_LOG_SCREEN(TEXT("==-- Child Sequence Created [ Depth : %d ] --=="), SeqManager->GetDepth(OwnerSequence));
			// Proceed to run child effect sequence.
			DMS_LOG_SIMPLE(TEXT("==== %s : OnNotifyReceived -> Advance ===="), *OwnerSequence->GetName());
			auto ChildNode = OwnerSequence->OriginalEffectNode->ChildEffect->GetEffectNode();
			// follows parents data. 
			auto NewSeq = SeqManager->RequestCreateSequence(OwnerSequence->GetSourceObject(), OwnerSequence->GetSourcePlayer(), ChildNode,
				TArray<TScriptInterface<IDMSEffectorInterface>>(), OwnerSequence->EIDatas, OwnerSequence);

			// Set delegates when child effect sequence completed.
			NewSeq->AddToOnSequenceFinished_Native(
				[=, ParentSequence = OwnerSequence](bool Successed) __declspec(noinline) {
				// ==== ON CHILD EFFECT SEQUENCE COMPLETED ====
				DMS_LOG_SIMPLE(TEXT("==== %s : ON CHILD EFFECT SEQUENCE COMPLETED [ Depth : %d ] ==== "), *ParentSequence->GetName(), SeqManager->GetDepth(ParentSequence));

				auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager();

				// Resume parent sequence closing
				ProgressComplete();
			});

			// Run setup child effect sequence.
			SeqManager->RunSequence(NewSeq);
		}

		else {//Complete this step.
			//DMS_LOG_SCREEN(TEXT("==-- ApplyStep_AFTER : Failed [ Depth : %d ] --=="), SeqManager->GetDepth(OwnerSequence));
			ProgressComplete();
		}
	});
}
