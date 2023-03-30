#include "Conditions/ObjectConditions/DMSObjectCompareCondition.h"
#include "DMSObjectCompareCondition.h"

bool UDMSObjectCompareCondition::CheckCondition(UObject* Caller, UDMSSequence* iSeq) const
{
	auto SourceObjects = GetCompareTarget(Caller, iSeq, SourceFlag);
	auto TargetObjects = GetCompareTarget(Caller, iSeq, TargetFlag);
	bool outResult = bAllObjectMustPassed;

	for (auto SourceObject : SourceObjects) {
		bool currentResult= false;
		for (auto TargetObject : TargetObjects){
			currentResult = CheckCondition_Single(SourceObject, iSeq, TargetObject);
			if (currentResult) break;
		}
		UpdateResult(outResult, currentResult);
		if (bAllObjectMustPassed != outResult) return outResult;
	}

	return outResult;
}

bool UDMSObjectCompareCondition::CheckCondition_Single(UObject* Caller, UDMSSequence* iSeq, UObject* Target) const
{
	return Comparer->Compare(Caller, iSeq, Target);
}
