#include "Conditions/ObjectConditions/DMSObjectCompareCondition.h"
#include "DMSObjectCompareCondition.h"

bool UDMSObjectCompareCondition::CheckCondition_Single(UObject* Caller, UDMSSequence* iSeq, UObject* Target) const
{
	return Comparer->Compare(Caller, iSeq, Target);
}
