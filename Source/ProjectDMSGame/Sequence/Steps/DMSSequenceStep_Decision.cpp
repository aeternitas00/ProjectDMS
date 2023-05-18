// Fill out your copyright notice in the Description page of Project Settings.


#include "Sequence/Steps/DMSSequenceStep_Decision.h"

//void UDMSSequenceStep_Decision::OnBefore_Implementation()
//{
//	// Behavior
//	ProgressComplete();
//}

void UDMSSequenceStep_Decision::OnDuring_Implementation()
{
	// Behavior

	//TArray<UDMSConfirmWidgetBase*> Widgets;
	////if (!iSeq->OriginalEffectNode->bForced && DefaultYNWidget != nullptr)
	////	Widgets.Add(NewObject<UDMSConfirmWidgetBase>(this, DefaultYNWidget.Get()));
	//Widgets.Append(TArray<UDMSConfirmWidgetBase*>(iSeq->OriginalEffectNode->CreateDecisionWidgets(WidgetOwner)));
	//if (!iSeq->SetupWidgetQueue(Widgets))
	//{
	//	DMS_LOG_SIMPLE(TEXT("Can't setup Decision selector"));
	//	CompleteSequence(iSeq);
	//	return;
	//}

	//iSeq->RunWidgetQueue(
	//	[&, WidgetOwner](UDMSSequence* pSequence) {
	//		// Decision confirmed or no decision widget

	//		// Create EI first for preview.
	//		UDMSCoreFunctionLibrary::GetDMSEffectHandler()->CreateEffectInstance(pSequence, pSequence->OriginalEffectNode);

	//		// ====== Effect Data Selection Step ====== //
	//		// ( ex. User set value of effect's damage amount , ... )
	//		if (!pSequence->SetupWidgetQueue(TArray<UDMSConfirmWidgetBase*>(pSequence->OriginalEffectNode->CreateSelectors(pSequence, WidgetOwner))))
	//		{
	//			DMS_LOG_SIMPLE(TEXT("Can't setup Effect selector"));
	//			CompleteSequence(pSequence);
	//			return;
	//		}

	//		pSequence->RunWidgetQueue(
	//			[this](UDMSSequence* pSequenceN) {
	//				// Selection completed

	//				// Run sequence ( Notifying steps and apply )
	//				DMS_LOG_SIMPLE(TEXT("==== %s : EI Data Selection Completed  ===="), *pSequenceN->GetName());
	//				ApplySequence(pSequenceN);

	//				// Prevent self notifing 
	//				for (auto EI : pSequenceN->EIs)
	//				{
	//					EI->ChangeEIState(EDMSEIState::EIS_Default);
	//				}
	//			},
	//			[this](UDMSSequence* pSequenceN) {
	//				// Selection Canceled
	//				DMS_LOG_SIMPLE(TEXT("Selection Canceled"));
	//				CompleteSequence(pSequenceN);
	//				// Cleanup this seq
	//			} // Runwidgetqueue end.
	//			);
	//	},

	//	[this](UDMSSequence* pSequence) {
	//		// Decision canceled
	//		DMS_LOG_SIMPLE(TEXT("Decision canceled"));
	//		// Cleanup this seq
	//		CompleteSequence(pSequence);
	//		//...
	//	}
	//	);
	ProgressComplete();
}

//void UDMSSequenceStep_Decision::OnAfter_Implementation()
//{
//	// Behavior
//	ProgressComplete();
//}
