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
UCLASS(Abstract, Blueprintable, BlueprintType, Const,EditInlineNew, ClassGroup = (Condition))
class PROJECTDMSGAME_API UDMSObjectConditionBase : public UDMSConditionObject
{
	GENERATED_BODY()

public:
	UDMSObjectConditionBase(){}

	/**
	 * Flag that how to get target from checking sequence.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Condition)
	EDMSObjectSelectorFlag TargetFlag;

	/**
	 * If it is true, CheckOperation returns true only if all of object checkings were true
	 */
	UPROPERTY(EditDefaultsOnly, Category = Condition)
	bool bAllObjectMustPassed;



	/**
	 * Get compare target with TargetFlag.
	 * @param	CheckingGameObject
	 * @param	CurrentSequence
	 * @param	iTargetFlag
	 * @return	
	 */
	TArray<UObject*> GetCompareTarget(UObject* CheckingGameObject, UDMSSequence* CurrentSequence, const EDMSObjectSelectorFlag& iTargetFlag) const;

	UFUNCTION(BlueprintNativeEvent)
	TArray<UObject*> GetCustomCompareTarget(UObject* CheckingGameObject, UDMSSequence* CurrentSequence, const EDMSObjectSelectorFlag& iTargetFlag) const;
	TArray<UObject*> GetCustomCompareTarget_Implementation(UObject* Caller, UDMSSequence* iSeq, const EDMSObjectSelectorFlag& iTargetFlag) const;

	virtual bool CheckOperation_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence) const;

	/** 
	 * Checking function will be excuted for each compare targets.
	 * @param	CheckingGameObject
	 * @param	CurrentSequence
	 * @param	Target
	 * return	Result of check condition.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	bool SingleCheckCondition(UObject* CheckingGameObject, UDMSSequence* CurrentSequence, UObject* Target) const;
	virtual bool SingleCheckCondition_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence, UObject* Target) const {
		return bNullIsTrue;
	}

	/**
	 * Used in CheckOperation. Update return value of CheckOperation.
	 */
	void UpdateResult(bool& outResult,const bool& NewValue) const {
		outResult = bAllObjectMustPassed ? outResult && NewValue : outResult || NewValue;
	}

	/**
	 * Used in CheckOperation. Update return value of CheckOperation.
	 */
	void UpdateResultWhenNull(bool& outResult) const{
		UpdateResult(outResult, bNullIsTrue);
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
	/**
	 * Checking attribute's tag.
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Condition)
	FGameplayTag AttributeTag;

	/**
	 * Checking operator.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Condition)
	EDMSComparisonOperator Operator;

	/**
	 * Operating value.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Condition)
	int Value; // float? 

	virtual bool SingleCheckCondition_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence, UObject* Target) const;
};

UCLASS(BlueprintType)
class PROJECTDMSGAME_API UDMSObjectClassCondition : public UDMSObjectConditionBase
{
	GENERATED_BODY()

public:
	/**
	 * Checking Class;
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Condition)
	TSubclassOf<UObject> Class;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Condition)
	bool OnlyExact;

	virtual bool SingleCheckCondition_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence, UObject* Target) const;
};

//UCLASS(BlueprintType)
//class PROJECTDMSGAME_API UDMSLocatingCondition : public UDMSObjectConditionBase
//{
//	GENERATED_BODY()
//
//public:
//	/**
//	 * Checking attribute's tag.
//	 */
//	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Condition)
//	FGameplayTag AttributeTag;
//
//	/**
//	 * Operating value.
//	 */
//	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Condition)
//	int Value; // float? 
//
//	virtual bool SingleCheckCondition_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence, UObject* Target) const;
//};
