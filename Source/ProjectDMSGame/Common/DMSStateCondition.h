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

#include "DMSConditionObject.h"
#include "Common/DMSCommons.h"
#include "DMSStateCondition.generated.h"

class UDMSSequence;

UCLASS()
class PROJECTDMSGAME_API UDMSStateConditionBase : public UDMSConditionObject
{
	GENERATED_BODY()
};

/**
 *	========================================
 *
 *	A simple container that collect single conditions and compose them like commonly used boolean expressions.
 *
 *	========================================
 */
UCLASS(/*HideDropdown,*/ Blueprintable, BlueprintType, Const,EditInlineNew, ClassGroup = (Condition), meta = (DisplayName = "State Condition Base"))
class PROJECTDMSGAME_API UDMSStateCondition : public UDMSStateConditionBase
{
	GENERATED_BODY()

public:
	UDMSStateCondition(){}
	UPROPERTY(EditDefaultsOnly, Instanced, Category = Condition)
	TArray<UDMSStateCheckerDefinition*> StateConditions;


	virtual bool CheckCondition(UDMSSequence* Seq) const;
};

UCLASS(BlueprintType, ClassGroup = (Condition), meta = (DisplayName = "Use BP Condition"))
class PROJECTDMSGAME_API UDMSStateClassWrapper : public UDMSStateConditionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = Condition, meta = (DisplayName = "Condition Class"))
	TSubclassOf<UDMSStateCondition> Condition;

	virtual bool CheckCondition(UDMSSequence* iSeq) const override;
};


UCLASS(BlueprintType, ClassGroup = (Condition))
class PROJECTDMSGAME_API UDMSStateCombiner : public UDMSStateConditionBase
{
	GENERATED_BODY()

public:
	UDMSStateCombiner() :bIsAnd(true), bEmptyStateIsTrue(false) {}

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced)
	TArray<UDMSStateConditionBase*> Conditions;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bIsAnd;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bEmptyStateIsTrue;


	virtual bool CheckCondition(UDMSSequence* iSeq) const override;
};




/**
 *	========================================
 *
 *	Define non-timing condition of Effect's. ( non-relative )
 *	It doesn't depend on who receives the notify data, so we may be able to check this just once during the notify loop.
 *	( RENAME? )
 *
 *	========================================
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, ClassGroup = (Condition))
class PROJECTDMSGAME_API UDMSStateCheckerDefinition : public UObject
{
	GENERATED_BODY()

public:
	UDMSStateCheckerDefinition() : bNullIsTrue(true), CheckerTargetFlag(EDMSObjectSelectorFlag::OSF_SourceObj) {};

	UPROPERTY(EditDefaultsOnly, Category = Condition)
	bool bNullIsTrue;

	UPROPERTY(EditDefaultsOnly, Category = Condition)
	EDMSObjectSelectorFlag CheckerTargetFlag;

	virtual bool CheckState(UObject* Target) { return bNullIsTrue; }
};

UCLASS(BlueprintType)
class PROJECTDMSGAME_API UDMSAttributeCheckerDefinition : public UDMSStateCheckerDefinition
{
	GENERATED_BODY()

public:
	//UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Condition)
	//FName AttributeName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute)
	FGameplayTag AttributeTag;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Condition)
	EDMSComparisonOperator Operator;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Condition)
	int Value; // float? 

	virtual bool CheckState(UObject* Target);
};

UCLASS()
class PROJECTDMSGAME_API UDMSDataCheckerDefinition : public UDMSStateCheckerDefinition
{
	GENERATED_BODY()

public:
	//UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Condition)
	//FName DataName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute)
	FGameplayTag DataTag;

	// 플래그를 통한 프로퍼티의 에디터 노출 ON OFF가 가능했나?
	// 상속 없이 구현하는게 더 깔끔해 보이지만 다른 수가 없으니 일단은.

	virtual bool CheckState(UObject* Target) { return bNullIsTrue; }

	template<typename T>
	bool CheckState_Internal(UObject* Target, const T& Value);
};

UCLASS(BlueprintType)
class PROJECTDMSGAME_API UDMSDataCheckerDefinition_Numeric : public UDMSDataCheckerDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Condition)
	float Data;

	virtual bool CheckState(UObject* Target);
};

UCLASS(BlueprintType)
class PROJECTDMSGAME_API UDMSDataCheckerDefinition_Name : public UDMSDataCheckerDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Condition)
	FName Data;

	virtual bool CheckState(UObject* Target);
};

