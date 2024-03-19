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
	//StepTag = TAG_DMS_Step_Apply;
}

void UDMSSequenceStep_Apply::OnStepInitiated()
{
	if(!OwnerSequence->IsTargetted()){
		auto EH = UDMSCoreFunctionLibrary::GetDMSEffectHandler(this); check(EH);
		EH->CreateApplyingActiveEffect(OwnerSequence, OwnerSequence->OriginalEffectNode);
	}
	UDMSSequenceStep::OnStepInitiated();
}

inline FGameplayTagContainer UDMSSequenceStep_Apply::GetStepTag_Implementation() const {return FGameplayTagContainer(TAG_DMS_Step_Apply);}

void UDMSSequenceStep_Apply::OnBefore_Implementation()
{
	// ==== ON BEFORE TIMING RESPONSE ENDED ====

	auto EffectHandler	=	UDMSCoreFunctionLibrary::GetDMSEffectHandler(this);			check(EffectHandler);
	auto SeqManager		=	UDMSCoreFunctionLibrary::GetDMSSequenceManager(this);		check(SeqManager);

	DMS_LOG_SIMPLE(TEXT("==== %s : Apply Sequence [ Depth : %d ] ===="), *OwnerSequence->GetName(), SeqManager->GetDepth(OwnerSequence));

	ProgressComplete();
}

void UDMSSequenceStep_Apply::Progress_During()
{
	// Behavior
	auto NotifyManager	=	UDMSCoreFunctionLibrary::GetDMSNotifyManager(this);			check(NotifyManager);
	auto EffectHandler	=	UDMSCoreFunctionLibrary::GetDMSEffectHandler(this);			check(EffectHandler);
	auto SeqManager		=	UDMSCoreFunctionLibrary::GetDMSSequenceManager(this);		check(SeqManager);

	DMS_LOG_SIMPLE(TEXT("==== %s : Step progress During ===="), *GetClass()->GetName());

	DMS_LOG_SIMPLE(TEXT("==== %s : ON BEFORE TIMING RESPONSE ENDED [ Depth : %d ] ===="), *OwnerSequence->GetName(), SeqManager->GetDepth(OwnerSequence));
	
	// Proceed to 'During Timing'
	// Resolve First
	DMS_LOG_SIMPLE(TEXT("==== %s : RESOLVE START ===="), *OwnerSequence->GetName());

	EffectHandler->Resolve(OwnerSequence, [=](bool ResolveSucceeded) {
		// ==== ON RESOLVE COMPLETED ====
		if (ResolveSucceeded) {
			//DMS_LOG_SCREEN(TEXT("==-- ApplyStep_DURING : Succeeded [ Depth : %d ] --=="), SeqManager->GetDepth(OwnerSequence));

			DMS_LOG_SIMPLE(TEXT("==== %s : ON RESOLVE COMPLETED [ Depth : %d ] ===="), *OwnerSequence->GetName(), SeqManager->GetDepth(OwnerSequence));
			// 'During Timing' broadcast starts.

			auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager(this);

			check(NotifyManager);

			NotifyManager->Broadcast(OwnerSequence, [this]() {OnDuring(); });

		}
		else {
			DMS_LOG_SIMPLE(TEXT("==== %s : ON RESOLVE FAILED [ Depth : %d ] ===="), *OwnerSequence->GetName(), SeqManager->GetDepth(OwnerSequence));
			ProgressComplete(false);
		}
	});
}


void UDMSSequenceStep_Apply::OnDuring_Implementation()
{
	auto SeqManager		=	UDMSCoreFunctionLibrary::GetDMSSequenceManager(this);		check(SeqManager);

	// ==== ON DURING TIMING RESPONSE ENDED ====

	DMS_LOG_SIMPLE(TEXT("==== %s : ON DURING TIMING RESPONSE ENDED [ Depth : %d ] ===="), *OwnerSequence->GetName(), SeqManager->GetDepth(OwnerSequence));

	ProgressComplete();
}

