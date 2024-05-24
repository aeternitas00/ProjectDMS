#include "Conditions/DMSSequenceCondition.h"
#include "Sequence/DMSSequence.h"
#include "Effect/DMSEffectDefinition.h"
#include "Sequence/DMSSequenceStep.h"
#include "Common/DMSCommons.h"


bool UDMSSeqTimingCondition::CheckOperation_Implementation(UObject* CheckingGameObject, ADMSSequence* CurrentSequence) const
{
	//return (CurrentSequence->GetCurrentProgress() == Timing || Timing == EDMSTimingFlag::T_Null)
	//	&& CurrentSequence->GetSequenceTags().MatchesQuery(EffectTagQuery)
	//;
	//auto debugcont = CurrentSequence->GetSequenceTags();
	//DMS_LOG_SIMPLE(TEXT("%s"),*debugcont.ToString());
	return CurrentSequence->GetSequenceTags().MatchesQuery(EffectTagQuery);
}

bool UDMSSeqStateCondition::CheckOperation_Implementation(UObject* CheckingGameObject, ADMSSequence* CurrentSequence) const
{
	ADMSSequence* CheckingSeq = CheckParent ? CurrentSequence->ParentSequence.Get() : CurrentSequence;

	return SuccessCondition.Contains(CheckingSeq->SequenceState);
}
