// Fill out your copyright notice in the Description page of Project Settings.


#include "Sequence/Steps/DMSSequenceStep_Apply.h"
#include "Sequence/DMSSeqManager.h"

#include "Conditions/DMSConditionObject.h"
#include "Notify/DMSNotifyManager.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Effect/DMSEffectDefinition.h"
#include "Effect/DMSEffectHandler.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Step_Apply, "Step.Apply");

UDMSSequenceStepDefinition_Apply::UDMSSequenceStepDefinition_Apply()
{
}

void UDMSSequenceStepDefinition_Apply::Progress_Before(UDMSSequenceStep* InstancedStep)
{
	DMS_LOG_SIMPLE(TEXT("==== %s : Step progress Before ===="), *GetClass()->GetName());
	BroadcastProgress(InstancedStep,FName(NAME_None));
}

void UDMSSequenceStepDefinition_Apply::Progress_During(UDMSSequenceStep* InstancedStep)
{
	auto NotifyManager	=	UDMSCoreFunctionLibrary::GetDMSNotifyManager(InstancedStep);		check(NotifyManager);
	auto EffectHandler	=	UDMSCoreFunctionLibrary::GetDMSEffectHandler(InstancedStep);		check(EffectHandler);
	auto SeqManager		=	UDMSCoreFunctionLibrary::GetDMSSequenceManager(InstancedStep);		check(SeqManager);
	auto Seq = InstancedStep->OwnerSequence;

	DMS_LOG_SIMPLE(TEXT("==== %s : Step progress During ===="), *GetClass()->GetName());

	DMS_LOG_SIMPLE(TEXT("==== %s : ON BEFORE TIMING RESPONSE ENDED [ Depth : %d ] ===="), *Seq->GetName(), SeqManager->GetDepth(Seq));

	// Proceed to 'During Timing'
	// Resolve 
	DMS_LOG_SIMPLE(TEXT("==== %s : RESOLVE START ===="), *Seq->GetName());

	FOnTaskCompleted OnResolveCompleted;

	OnResolveCompleted.BindDynamic(InstancedStep, &UDMSSequenceStep::ProgressEnd);

	//OnResolveCompleted.BindLambda([=](bool ResolveSucceeded) {
	//	// ==== ON RESOLVE COMPLETED ====
	//	if (ResolveSucceeded) {
	//		DMS_LOG_SIMPLE(TEXT("==== %s : ON RESOLVE COMPLETED [ Depth : %d ] ===="), *Seq->GetName(), SeqManager->GetDepth(Seq));
	//		InstancedStep->ProgressEnd(true);
	//	}
	//	else {
	//		DMS_LOG_SIMPLE(TEXT("==== %s : ON RESOLVE FAILED [ Depth : %d ] ===="), *Seq->GetName(), SeqManager->GetDepth(Seq));
	//		InstancedStep->ProgressEnd(false);
	//	}
	//	DMS_LOG_SIMPLE(TEXT("==== %s : after resolve effect lambda ends ===="),*Seq->GetName());
	//});

	EffectHandler->Resolve(Seq, OnResolveCompleted);
}

void UDMSSequenceStepDefinition_Apply::Progress_After(UDMSSequenceStep* InstancedStep)
{
	DMS_LOG_SIMPLE(TEXT("==== %s : Step progress After ===="), *GetClass()->GetName());
	BroadcastProgress(InstancedStep,FName("ApplyChildEffect"));
}

