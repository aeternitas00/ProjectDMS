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

	TArray<UObject*> GetCompareTarget(UObject* CheckingGameObject, UDMSSequence* CurrentSequence, const EDMSObjectSelectorFlag& iTargetFlag) const;

	virtual bool CheckOperation_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	bool SingleCheckCondition(UObject* CheckingGameObject, UDMSSequence* CurrentSequence, UObject* Target) const;
	virtual bool SingleCheckCondition_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence, UObject* Target) const {
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
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Condition)
	FGameplayTag AttributeTag;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Condition)
	EDMSComparisonOperator Operator;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Condition)
	int Value; // float? 

	virtual bool SingleCheckCondition_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence, UObject* Target) const;
};

//
//UCLASS(Blueprintable, BlueprintType, Const, EditInlineNew, ClassGroup = (Condition), meta = (DisplayName = "CO No self trigger"))
//class PROJECTDMSGAME_API UDMSNoSelfTriggerCondition : public UDMSConditionObjectBase
//{
//	GENERATED_BODY()
//
//public:
//
//	UPROPERTY(EditDefaultsOnly, Category = Condition)
//	bool bAllObjectMustPassed;
//
//	virtual bool CheckOperation_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence) const;
//};
