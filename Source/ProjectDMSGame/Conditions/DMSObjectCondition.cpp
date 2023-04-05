#include "Conditions/DMSObjectCondition.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Attribute/DMSAttribute.h"
#include "Sequence/DMSSequence.h"
#include "Effect/DMSEffectInstance.h"
#include "DMSObjectCondition.h"

TArray<UObject*> UDMSObjectConditionBase::GetCompareTarget(UObject* Caller, UDMSSequence* iSeq, const EDMSObjectSelectorFlag& iTargetFlag) const
{
	TArray<UObject*> Rv;
	switch (iTargetFlag)
	{
	case EDMSObjectSelectorFlag::OSF_Default:
		Rv.Add(Caller);
		break;
	case EDMSObjectSelectorFlag::OSF_OwningPlayer:
		if (Caller->Implements<UDMSEffectorInterface>())
			Rv.Add(Cast<IDMSEffectorInterface>(Caller)->GetOwningPlayer());
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

bool UDMSObjectConditionBase::CheckOperation_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence) const
{
	auto CheckingObjects = GetCompareTarget(CheckingGameObject, CurrentSequence, TargetFlag);
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
		DMS_LOG_SCREEN(TEXT("%s : Outer (%s) is not actor"), *GetName(), *Target->GetName());
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
			rv = Att->GetValue() == Value;
		case EDMSComparisonOperator::BO_Greater:
			rv = Att->GetValue() > Value;
		case EDMSComparisonOperator::BO_Less:
			rv = Att->GetValue() < Value;
		case EDMSComparisonOperator::BO_GreaterEqual:
			rv = Att->GetValue() >= Value;
		case EDMSComparisonOperator::BO_LessEqual:
			rv = Att->GetValue() <= Value;
		default: return bNullIsTrue;
	}

	return rv;
}

//bool UDMSNoSelfTriggerCondition::CheckOperation_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence) const
//{
//	bool rv = false;
//	for (auto EI : CurrentSequence->EIs)
//	{
//		if (CurrentSequence->OriginalEffectNode == EI->EffectNode)
//			rv=true;
//
//	}
//	return rv;
//}
