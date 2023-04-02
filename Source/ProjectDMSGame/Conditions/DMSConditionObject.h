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

#include "ProjectDMS.h"
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
UCLASS(Abstract, HideDropdown, Const, EditInlineNew, ClassGroup = (Condition))
class PROJECTDMSGAME_API UDMSConditionObjectBase : public UObject
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintNativeEvent)
	bool CheckOperation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence) const;
	virtual bool CheckOperation_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence) const {return true;}
	
public:
	UDMSConditionObjectBase():bIsNot(false){}

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Condition)
	bool bIsNot;

	UFUNCTION(BlueprintPure)
	bool CheckCondition(UObject* CheckingGameObject,UDMSSequence* CurrentSequence) const { return bIsNot? !CheckOperation(CheckingGameObject, CurrentSequence) : CheckOperation(CheckingGameObject, CurrentSequence); }


	virtual const UDMSConditionObjectBase* CheckCondition_(UObject* CheckingGameObject, UDMSSequence* CurrentSequence, bool& outResult) const { outResult = !bIsNot; return nullptr; }
};

UCLASS(BlueprintType, ClassGroup = (Condition))
class PROJECTDMSGAME_API UDMSConditionObject : public UDMSConditionObjectBase
{
	GENERATED_BODY()

public:
	UDMSConditionObject() :bIsRelative(false), bNullIsTrue(false) {}

	// For test now
	//UPROPERTY(EditDefaultsOnly, Category = Condition, meta = (DisplayName = "Is will be checked for every notified object"))
	bool bIsRelative;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Condition)
	bool bNullIsTrue;

	virtual bool CheckOperation_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence) const;
	virtual const UDMSConditionObjectBase* CheckCondition_(UObject* Caller, UDMSSequence* iSeq,bool& outResult) const;
};

UCLASS(BlueprintType, ClassGroup = (Condition), meta = (DisplayName = "Use BP Condition"))
class PROJECTDMSGAME_API UDMSConditionClassWrapper : public UDMSConditionObjectBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = Condition, meta = (DisplayName = "Condition Class"))
	TSubclassOf<UDMSConditionObjectBase> Condition;

	virtual bool CheckOperation_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence) const;
	virtual const UDMSConditionObjectBase* CheckCondition_(UObject* Caller, UDMSSequence* iSeq, bool& outResult) const override;
};


UCLASS(Blueprintable,BlueprintType, ClassGroup = (Condition))
class PROJECTDMSGAME_API UDMSConditionCombiner : public UDMSConditionObjectBase
{
	GENERATED_BODY()

public:
	UDMSConditionCombiner() :bIsAnd(true), bEmptyIsTrue(false){}

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Category = Condition)
	TArray<UDMSConditionObjectBase*> Conditions;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Condition)
	bool bIsAnd;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Condition)
	bool bEmptyIsTrue;

	virtual bool CheckOperation_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence) const;
	virtual const UDMSConditionObjectBase* CheckCondition_(UObject* Caller, UDMSSequence* iSeq, bool& outResult) const;
};