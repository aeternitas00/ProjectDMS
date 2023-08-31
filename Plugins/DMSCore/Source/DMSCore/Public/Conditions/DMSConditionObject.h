#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *	Base of Effect's condition.
 * 
 *	( RENAME? )
 *
 *	=========================================
 */

#include "DMSCoreIncludes.h"
#include "Common/DMSCommons.h"
#include "Kismet/KismetMathLibrary.h"
#include "DMSConditionObject.generated.h"

/**
 *	========================================
 *
 *	Condition Object base
 *
 *	========================================
 */

// Sort of interface but we need type specifier.
UCLASS(Abstract, Const, EditInlineNew, ClassGroup = (Condition))
class DMSCORE_API UDMSConditionObjectBase : public UObject
{
	GENERATED_BODY()

protected:
	/**
	 *	Condition checking function; This function is the actual implementation of 'Condition'.
 	 */
	UFUNCTION(BlueprintNativeEvent)
	bool CheckOperation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence) const;
	virtual bool CheckOperation_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence) const {return true;}
	
public:
	UDMSConditionObjectBase():bIsNot(false){}

	/**
	 * true if this checker is read as negative.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Condition)
	bool bIsNot;

	/**
	 * Checking condition function will be called from outer.
	 * @param	CheckingGameObject
	 * @param	CurrentSequence
	 * @return	bIsNot(CheckOperation).
	 */
	UFUNCTION(BlueprintPure)
	bool CheckCondition(UObject* CheckingGameObject,UDMSSequence* CurrentSequence) const { return bIsNot? !CheckOperation(CheckingGameObject, CurrentSequence) : CheckOperation(CheckingGameObject, CurrentSequence); }

	virtual const UDMSConditionObjectBase* CheckCondition_(UObject* CheckingGameObject, UDMSSequence* CurrentSequence, bool& outResult) const { outResult = !bIsNot; return nullptr; }
};

UCLASS(BlueprintType, ClassGroup = (Condition))
class DMSCORE_API UDMSConditionObject : public UDMSConditionObjectBase
{
	GENERATED_BODY()

public:
	UDMSConditionObject() :bIsRelative(false), bNullIsTrue(false) {}

	// For test now
	//UPROPERTY(EditDefaultsOnly, Category = Condition, meta = (DisplayName = "Is will be checked for every notified object"))
	bool bIsRelative;

	/**
	 * true if Condition object will returns true if invalid checked occurred
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Condition)
	bool bNullIsTrue;

	virtual bool CheckOperation_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence) const;
	virtual const UDMSConditionObjectBase* CheckCondition_(UObject* Caller, UDMSSequence* iSeq,bool& outResult) const;
};


/**
 * Use class data only with this wrapper if condition object doesn't need to instanced.
 */
UCLASS(BlueprintType, ClassGroup = (Condition), meta = (DisplayName = "Use BP Condition"))
class DMSCORE_API UDMSConditionClassWrapper : public UDMSConditionObjectBase
{
	GENERATED_BODY()
public:
	/**
	 * Class of actual condition object.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Condition, meta = (DisplayName = "Condition Class"))
	TSubclassOf<UDMSConditionObjectBase> Condition;

	virtual bool CheckOperation_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence) const;
	virtual const UDMSConditionObjectBase* CheckCondition_(UObject* Caller, UDMSSequence* iSeq, bool& outResult) const override;
};

/**
 * Combine multiple condition objects to applying group option.
 */
UCLASS(Blueprintable,BlueprintType, ClassGroup = (Condition))
class DMSCORE_API UDMSConditionCombiner : public UDMSConditionObjectBase
{
	GENERATED_BODY()

public:
	UDMSConditionCombiner() :bIsAnd(true), bEmptyIsTrue(false){}

	/**
	 * Condition objects.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Category = Condition)
	TArray<UDMSConditionObjectBase*> Conditions;

	/**
	 * true if this combiner returns combine condition results as and operator.
	 * ex) return (Condition[0] && ... && Condition[n])
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Condition)
	bool bIsAnd;

	/**
	 * true if combiner will returns true if Conditions is empty.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Condition)
	bool bEmptyIsTrue;

	virtual bool CheckOperation_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence) const;
	virtual const UDMSConditionObjectBase* CheckCondition_(UObject* Caller, UDMSSequence* iSeq, bool& outResult) const;
};