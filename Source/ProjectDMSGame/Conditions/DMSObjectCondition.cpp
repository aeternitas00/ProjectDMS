#include "Conditions/DMSObjectCondition.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Attribute/DMSAttribute.h"
#include "Sequence/DMSSequence.h"
#include "DMSObjectCondition.h"

TArray<UObject*> UDMSObjectConditionBase::GetCompareTarget(UObject* Caller, UDMSSequence* iSeq) const
{
	TArray<UObject*> Rv;
	switch (TargetFlag)
	{
	case EDMSObjectSelectorFlag::OSF_Default:
		Rv.Add(Caller);
		break;
	case EDMSObjectSelectorFlag::OSF_SourceObj:
		Rv.Add(iSeq->SourceObject);
		break;
	case EDMSObjectSelectorFlag::OSF_SourceCtr:
		Rv.Add(iSeq->SourceController);
		break;
	case EDMSObjectSelectorFlag::OSF_Target:
		for (auto InterfaceScript : iSeq->Targets)
			Rv.Add(InterfaceScript.GetObject());
		break;
	case EDMSObjectSelectorFlag::OSF_Data:
	case EDMSObjectSelectorFlag::OSF_Custom:
		//Values.Add(State.CheckState(Seq->Target, Seq));
		break;
	default:	break;
	}

	return Rv;
}

bool UDMSObjectConditionBase::CheckCondition(UObject* Caller, UDMSSequence* iSeq) const
{
	auto CheckingObjects = GetCompareTarget(Caller, iSeq);
	bool outResult = bAllObjectMustPassed;

	for (auto CheckingObject : CheckingObjects) {

		UpdateResult(outResult, CheckCondition_Single(Caller, iSeq, CheckingObject));

		if (outResult != bAllObjectMustPassed) return outResult;
	}

	return outResult;
}

bool UDMSObjectAttributeCondition::CheckCondition_Single(UObject* Caller, UDMSSequence* iSeq, UObject* Target) const
{
	AActor* tOuter = Cast<AActor>(Target);
	if (tOuter == nullptr) {
		DMS_LOG_SCREEN(TEXT("%s : Outer (%s) is not actor"), *GetName(), *Target->GetName());
		return bNullIsTrue;
	}
	UDMSAttributeComponent* AttComp = Cast<UDMSAttributeComponent>(tOuter->GetComponentByClass(UDMSAttributeComponent::StaticClass()));

	if (AttComp == nullptr) { return bNullIsTrue; }

	UDMSAttribute* Att = AttComp->GetAttribute(AttributeTag);

	if (Att == nullptr) { return bNullIsTrue; }

	switch (Operator)
	{
		case EDMSComparisonOperator::BO_Equal:
			return Att->GetValue() == Value;
		case EDMSComparisonOperator::BO_Greater:
			return Att->GetValue() > Value;
		case EDMSComparisonOperator::BO_Less:
			return Att->GetValue() < Value;
		case EDMSComparisonOperator::BO_GreaterEqual:
			return Att->GetValue() >= Value;
		case EDMSComparisonOperator::BO_LessEqual:
			return Att->GetValue() <= Value;
		default: return bNullIsTrue;
	}

	//return bNullIsTrue;
}