void UDMSSequenceStep_Apply::OnAfter_Implementation()
{
	// When 'After Timing' broadcast finished.
	auto NotifyManager	=	UDMSCoreFunctionLibrary::GetDMSNotifyManager(this); 		check(NotifyManager);
	auto EffectHandler	=	UDMSCoreFunctionLibrary::GetDMSEffectHandler(this);			check(EffectHandler);
	auto SeqManager		=	UDMSCoreFunctionLibrary::GetDMSSequenceManager(this);		check(SeqManager);

	//DMS_LOG_SCREEN(TEXT("==-- ApplyStep_AFTER [ Depth : %d ] --=="), SeqManager->GetDepth(OwnerSequence));

	// ==== ON AFTER TIMING RESPONSE ENDED ====
	DMS_LOG_SIMPLE(TEXT("==== %s : ON AFTER TIMING RESPONSE ENDED [ Depth : %d ] ===="), *OwnerSequence->GetName(), SeqManager->GetDepth(OwnerSequence));

	// Run child effect if exist.
	if (OwnerSequence->OriginalEffectNode->ChildEffect != nullptr && OwnerSequence->OriginalEffectNode->ChildEffect->GetEffectNode() != nullptr &&
		OwnerSequence->OriginalEffectNode->ChildEffect->GetEffectNode()->Conditions->CheckCondition(OwnerSequence->GetSourceObject(), OwnerSequence)) {

		// Proceed to run child effect sequence.
		DMS_LOG_SIMPLE(TEXT("==== %s : Child Sequence Created -> Advance ===="), *OwnerSequence->GetName());
		auto ChildNode = OwnerSequence->OriginalEffectNode->ChildEffect->GetEffectNode();
		// follows parents data. 
		auto NewSeq = SeqManager->RequestCreateSequence(OwnerSequence->GetSourceObject(), OwnerSequence->GetSourcePlayer(), ChildNode,
			TArray<TScriptInterface<IDMSEffectorInterface>>(), OwnerSequence->SequenceDatas, OwnerSequence);

		// Set delegates when child effect sequence completed.
		NewSeq->AddToOnSequenceFinished_Native(
			[=, ParentSequence = OwnerSequence](bool Succeeded) __declspec(noinline) {
			// ==== ON CHILD EFFECT SEQUENCE COMPLETED ====
			DMS_LOG_SIMPLE(TEXT("==== %s : ON CHILD EFFECT SEQUENCE COMPLETED [ Depth : %d ] ==== "), *ParentSequence->GetName(), SeqManager->GetDepth(ParentSequence));

			auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager(this);

			// Resume parent sequence closing
			this->ProgressComplete();
		});

		// Run setup child effect sequence.
		SeqManager->RunSequence(NewSeq);
	}

	else {
		//Complete this step.
		ProgressComplete();
	}
}

UDMSSequenceStepDefinition_Apply::UDMSSequenceStepDefinition_Apply()
{
}

void UDMSSequenceStepDefinition_Apply::Progress_Before(UDMSSequenceStep* InstancedStep)
{
}

void UDMSSequenceStepDefinition_Apply::Progress_During(UDMSSequenceStep* InstancedStep)
{
}

void UDMSSequenceStepDefinition_Apply::Progress_After(UDMSSequenceStep* InstancedStep)
{
}

FGameplayTagContainer UDMSSequenceStepDefinition_Apply::GetStepTag_Implementation() const
{
	return FGameplayTagContainer(TAG_DMS_Step_Apply);
}

bool UDMSSequenceStepDefinition_Apply::GetProgressOps_Implementation(const FGameplayTag& ProgressTag, TArray<FProgressExecutor>& OutExecutor)
{
	if(ProgressTag.MatchesTagExact(TAG_DMS_Step_Apply)){
		FProgressExecutor_Signature B,D,A;
		B.BindDynamic(this,&UDMSSequenceStepDefinition_Apply::Progress_Before);
		D.BindDynamic(this,&UDMSSequenceStepDefinition_Apply::Progress_During);
		A.BindDynamic(this,&UDMSSequenceStepDefinition_Apply::Progress_After);
		OutExecutor.Append({B,D,A});	
		return true;
	}
	else if(ProgressTag.MatchesTag(TAG_DMS_Step_Apply)){
		// Alt :: Using native defined tag 
		TArray<FString> Words;	ProgressTag.ToString().ParseIntoArray(Words,TEXT("."));
		FName FunctionName(TEXT("Progress_%s",*Words.Last()));

		if ( FindFunction(FunctionName) == nullptr ) return false;

		OutExecutor.Add({this,FunctionName});		
	}

	return false;
}
