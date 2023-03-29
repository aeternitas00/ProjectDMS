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

public:
	UDMSConditionObjectBase() {}

	virtual bool CheckCondition(UObject* Caller, UDMSSequence* iSeq) const {return true;}
	virtual const UDMSConditionObjectBase* CheckCondition_(UObject* Caller, UDMSSequence* iSeq, bool& outResult) const { outResult = true; return nullptr; }
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

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bNullIsTrue;

	virtual bool CheckCondition(UObject* Caller, UDMSSequence* iSeq) const;
	virtual const UDMSConditionObjectBase* CheckCondition_(UObject* Caller, UDMSSequence* iSeq,bool& outResult) const;
};

UCLASS(BlueprintType, ClassGroup = (Condition), meta = (DisplayName = "Use BP Condition"))
class PROJECTDMSGAME_API UDMSConditionClassWrapper : public UDMSConditionObjectBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = Condition, meta = (DisplayName = "Condition Class"))
	TSubclassOf<UDMSConditionObjectBase> Condition;

	virtual bool CheckCondition(UObject* Caller, UDMSSequence* iSeq) const;
	virtual const UDMSConditionObjectBase* CheckCondition_(UObject* Caller, UDMSSequence* iSeq, bool& outResult) const override;
};


UCLASS(Blueprintable,BlueprintType, ClassGroup = (Condition))
class PROJECTDMSGAME_API UDMSConditionCombiner : public UDMSConditionObjectBase
{
	GENERATED_BODY()

public:
	UDMSConditionCombiner() :bIsAnd(true), bEmptyIsTrue(false){}

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced)
	TArray<UDMSConditionObjectBase*> Conditions;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bIsAnd;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bEmptyIsTrue;

	virtual bool CheckCondition(UObject* Caller, UDMSSequence* iSeq) const;
	virtual const UDMSConditionObjectBase* CheckCondition_(UObject* Caller, UDMSSequence* iSeq, bool& outResult) const;
};