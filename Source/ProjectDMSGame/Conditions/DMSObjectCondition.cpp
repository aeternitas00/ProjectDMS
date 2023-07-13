#include "Conditions/DMSObjectCondition.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Attribute/DMSAttribute.h"
#include "Sequence/DMSSequence.h"
#include "Effect/DMSEffectInstance.h"
#include "DMSObjectCondition.h"

UDMSObjectConditionBase::UDMSObjectConditionBase() 
{
}

TArray<UObject*> UDMSObjectConditionBase::GetCompareTarget(UObject* Caller, UDMSSequence* iSeq, const UDMSTargetGenerator* TargetGenerator) const
{
	TArray<UObject*> Rv;
	
	Rv = TargetGenerator == nullptr ? TArray<UObject*>() : TargetGenerator->GetTargets(Caller, iSeq);

	return Rv;
}

bool UDMSObjectConditionBase::CheckOperation_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence) const
{
	auto CheckingObjects = GetCompareTarget(CheckingGameObject, CurrentSequence, CompareTargetGenerator);
	bool outResult = bAllObjectMustPassed;

	for (auto CheckingObject : CheckingObjects) {

		UpdateResult(outResult, SingleCheckCondition(CheckingGameObject, CurrentSequence, CheckingObject));

		if (outResult != bAllObjectMustPassed) return outResult;
	}

	return outResult;
}

bool UDMSObjectAttributeCondition::SingleCheckCondition_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence, UObject* Target) const
{
	AActor* tOuter = Cast<AActor>(Target);
	if (tOuter == nullptr) {
		//DMS_LOG_SCREEN(TEXT("%s : Outer (%s) is not actor"), *GetName(), *Target->GetName());
		return bNullIsTrue;
	}
	UDMSAttributeComponent* AttComp = Cast<UDMSAttributeComponent>(tOuter->GetComponentByClass(UDMSAttributeComponent::StaticClass()));

	if (AttComp == nullptr) { return bNullIsTrue; }

	UDMSAttribute* Att = AttComp->GetAttribute(AttributeTag);

	if (Att == nullptr) { return bNullIsTrue; }

	bool rv;
	switch (Operator)
	{
		case EDMSComparisonOperator::BO_Equal:
			rv = Att->GetValue() == Value; break;			
		case EDMSComparisonOperator::BO_Greater:
			rv = Att->GetValue() > Value; break;
		case EDMSComparisonOperator::BO_Less:
			rv = Att->GetValue() < Value; break;
		case EDMSComparisonOperator::BO_GreaterEqual:
			rv = Att->GetValue() >= Value; break;
		case EDMSComparisonOperator::BO_LessEqual:
			rv = Att->GetValue() <= Value; break;
		default: return bNullIsTrue;
	}

	return rv;
}

bool UDMSObjectClassCondition::SingleCheckCondition_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence, UObject* Target) const
{
	return OnlyExact ? Target->GetClass() == Class : Target->IsA(Class);
}
