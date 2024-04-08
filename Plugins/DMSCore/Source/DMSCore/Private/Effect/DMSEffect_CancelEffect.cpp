// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/DMSEffect_CancelEffect.h"
#include "Sequence/DMSSequence.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Effect_CancelEffect, "Effect.CancelEffect");
UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Effect_IgnoreEffect, "Effect.IgnoreEffect");
//UDMSEffect_CancelEffect::UDMSEffect_CancelEffect()
//{
//}

// 올드 소스 : 작동되는지 확인해야함.
void UDMSEffect_CancelEffect::Work_Implementation(UDMSSequence* SourceSequence, ADMSActiveEffect* iEI, const FOnExecuteCompleted& OnWorkCompleted)
{
	DMS_LOG_SIMPLE(TEXT("CancelEffect Working"));
	auto SearchSeq = SourceSequence->ParentSequence;
	while (SearchSeq !=nullptr){
		if (SearchSeq->GetSequenceTags().MatchesQuery(CancelTargetTagQuery)) {
			SearchSeq->SequenceState = EDMSSequenceState::SS_Canceled;
			break;
		}
		SearchSeq = SearchSeq->ParentSequence;
	}
	
	OnWorkCompleted.ExecuteIfBound(true);
}

void UDMSEffect_IgnoreEffect::Work_Implementation(UDMSSequence* SourceSequence, ADMSActiveEffect* iEI, const FOnExecuteCompleted& OnWorkCompleted)
{
	auto SearchSeq = SourceSequence->ParentSequence;
	while (SearchSeq != nullptr) {
		if (SearchSeq->GetSequenceTags().MatchesQuery(IgnoreTargetTagQuery)) {
			if (IgnoreWholeSequence) 
				SearchSeq->SequenceState = EDMSSequenceState::SS_Ignored;
			else
				SearchSeq->SequenceDatas->SetData(TAG_DMS_Effect_IgnoreEffect, IgnoreTargetTagQuery);
			break;
		}
		SearchSeq = SearchSeq->ParentSequence;
	}
	
	OnWorkCompleted.ExecuteIfBound(true);
}
