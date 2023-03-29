#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *	State checking part of Effect's condition.
 *
 * =========================================
 */

#include "Conditions/DMSConditionObject.h"
#include "Common/DMSCommons.h"
#include "DMSObjectCondition.generated.h"

class UDMSSequence;

/**
 *	========================================
 *
 *	A simple container that collect single conditions and compose them like commonly used boolean expressions.
 *
 *	========================================
 */
UCLASS(Abstract, Blueprintable, BlueprintType, Const,EditInlineNew, ClassGroup = (Condition), meta = (DisplayName = "State Condition Base"))
class PROJECTDMSGAME_API UDMSObjectConditionBase : public UDMSConditionObject
{
	GENERATED_BODY()

public:
	UDMSObjectConditionBase(){}

	UPROPERTY(EditDefaultsOnly, Category = Condition)
	EDMSObjectSelectorFlag TargetFlag;

	UPROPERTY(EditDefaultsOnly, Category = Condition)
	bool bAllObjectMustPassed;

	TArray<UObject*> GetCompareTarget(UObject* Caller, UDMSSequence* iSeq) const;

	virtual bool CheckCondition(UObject* Caller, UDMSSequence* iSeq) const;

	virtual bool CheckCondition_Single(UObject* Caller, UDMSSequence* iSeq, UObject* Target) const {
		return bNullIsTrue;
	}

	void UpdateResult(bool& outResult,const bool& NewValue) const {
		outResult = bAllObjectMustPassed ? outResult && NewValue : outResult || NewValue;
	}

	void UpdateResultWhenNull(bool& outResult) const{
		UpdateResult(outResult, bAllObjectMustPassed);
	}
};


// ========================================= //
// =====  SIMPLE SAMPLE OF IMPLEMENTS  ===== //
// ========================================= //

UCLASS(BlueprintType)
class PROJECTDMSGAME_API UDMSObjectAttributeCondition : public UDMSObjectConditionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Attribute)
	FGameplayTag AttributeTag;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Condition)
	EDMSComparisonOperator Operator;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Condition)
	int Value; // float? 

	virtual bool CheckCondition_Single(UObject* Caller, UDMSSequence* iSeq, UObject* Target) const;
};
