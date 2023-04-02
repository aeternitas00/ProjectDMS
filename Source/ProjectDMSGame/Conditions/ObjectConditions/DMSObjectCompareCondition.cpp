#include "Conditions/ObjectConditions/DMSObjectCompareCondition.h"
#include "DMSObjectCompareCondition.h"

bool UDMSObjectCompareCondition::CheckOperation_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence) const
{
	auto SourceObjects = GetCompareTarget(CheckingGameObject, CurrentSequence, SourceFlag);
	auto TargetObjects = GetCompareTarget(CheckingGameObject, CurrentSequence, TargetFlag);
	bool outResult = bAllObjectMustPassed;

	for (auto SourceObject : SourceObjects) {
		bool currentResult= false;
		for (auto TargetObject : TargetObjects){
			currentResult = SingleCheckCondition(SourceObject, CurrentSequence, TargetObject);
			if (currentResult) break;
		}
		UpdateResult(outResult, currentResult);
		if (bAllObjectMustPassed != outResult) return outResult;
	}

	return outResult;
}

bool UDMSObjectCompareCondition::SingleCheckCondition_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence, UObject* Target) const
{
	return Comparer->Compare(CheckingGameObject, CurrentSequence, Target);
}

