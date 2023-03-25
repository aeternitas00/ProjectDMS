#include "Common/DMSConditionObject.h"
#include "DMSConditionObject.h"

bool UDMSConditionClassWrapper::CheckCondition(UDMSSequence* iSeq) const
{
	return Condition.GetDefaultObject()->CheckCondition(iSeq);
}

bool UDMSConditionClassWrapper::CheckCondition(UObject* Caller, UDMSSequence* iSeq) const
{
	return Condition.GetDefaultObject()->CheckCondition(Caller, iSeq);
}

bool UDMSConditionCombiner::CheckCondition(UDMSSequence* iSeq) const
{ 
	bool rv = bIsAnd;

	if (Conditions.Num() == 0)return bEmptyTimingIsTrue;

	for (auto Condition : Conditions)
	{
		rv = bIsAnd ? rv && Condition->CheckCondition(iSeq) : rv || Condition->CheckCondition(iSeq);
	}

	return rv;
}

bool UDMSConditionCombiner::CheckCondition(UObject* Caller, UDMSSequence* iSeq) const
{ 
	bool rv = bIsAnd;

	if (Conditions.Num()==0) return bEmptyTimingIsTrue;

	for (auto Condition : Conditions)
	{
		rv = bIsAnd ? rv && Condition->CheckCondition(Caller,iSeq) : rv || Condition->CheckCondition(Caller,iSeq);
	}

	return rv;
}

