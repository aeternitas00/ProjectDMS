#include "Conditions/DMSConditionObject.h"


bool UDMSConditionObject::CheckOperation_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence) const
{
	return bNullIsTrue;
}

bool UDMSConditionClassWrapper::CheckOperation_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence) const
{
	return Condition.GetDefaultObject()->CheckCondition(CheckingGameObject,  CurrentSequence);
}

bool UDMSConditionCombiner::CheckOperation_Implementation(UObject* CheckingGameObject,  UDMSSequence* CurrentSequence) const
{
	if (Conditions.Num() == 0) 
		return bEmptyIsTrue;

	bool outResult = bIsAnd;

	for (auto CO : Conditions)
	{
		bool Value = CO->CheckCondition(CheckingGameObject,  CurrentSequence);
		
		outResult = bIsAnd ? outResult && Value : outResult || Value;
		// No need to check others (no need to check relative conditions).
		if (outResult != bIsAnd) return outResult;
		
	}

	return outResult;
}




//=============TEST===============//

const UDMSConditionObjectBase* UDMSConditionClassWrapper::CheckCondition_(UObject* Caller, UDMSSequence* iSeq, bool& outResult) const
{
	return Condition.GetDefaultObject()->CheckCondition_(Caller, iSeq, outResult);
}

const UDMSConditionObjectBase* UDMSConditionObject::CheckCondition_(UObject* Caller, UDMSSequence* iSeq, bool& outResult) const
{ 
	outResult = true; return bIsRelative ? this : nullptr; 
}

const UDMSConditionObjectBase* UDMSConditionCombiner::CheckCondition_(UObject* Caller, UDMSSequence* iSeq, bool& outResult) const
{
	if (Conditions.Num() == 0) {
		outResult = bEmptyIsTrue;
		return nullptr;
	}
	
	outResult = bIsAnd;

	UDMSConditionCombiner* Rv = DuplicateObject<UDMSConditionCombiner>(this,GetOuter());

	for (int idx = Conditions.Num(); idx>=0 ; idx--)
	{
		auto CO = Conditions[idx];
		bool Value;
		auto RelativeCO = CO->CheckCondition_(Caller,iSeq,Value);
		if (RelativeCO != nullptr) {
			Rv->Conditions.RemoveAt(idx);
		}
		else {
			outResult = bIsAnd ? outResult && Value : outResult || Value;
			// No need to check others (no need to check relative conditions).
			if (outResult != bIsAnd) return nullptr;
		}
	}

	return Rv->Conditions.Num() != 0 ? Rv : nullptr;
}
