#include "Conditions/DMSSequenceCondition.h"
#include "Sequence/DMSSequence.h"
#include "Effect/DMSEffectDefinition.h"

#include "Common/DMSCommons.h"


bool UDMSSeqTimingCondition::CheckCondition_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence) const
{
	return (CurrentSequence->Progress == Timing || Timing == EDMSTimingFlag::T_Null)
		&& CurrentSequence->OriginalEffectNode->ExecuteTagQuery(EffectTagQuery)
	;
}


