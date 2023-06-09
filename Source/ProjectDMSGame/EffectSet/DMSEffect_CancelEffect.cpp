// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectSet/DMSEffect_CancelEffect.h"
#include "Sequence/DMSSequence.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Effect_CancelEffect, "Effect.CancelEffect");
UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Effect_IgnoreEffect, "Effect.IgnoreEffect");
//UDMSEffect_CancelEffect::UDMSEffect_CancelEffect()
//{
//}

void UDMSEffect_CancelEffect::Work_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnWorkCompleted& OnWorkCompleted)
{
	DMS_LOG_SIMPLE(TEXT("CancelEffect Working"));
	auto SearchSeq = SourceSequence->ParentSequence;
	while (SearchSeq !=nullptr){
		if (SearchSeq->OriginalEffectNode->ExecuteTagQuery(CancelTargetTagQuery)) {
			SearchSeq->SequenceState = EDMSSequenceState::SS_Canceled;
			break;
		}
		SearchSeq = SearchSeq->ParentSequence;
	}
	
	OnWorkCompleted.ExecuteIfBound(SourceSequence,true);
}

void UDMSEffect_IgnoreEffect::Work_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnWorkCompleted& OnWorkCompleted)
{
	auto SearchSeq = SourceSequence->ParentSequence;
	while (SearchSeq != nullptr) {
		if (SearchSeq->OriginalEffectNode->ExecuteTagQuery(IgnoreTargetTagQuery)) {
			if (IgnoreWholeSequence) 
				SearchSeq->SequenceState = EDMSSequenceState::SS_Ignored;
			else
				SearchSeq->EIDatas->SetData(TAG_DMS_Effect_IgnoreEffect, IgnoreTargetTagQuery);
			break;
		}
		SearchSeq = SearchSeq->ParentSequence;
	}
	
	OnWorkCompleted.ExecuteIfBound(SourceSequence,true);
}
