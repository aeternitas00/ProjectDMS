#include "Conditions/DMSObjectConditionBase.h"
#include "Sequence/DMSSequence.h"
#include "Effect/DMSEffectInstance.h"

UDMSObjectConditionBase::UDMSObjectConditionBase() 
{
}

TArray<UObject*> UDMSObjectConditionBase::GetCompareTarget(UObject* Caller, ADMSSequence* iSeq, const UDMSTargetGenerator* TargetGenerator) const
{
	TArray<UObject*> Rv;
	
	Rv = TargetGenerator == nullptr ? TArray<UObject*>() : TargetGenerator->GetTargets(Caller, iSeq);

	return Rv;
}

bool UDMSObjectConditionBase::CheckOperation_Implementation(UObject* CheckingGameObject, ADMSSequence* CurrentSequence) const
{
	auto CheckingObjects = GetCompareTarget(CheckingGameObject, CurrentSequence, CompareTargetGenerator);
	bool outResult = bAllObjectMustPassed;

	for (auto CheckingObject : CheckingObjects) {

		UpdateResult(outResult, SingleCheckCondition(CheckingGameObject, CurrentSequence, CheckingObject));

		if (outResult != bAllObjectMustPassed) return outResult;
	}

	return outResult;
}

bool UDMSObjectClassCondition::SingleCheckCondition_Implementation(UObject* CheckingGameObject, ADMSSequence* CurrentSequence, UObject* Target) const
{
	return OnlyExact ? Target->GetClass() == Class : Target->IsA(Class);
}
