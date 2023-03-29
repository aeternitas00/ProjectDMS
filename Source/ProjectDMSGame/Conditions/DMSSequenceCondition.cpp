#include "Conditions/DMSSequenceCondition.h"
#include "Sequence/DMSSequence.h"
#include "Effect/DMSEffectDefinition.h"
#include "Common/DMSCommons.h"


bool UDMSSeqTimingCondition::CheckCondition(UObject* Caller, UDMSSequence* iSeq) const
{
	return (iSeq->Progress == Timing || Timing == EDMSTimingFlag::T_Null)
		&& iSeq->OriginalEffectNode->ExecuteTagQuery(EffectTagQuery)
	;
}

