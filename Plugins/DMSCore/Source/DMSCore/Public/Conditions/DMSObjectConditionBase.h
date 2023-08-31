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
#include "Common/DMSTargetGenerator.h"
#include "Common/DMSCommons.h"
#include "DMSObjectConditionBase.generated.h"

class UDMSSequence;

/**
 *	========================================
 *
 *	A simple container that collect single conditions and compose them like commonly used boolean expressions.
 *
 *	========================================
 */
UCLASS(Abstract, Blueprintable, BlueprintType, Const,EditInlineNew, ClassGroup = (Condition))
class DMSCORE_API UDMSObjectConditionBase : public UDMSConditionObject
{
	GENERATED_BODY()

public:
	UDMSObjectConditionBase();

	/**
	 * Target generator that get the other side of objects for condition checking.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = Condition)
	TObjectPtr<UDMSTargetGenerator> CompareTargetGenerator;

	/**
	 * If it is true, CheckOperation returns true only if all of object checkings were true
	 */
	UPROPERTY(EditDefaultsOnly, Category = Condition)
	bool bAllObjectMustPassed;

	/**
	 * Get compare target with TargetFlag.
	 * @param	CheckingGameObject
	 * @param	CurrentSequence
	 * @return	Compare targets[]
	 */
	TArray<UObject*> GetCompareTarget(UObject* CheckingGameObject, UDMSSequence* CurrentSequence, const UDMSTargetGenerator* TargetGenerator) const;

	virtual bool CheckOperation_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence) const;

	/** 
	 * Checking function will be excuted for each compare targets.
	 * @param	CheckingGameObject
	 * @param	CurrentSequence
	 * @param	Target
	 * return	Result of check condition.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = Condition)
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

UCLASS(BlueprintType)
class DMSCORE_API UDMSObjectClassCondition : public UDMSObjectConditionBase
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
