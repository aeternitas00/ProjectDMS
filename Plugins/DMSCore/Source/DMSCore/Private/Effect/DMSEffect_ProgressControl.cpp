// Copyright JeongWoo Lee

#include "Effect/DMSEffect_ProgressControl.h"
#include "Effect/DMSEffectInstance.h"
#include "Sequence/DMSSequence.h"
#include "Sequence/DMSSequenceStep.h"

void UDMSEffect_ProgressControl::Work_Implementation(ADMSSequence* SourceSequence, ADMSActiveEffect* iEI, const FOnExecuteCompleted& OnWorkCompleted)
{
	auto TargetSeq = Cast<ADMSSequence>(iEI->GetApplyTarget());
	if(!TargetSeq){	OnWorkCompleted.ExecuteIfBound(false); return;}

	auto Step = TargetSeq->GetInstancedStep();

	if(bTargetByIndex)
		Step->SetNextStep(JumpTargetIdx);
	else
		Step->SetNextStep(JumpTargetTag);

	OnWorkCompleted.ExecuteIfBound(true);
}
