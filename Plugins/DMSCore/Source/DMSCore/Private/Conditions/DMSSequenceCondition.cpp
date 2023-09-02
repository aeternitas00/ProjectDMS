#include "Conditions/DMSSequenceCondition.h"
#include "Sequence/DMSSequence.h"
#include "Effect/DMSEffectDefinition.h"
#include "Sequence/DMSSequenceStep.h"
#include "Common/DMSCommons.h"


bool UDMSSeqTimingCondition::CheckOperation_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence) const
{
	return (CurrentSequence->GetCurrentProgress() == Timing || Timing == EDMSTimingFlag::T_Null)
		&& CurrentSequence->GenerateTagContainer().MatchesQuery(EffectTagQuery)
	;
}