void UDMSSequenceStepDefinition_Apply::ApplyChildEffect(UDMSSequenceStep* InstancedStep)
{
	// When 'After Timing' broadcast finished.
	auto NotifyManager	=	UDMSCoreFunctionLibrary::GetDMSNotifyManager(InstancedStep); 		check(NotifyManager);
	auto EffectHandler	=	UDMSCoreFunctionLibrary::GetDMSEffectHandler(InstancedStep);		check(EffectHandler);
	auto SeqManager		=	UDMSCoreFunctionLibrary::GetDMSSequenceManager(InstancedStep);		check(SeqManager);
	auto Seq = InstancedStep->OwnerSequence;

	//DMS_LOG_SCREEN(TEXT("==-- ApplyStep_AFTER [ Depth : %d ] --=="), SeqManager->GetDepth(OwnerSequence));

	// ==== ON AFTER TIMING RESPONSE ENDED ====
	DMS_LOG_SIMPLE(TEXT("==== %s : ON AFTER TIMING RESPONSE ENDED [ Depth : %d ] ===="), *Seq->GetName(), SeqManager->GetDepth(Seq));

	// Run child effect if exist and condition check succeeded.
	if (Seq->OriginalEffectNode->ChildEffect != nullptr && Seq->OriginalEffectNode->ChildEffect->GetEffectNode() != nullptr &&
		Seq->OriginalEffectNode->ChildEffect->GetEffectNode()->Conditions->CheckCondition(Seq->GetSourceObject(), Seq)) {

		// Proceed to run child effect sequence.
		DMS_LOG_SIMPLE(TEXT("==== %s : Child Sequence Created -> Advance ===="), *Seq->GetName());
		auto ChildNode = Seq->OriginalEffectNode->ChildEffect->GetEffectNode();
		// follows parents data. 
		auto NewSeq = SeqManager->RequestCreateSequence(Seq->GetSourceObject(), Seq->GetSourcePlayer(), ChildNode,
			TArray<TScriptInterface<IDMSEffectorInterface>>(), true, Seq);

		// Set delegates when child effect sequence completed.
		NewSeq->AddToPreSequenceFinished_Native(
			[=, ParentSequence = Seq](bool Succeeded) __declspec(noinline) {
			// ==== ON CHILD EFFECT SEQUENCE COMPLETED ====
			DMS_LOG_SIMPLE(TEXT("==== %s : ON CHILD EFFECT SEQUENCE COMPLETED [ Depth : %d ] ==== "), *ParentSequence->GetName(), SeqManager->GetDepth(ParentSequence));

			// Resume parent sequence closing
			InstancedStep->ProgressEnd();

			DMS_LOG_SIMPLE(TEXT("==== %s : after activate child effect lambda ends ===="),*ParentSequence->GetName());
		});

		// Run setup child effect sequence.
		SeqManager->RunSequence(NewSeq);
	}

	else {
		//Complete this step.
		InstancedStep->ProgressEnd();
	}
}


// Implementations

FGameplayTag UDMSSequenceStepDefinition_Apply::GetPureStepTag_Implementation() const
{
	return FGameplayTag::RequestGameplayTag("Step.Arkham.Apply");
}

FGameplayTagContainer UDMSSequenceStepDefinition_Apply::GetStepTag_Implementation(UDMSSequenceStep* InstancedStep) const
{
	// combine tag with applying context?
	FGameplayTagContainer rv = GetPureStepTag().GetSingleTagContainer();
	
	return rv;
	//return FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Step.Arkham.Apply"));
}

bool UDMSSequenceStepDefinition_Apply::GetProgressOps_Implementation(const FGameplayTag& ProgressTag, TArray<FProgressExecutor>& OutExecutor)
{
	auto BaseTag = FGameplayTag::RequestGameplayTag("Step.Arkham.Apply");
	if(ProgressTag.MatchesTagExact(BaseTag)){
		OutExecutor.Append({
			{this,FGameplayTag::RequestGameplayTag("Step.Arkham.Apply.Before"),FName(TEXT("Progress_Before"))},
			{this,FGameplayTag::RequestGameplayTag("Step.Arkham.Apply.During"),FName(TEXT("Progress_During"))},
			{this,FGameplayTag::RequestGameplayTag("Step.Arkham.Apply.After"),FName(TEXT("Progress_After"))}
		});	
		return true;
	}
	else if(ProgressTag.MatchesTag(BaseTag)){
		// Alt :: Using native defined tag 
		TArray<FString> Words;	ProgressTag.ToString().ParseIntoArray(Words,TEXT("."));
		FName FunctionName(FString::Printf(TEXT("Progress_%s"), *Words.Last()));
		if ( FindFunction(FunctionName) == nullptr ) return false;

		OutExecutor.Add({this,ProgressTag,FunctionName});		
	}

	return false;
}

