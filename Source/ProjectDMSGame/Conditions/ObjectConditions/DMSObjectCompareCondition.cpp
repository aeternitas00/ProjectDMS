#include "Conditions/ObjectConditions/DMSObjectCompareCondition.h"
#include "DMSObjectCompareCondition.h"
#include "Location/DMSLocatableInterface.h"

UDMSObjectCompareCondition::UDMSObjectCompareCondition() : UDMSObjectConditionBase() 
{

}

bool UDMSObjectCompareCondition::CheckOperation_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence) const
{
	auto SourceObjects = GetCompareTarget(CheckingGameObject, CurrentSequence, SourceGenerator);
	auto TargetObjects = GetCompareTarget(CheckingGameObject, CurrentSequence, CompareTargetGenerator);
	bool outResult = bAllSourcesMustPassed;
	
	// 'bAllSourcesMustPassed' and 'bAllObjectMustPassed' flag desc )
	//		[switch (bAllObjectMustPassed)]
	//		true	:	All Source object must pass comparing to [switch (bAllObjectMustPassed)]
	//			true	:	All targets
	//			false	:	Any target
	//		false	:	Any Source object must ...
	//			...
	// 
	//		(true && true) return true only if 'All SingleCheckCondition' was true
	//		(false && false) return true if 'Any SingleCheckCondition' was true
	for (auto SourceObject : SourceObjects) {

		bool currentResult = bAllObjectMustPassed;
		for (auto TargetObject : TargetObjects){
			bool NewResult = SingleCheckCondition(SourceObject, CurrentSequence, TargetObject);
			UpdateResult(currentResult, NewResult);
			if (bAllObjectMustPassed != currentResult){ outResult = currentResult; break;}
		}
		outResult = bAllSourcesMustPassed ? outResult && currentResult : outResult || currentResult;
		if (bAllSourcesMustPassed != outResult) return outResult;
	}

	return outResult;
}

bool UDMSObjectCompareCondition::SingleCheckCondition_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence, UObject* Target) const
{
	return Comparer->Compare(CheckingGameObject, CurrentSequence, Target, bNullIsTrue);
}

bool UDMSObjectComparer_IsSameLocation::Compare_Implementation(UObject* SourceObject, UDMSSequence* iSeq, UObject* TargetObject, bool NullIsTrue) {
	DMS_LOG_SIMPLE(TEXT("%s and %s is Same location?"), *SourceObject->GetName(), *TargetObject->GetName());
	
	//DMS_LOG_SCREEN(TEXT("%s and %s is Same location?"), *SourceObject->GetName(), *TargetObject->GetName());
	if (!SourceObject->Implements<UDMSLocatableInterface>() || !TargetObject->Implements<UDMSLocatableInterface>())
		return NullIsTrue;

	return IDMSLocatableInterface::Execute_GetCurrentLocation(SourceObject) == IDMSLocatableInterface::Execute_GetCurrentLocation(TargetObject);
}
