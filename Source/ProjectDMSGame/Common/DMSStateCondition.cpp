#include "Common/DMSStateCondition.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Attribute/DMSAttribute.h"
#include "Sequence/DMSSequence.h"


bool UDMSStateCondition::CheckCondition(UDMSSequence* Seq)
{ 
	if(StateConditions.Num()==0) return true;

	TArray<bool> Values;

	bool tempRv=true;
	bool tempVal=true;

	for (auto State : StateConditions)
	{
		switch (State->CheckerTargetFlag)
		{
			case EDMSObjectSelectorFlag::OSF_SourceObj:
				tempVal = State->CheckState(Seq->SourceObject);
				break;
			case EDMSObjectSelectorFlag::OSF_SourceCtr:
				tempVal = State->CheckState(Seq->SourceController);
				break;
			case EDMSObjectSelectorFlag::OSF_Target:
				for (auto InterfaceScript : Seq->Targets)
					tempVal = tempVal && State->CheckState(InterfaceScript.GetObject());
				break;
			case EDMSObjectSelectorFlag::OSF_Data:
				tempVal = State->CheckState(Seq->EIDatas);
				break;
			case EDMSObjectSelectorFlag::OSF_Custom:
				tempVal = true;
				//Values.Add(State.CheckState(Seq->Target, Seq));
				break;
			default:	break;
		}
		//test
		tempRv = tempRv && tempVal;

		Values.Add(tempVal);
	}
	//return Combinator ( Values );
	return tempRv;
}

bool UDMSAttributeCheckerDefinition::CheckState(UObject* CheckingObject)
{
	AActor* tOuter = Cast<AActor>(CheckingObject);
	if (tOuter == nullptr) {
		DMS_LOG_SCREEN(TEXT("%s : Outer (%s) is not actor"), *GetName(), *CheckingObject->GetName());
		return bNullIsTrue;
	}
	UDMSAttributeComponent* AttComp = Cast<UDMSAttributeComponent>(tOuter->GetComponentByClass(UDMSAttributeComponent::StaticClass()));

	if (AttComp==nullptr)return bNullIsTrue;

	UDMSAttribute* Att = AttComp->GetAttribute(AttributeName);

	if (Att == nullptr)return bNullIsTrue;

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
		default : return true;
	}
}

bool UDMSStateConditionWrapper_Manual::CheckCondition(UDMSSequence* iSeq) const
{
	return Condition->CheckCondition(iSeq);
}

bool UDMSStateConditionWrapper_Preset::CheckCondition(UDMSSequence* iSeq) const
{
	return Condition.GetDefaultObject()->CheckCondition(iSeq);
}

template<typename T>
bool UDMSDataCheckerDefinition::CheckState_Internal(UObject* Target, const T& Value)
{
	UDMSDataObjectSet* DataSet = Cast<UDMSDataObjectSet>(Target);
	if (DataSet == nullptr) return bNullIsTrue;

	UDMSDataObject* DataObj = DataSet->GetData(DataName);
	if (DataObj == nullptr) return bNullIsTrue;

	if (!DataObj->TypeCheck<T>()) return false;
	return DataObj->Get<T>() == Value ;
}

bool UDMSDataCheckerDefinition_Numeric::CheckState(UObject* Target)
{ 
	return CheckState_Internal(Target, Data);
}

bool UDMSDataCheckerDefinition_Name::CheckState(UObject* Target)
{ 
	return CheckState_Internal(Target, Data);
}
