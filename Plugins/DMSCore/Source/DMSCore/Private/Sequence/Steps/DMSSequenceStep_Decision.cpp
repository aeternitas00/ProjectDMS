// Fill out your copyright notice in the Description page of Project Settings.


#include "Sequence/Steps/DMSSequenceStep_Decision.h"
#include "Sequence/DMSSeqManager.h"
#include "Common/DMSTargetGenerator.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Effect/DMSEffectDefinition.h"
#include "Effect/DMSEffectorOwnableInterface.h"
#include "Effect/DMSEffectHandler.h"
#include "Effect/DMSEffectInstance.h"
#include "Player/DMSPlayerControllerBase.h"
#include "GameFramework/PlayerController.h"
#include "Selector/DMSDecisionWidget.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Step_Decision, "Step.Decision");

UDMSSequenceStep_Decision::UDMSSequenceStep_Decision()
{
	StepTag = TAG_DMS_Step_Decision;
	DecisionMaker = CreateDefaultSubobject<UDMSTargetGenerator_SourcePlayer>("DecisionMaker");
}

//TArray<UDMSConfirmWidgetBase*> UDMSSequenceStep_Decision::CreateDecisionWidgets(APlayerController* WidgetOwner)
//{
//	TArray<UDMSDecisionWidget*> rv;
//	for (auto& WidgetClass : DecisionWidgetClasses)
//	{
//		auto NewWidget = CreateWidget<UDMSDecisionWidget>(WidgetOwner, WidgetClass);
//		NewWidget->CurrentSequence = OwnerSequence;
//		rv.Add(NewWidget);
//	}
//
//	//InitializeDecisionWidget(rv);
//	return rv;
//}

void UDMSSequenceStep_Decision::OnBefore_Implementation()
{
	// Behavior
	auto SM = UDMSCoreFunctionLibrary::GetDMSSequenceManager(); check(SM);

	DMS_LOG_SCREEN(TEXT("==-- DecisionStep_BEFORE [ Depth : %d ] --=="), SM->GetDepth(OwnerSequence));
	ProgressComplete();
}

void UDMSSequenceStep_Decision::OnDuring_Implementation()
{
	// Behavior

	auto SM = UDMSCoreFunctionLibrary::GetDMSSequenceManager(); check(SM);
	auto EH = UDMSCoreFunctionLibrary::GetDMSEffectHandler(); check(EH);

	DMS_LOG_SCREEN(TEXT("==-- DecisionStep_DURING [ Depth : %d ] --=="), SM->GetDepth(OwnerSequence));
	
	ADMSPlayerControllerBase* WidgetOwner = nullptr;

	auto DecisionMakers = DecisionMaker->GetTargets(OwnerSequence->GetSourceObject(), OwnerSequence);
	
	// 디시전을 여러명이서 순차 진행하는 케이스가 있을까? 
	if (DecisionMakers[0]->Implements<UDMSEffectorInterface>()) 
		WidgetOwner = Cast<IDMSEffectorInterface>(DecisionMakers[0])->GetOwningPlayerController();

	if ( WidgetOwner == nullptr || !WidgetOwner->SetupWidgetQueue(Decisions, OwnerSequence)) {
		ProgressComplete(false); return;
	}

	RunWidgetQueue(WidgetOwner, [=](UDMSSequence* pSequence) {

		if (!WidgetOwner->SetupWidgetQueue(OwnerSequence->OriginalEffectNode->GetSelectionFormsFromEffects(), OwnerSequence)) {
			ProgressComplete(false); return;
		}

		RunWidgetQueue(WidgetOwner, [=](UDMSSequence* pSequenceN) {

			// Run sequence ( Notifying steps and apply )
			DMS_LOG_SIMPLE(TEXT("==== %s : EI Data Selection Completed  ===="), *pSequenceN->GetName());

			// Prevent self notifing 
			for (auto EI : pSequenceN->GetAllEIs())
			{
				EI->ChangeEIState(EDMSEIState::EIS_Default);
			}

			ProgressComplete();
		});

	});


	// LEGACY 

	//TArray<UDMSConfirmWidgetBase*> Widgets(OwnerSequence->OriginalEffectNode->CreateDecisionWidgets(OwnerSequence, WidgetOwner));
	
	//if (!OwnerSequence->OriginalEffectNode->bForced && DefaultYNWidget != nullptr)
	//	Widgets.Add(NewObject<UDMSConfirmWidgetBase>(this, DefaultYNWidget.Get()));
	//Widgets.Append(TArray<UDMSConfirmWidgetBase*>(OwnerSequence->OriginalEffectNode->CreateDecisionWidgets(OwnerSequence,WidgetOwner)));
	//if (!OwnerSequence->SetupWidgetQueue(Widgets))
	//{
	//	DMS_LOG_SIMPLE(TEXT("Can't setup Decision selector"));
	//	ProgressComplete(false);
	//	//SM->CompleteSequence(OwnerSequence);
	//	return;
	//}

	//OwnerSequence->RunWidgetQueue(
	//	[=](UDMSSequence* pSequence) {
	//		// Decision confirmed or no decision widget

	//		// ====== Effect Data Selection Step ====== //
	//		// ( ex. User set value of effect's damage amount , ... )
	//		if (!pSequence->SetupWidgetQueue(TArray<UDMSConfirmWidgetBase*>(pSequence->OriginalEffectNode->CreateSelectors(pSequence, WidgetOwner))))
	//		{
	//			DMS_LOG_SIMPLE(TEXT("Can't setup Effect selector"));
	//			ProgressComplete(false);
	//			//SM->CompleteSequence(pSequence);
	//			return;
	//		}

	//		pSequence->RunWidgetQueue(
	//			[=](UDMSSequence* pSequenceN) {
	//				// Selection completed

	//				// Run sequence ( Notifying steps and apply )
	//				DMS_LOG_SIMPLE(TEXT("==== %s : EI Data Selection Completed  ===="), *pSequenceN->GetName());

	//				// Prevent self notifing 
	//				for (auto EI : pSequenceN->GetAllEIs())
	//				{
	//					EI->ChangeEIState(EDMSEIState::EIS_Default);
	//				}
	//				
	//				ProgressComplete();

	//			},
	//			[=](UDMSSequence* pSequenceN) {
	//				// Selection Canceled
	//				DMS_LOG_SIMPLE(TEXT("Selection Canceled"));
	//				ProgressComplete(false);
	//				//SM->CompleteSequence(pSequenceN);
	//				// Cleanup this seq
	//			} // Runwidgetqueue end.
	//			);
	//	},

	//	[=](UDMSSequence* pSequence) {
	//		// Decision canceled
	//		DMS_LOG_SIMPLE(TEXT("Decision canceled"));
	//		ProgressComplete(false);
	//	}
	//);

	////ProgressComplete();
}


void UDMSSequenceStep_Decision::OnAfter_Implementation()
{
	// Behavior
	auto SM = UDMSCoreFunctionLibrary::GetDMSSequenceManager(); check(SM);
	DMS_LOG_SCREEN(TEXT("==-- DecisionStep_AFTER [ Depth : %d ] --=="), SM->GetDepth(OwnerSequence));
	ProgressComplete();
}