//TArray<UDMSEffectDefinition*> UDMSSequenceStepDefinition_Apply::GetStepResolvingContext_Implementation(ADMSActiveEffect* CurrentAE, UDMSSequenceStep* InstancedStep)
//{	
//	TArray<UDMSEffectDefinition*> rv;
//	for (auto& CondDef : ConditionedEffectDefinitions)
//	{
//		if ( CondDef.ApplyConditions && !CondDef.ApplyConditions->CheckCondition(CurrentAE, InstancedStep->OwnerSequence) ) continue;
//		bool Check = true;
//		for(auto& CurrentDef : CondDef.EffectDefinitions ){
//			if(!CurrentDef->Predict(InstancedStep->OwnerSequence, CurrentAE))
//			{
//				Check = false; break;
//			}
//		}
//		if(Check) rv.Append(CondDef.EffectDefinitions);
//	}
//	return rv;
//}


// LEGACY

//UDMSSequenceStep_Apply::UDMSSequenceStep_Apply()
//{
//	//StepTag = TAG_DMS_Step_Apply;
//}

//void UDMSSequenceStep_Apply::OnStepInitiated()
//{
//	if(!OwnerSequence->IsTargetted()){
//		auto EH = UDMSCoreFunctionLibrary::GetDMSEffectHandler(this); check(EH);
//		EH->CreateApplyingActiveEffect(OwnerSequence, OwnerSequence->OriginalEffectNode);
//	}
//	UDMSSequenceStep::OnStepInitiated();
//}
//
//inline FGameplayTagContainer UDMSSequenceStep_Apply::GetStepTag_Implementation() const {return FGameplayTagContainer(TAG_DMS_Step_Apply);}
//
//void UDMSSequenceStep_Apply::OnBefore_Implementation()
//{
//	// ==== ON BEFORE TIMING RESPONSE ENDED ====
//
//	auto EffectHandler	=	UDMSCoreFunctionLibrary::GetDMSEffectHandler(this);			check(EffectHandler);
//	auto SeqManager		=	UDMSCoreFunctionLibrary::GetDMSSequenceManager(this);		check(SeqManager);
//
//	DMS_LOG_SIMPLE(TEXT("==== %s : Apply Sequence [ Depth : %d ] ===="), *OwnerSequence->GetName(), SeqManager->GetDepth(OwnerSequence));
//
//	ProgressComplete();
//}
//
//void UDMSSequenceStep_Apply::Progress_During()
//{
//	// Behavior
//	auto NotifyManager	=	UDMSCoreFunctionLibrary::GetDMSNotifyManager(this);			check(NotifyManager);
//	auto EffectHandler	=	UDMSCoreFunctionLibrary::GetDMSEffectHandler(this);			check(EffectHandler);
//	auto SeqManager		=	UDMSCoreFunctionLibrary::GetDMSSequenceManager(this);		check(SeqManager);
//
//	DMS_LOG_SIMPLE(TEXT("==== %s : Step progress During ===="), *GetClass()->GetName());
//
//	DMS_LOG_SIMPLE(TEXT("==== %s : ON BEFORE TIMING RESPONSE ENDED [ Depth : %d ] ===="), *OwnerSequence->GetName(), SeqManager->GetDepth(OwnerSequence));
//	
//	// Proceed to 'During Timing'
//	// Resolve First
//	DMS_LOG_SIMPLE(TEXT("==== %s : RESOLVE START ===="), *OwnerSequence->GetName());
//
//	FOnResolveCompleted OnResolveCompleted;
//
//	OnResolveCompleted.BindLambda([=](bool ResolveSucceeded) {
//		// ==== ON RESOLVE COMPLETED ====
//		if (ResolveSucceeded) {
//			//DMS_LOG_SCREEN(TEXT("==-- ApplyStep_DURING : Succeeded [ Depth : %d ] --=="), SeqManager->GetDepth(OwnerSequence));
//
//			DMS_LOG_SIMPLE(TEXT("==== %s : ON RESOLVE COMPLETED [ Depth : %d ] ===="), *OwnerSequence->GetName(), SeqManager->GetDepth(OwnerSequence));
//			// 'During Timing' broadcast starts.
//
//			auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager(this);
//
//			check(NotifyManager);
//
//			NotifyManager->Broadcast(OwnerSequence, [this]() {OnDuring(); });
//
//		}
//		else {
//			DMS_LOG_SIMPLE(TEXT("==== %s : ON RESOLVE FAILED [ Depth : %d ] ===="), *OwnerSequence->GetName(), SeqManager->GetDepth(OwnerSequence));
//			ProgressComplete(false);
//		}
//	});
//
//	EffectHandler->Resolve(OwnerSequence, OnResolveCompleted);
//}
//
//
//void UDMSSequenceStep_Apply::OnDuring_Implementation()
//{
//	auto SeqManager		=	UDMSCoreFunctionLibrary::GetDMSSequenceManager(this);		check(SeqManager);
//
//	// ==== ON DURING TIMING RESPONSE ENDED ====
//
//	DMS_LOG_SIMPLE(TEXT("==== %s : ON DURING TIMING RESPONSE ENDED [ Depth : %d ] ===="), *OwnerSequence->GetName(), SeqManager->GetDepth(OwnerSequence));
//
//	ProgressComplete();
//}
//
//void UDMSSequenceStep_Apply::OnAfter_Implementation()
//{
//	// When 'After Timing' broadcast finished.
//	auto NotifyManager	=	UDMSCoreFunctionLibrary::GetDMSNotifyManager(this); 		check(NotifyManager);
//	auto EffectHandler	=	UDMSCoreFunctionLibrary::GetDMSEffectHandler(this);			check(EffectHandler);
//	auto SeqManager		=	UDMSCoreFunctionLibrary::GetDMSSequenceManager(this);		check(SeqManager);
//
//	//DMS_LOG_SCREEN(TEXT("==-- ApplyStep_AFTER [ Depth : %d ] --=="), SeqManager->GetDepth(OwnerSequence));
//
//	// ==== ON AFTER TIMING RESPONSE ENDED ====
//	DMS_LOG_SIMPLE(TEXT("==== %s : ON AFTER TIMING RESPONSE ENDED [ Depth : %d ] ===="), *OwnerSequence->GetName(), SeqManager->GetDepth(OwnerSequence));
//
//	// Run child effect if exist.
//	if (OwnerSequence->OriginalEffectNode->ChildEffect != nullptr && OwnerSequence->OriginalEffectNode->ChildEffect->GetEffectNode() != nullptr &&
//		OwnerSequence->OriginalEffectNode->ChildEffect->GetEffectNode()->Conditions->CheckCondition(OwnerSequence->GetSourceObject(), OwnerSequence)) {
//
//		// Proceed to run child effect sequence.
//		DMS_LOG_SIMPLE(TEXT("==== %s : Child Sequence Created -> Advance ===="), *OwnerSequence->GetName());
//		auto ChildNode = OwnerSequence->OriginalEffectNode->ChildEffect->GetEffectNode();
//		// follows parents data. 
//		auto NewSeq = SeqManager->RequestCreateSequence(OwnerSequence->GetSourceObject(), OwnerSequence->GetSourcePlayer(), ChildNode,
//			TArray<TScriptInterface<IDMSEffectorInterface>>(), OwnerSequence->SequenceDatas, OwnerSequence);
//
//		// Set delegates when child effect sequence completed.
//		NewSeq->AddToPreSequenceFinished_Native(
//			[=, ParentSequence = OwnerSequence](bool Succeeded) __declspec(noinline) {
//			// ==== ON CHILD EFFECT SEQUENCE COMPLETED ====
//			DMS_LOG_SIMPLE(TEXT("==== %s : ON CHILD EFFECT SEQUENCE COMPLETED [ Depth : %d ] ==== "), *ParentSequence->GetName(), SeqManager->GetDepth(ParentSequence));
//
//			auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager(this);
//
//			// Resume parent sequence closing
//			this->ProgressComplete();
//
//			DMS_LOG_SIMPLE(TEXT("==== %s : after apply progress ends ===="),*ParentSequence->GetName());
//		});
//
//		// Run setup child effect sequence.
//		SeqManager->RunSequence(NewSeq);
//	}
//
//	else {
//		//Complete this step.
//		ProgressComplete();
//	}
//}