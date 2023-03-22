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

class UDMSNotifyCheckerDefinition;

/**
 *	========================================
 *
 *	Condition Object base
 *
 *	=========================================
 *
 */
UCLASS(Abstract, Const, EditInlineNew, ClassGroup = (Condition))
class PROJECTDMSGAME_API UDMSConditionObject : public UObject
{
	GENERATED_BODY()

public:
	UDMSConditionObject(){}

	virtual bool CheckCondition(UDMSSequence* iSeq) const {return true;}
	virtual bool CheckCondition(UObject* Caller, UDMSSequence* iSeq) const { return true; }
};

UCLASS(BlueprintType, ClassGroup = (Condition), meta = (DisplayName = "Use BP Condition"))
class PROJECTDMSGAME_API UDMSConditionClassWrapper : public UDMSConditionObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = Condition, meta = (DisplayName = "Condition Class"))
	TSubclassOf<UDMSConditionObject> Condition;


	virtual bool CheckCondition(UDMSSequence* iSeq) const override;
	virtual bool CheckCondition(UObject* Caller, UDMSSequence* iSeq) const override;
};


UCLASS(BlueprintType, ClassGroup = (Condition))
class PROJECTDMSGAME_API UDMSConditionCombiner : public UDMSConditionObject
{
	GENERATED_BODY()

public:
	UDMSConditionCombiner():bIsAnd(true){}

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<UDMSConditionObject*> Conditions;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	bool bIsAnd;
	
	virtual bool CheckCondition(UDMSSequence* iSeq) const;
	virtual bool CheckCondition(UObject* Caller, UDMSSequence* iSeq) const;
};

