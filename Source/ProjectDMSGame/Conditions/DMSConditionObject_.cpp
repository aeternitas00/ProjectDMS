#include "Conditions/DMSConditionObject_.h"

bool UDMSConditionClassWrapper_::CheckCondition(UObject* Caller, UDMSSequence* iSeq) const
{
	return Condition.GetDefaultObject()->CheckCondition(Caller, iSeq);
}

bool UDMSConditionObject_::CheckCondition(UObject* Caller, UDMSSequence* iSeq) const
{
	return bNullIsTrue;
}

bool UDMSConditionCombiner_::CheckCondition(UObject* Caller, UDMSSequence* iSeq) const
{
	if (Conditions.Num() == 0) 
		return bEmptyIsTrue;

	bool outResult = bIsAnd;

	for (auto CO : Conditions)
	{
		bool Value = CO->CheckCondition(Caller, iSeq);
		
		outResult = bIsAnd ? outResult && Value : outResult || Value;
		// No need to check others (no need to check relative conditions).
		if (outResult != bIsAnd) return outResult;
		
	}

	return outResult;
}




//=============TEST===============//

const UDMSConditionObjectBase* UDMSConditionClassWrapper_::CheckCondition_(UObject* Caller, UDMSSequence* iSeq, bool& outResult) const
{
	return Condition.GetDefaultObject()->CheckCondition_(Caller, iSeq, outResult);
}

const UDMSConditionObjectBase* UDMSConditionObject_::CheckCondition_(UObject* Caller, UDMSSequence* iSeq, bool& outResult) const
{ 
	outResult = true; return bIsRelative ? this : nullptr; 
}

const UDMSConditionObjectBase* UDMSConditionCombiner_::CheckCondition_(UObject* Caller, UDMSSequence* iSeq, bool& outResult) const
{
	if (Conditions.Num() == 0) {
		outResult = bEmptyIsTrue;
		return nullptr;
	}
	
	outResult = bIsAnd;

	UDMSConditionCombiner_* Rv = DuplicateObject<UDMSConditionCombiner_>(this,GetOuter());

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
