#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *	Timing checking part of Effect's condition.
 * 
 *	( RENAME? )
 *
 * =========================================
 */

#include "ProjectDMS.h"
#include "Common/DMSCommons.h"
#include "Common/DMSObjectSelectionComparer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/DataAsset.h"
#include "DMSTimingCondition.generated.h"


/**
 * ========================================
 *
 *	Define timing condition of Effect's. Sort of 'relative' condition.
 *	Comparing own data with passed notify data is main feature.
 *
 * =========================================
 *
 *	��Ƽ���� �Ŵ����� ��Ƽ���� ���ù��鿡�� [ "SourceObjectClass"�� "EffectKeyword"�� (���) "TargetObjectClass"���� "Timing"(�ϱ���/�ϴ���/����) ]
 *	�߰������� �ҽ� ������Ʈ�� Ÿ�� ������Ʈ���� Ŭ���� ������ �ƴ� ��ü ������ ĵ����Ʈ�� ���� �Ϸ��� ���� �ؾ��ұ�?
 */
UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew, Abstract, EditInlineNew, meta=(DisplayName="Create Custom Notify Checker"))
class UDMSNotifyCheckerDefinition : public UObject
{
	GENERATED_BODY()

public:
	// Return true when checker doesn't meet proper data
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bAllowNull;

	UPROPERTY(EditDefaultsOnly, Category = Selector)
	EDMSObjectSelectorFlag CompareTargetFlag;

public:
	virtual bool Check(UObject* Caller, TArray<UObject*> CompareTarget) { return bAllowNull; }
};

UCLASS(meta = (DisplayName = "Notify Checker : Numeric Check Base"))
class UDMSNotifyCheckerDefinition_Numeric : public UDMSNotifyCheckerDefinition
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName DataName;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	EDMSComparisonOperator Comparer;

	// TODO :: Not value check ... Make this Compare Operation with caller <-> Target
	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	//bool bIsRanged;

	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	//float ExactValue;

	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	//float MinValue;

	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	//float MaxValue;

public:
	// UFUNCTION + _Implements
	virtual bool Check(UObject* Caller, TArray<UObject*> CompareTarget);
};

UCLASS(/*Abstract, */meta = (DisplayName = "Notify Checker : Object Check Base"))
class UDMSNotifyCheckerDefinition_ObjectCompareBase : public UDMSNotifyCheckerDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FDMSObjectSelectionRules SelectionRules;

public:
	// UFUNCTION + _Implements
	virtual bool Check(UObject* Caller, TArray<UObject*> CompareTarget);
};

 /**
 *	========================================
 *
 *	A simple container that collect single conditions and compose them like commonly used boolean expressions.
 *
 *	========================================
 */
UCLASS(/*HideDropdown,*/ Blueprintable, BlueprintType, Const, EditInlineNew, ClassGroup = (Condition), meta = (DisplayName = "Timing Condition Base"))
class PROJECTDMSGAME_API UDMSTimingCondition : public UObject
{
	GENERATED_BODY()

public:
	UDMSTimingCondition()/*: SourceObjectClass(nullptr), TargetObjectClass(nullptr)*/ {}

	// 'Before / During / After' What happens
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Condition)
	EDMSTimingFlag Timing;

	// 'What'
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Condition)
	FName EffectKeyword;

	// 'How' 'Who' 'To who'
	UPROPERTY(EditDefaultsOnly, Instanced, Category = Condition)
	TArray<UDMSNotifyCheckerDefinition*> Checkers;

	UPROPERTY(EditDefaultsOnly, Category = Condition)
	FString CheckersCombinator;

	// ȿ�� �ߵ��� ���� ���ñ��� �ִ� ȿ���ΰ�? == false : ���ǿ� ������ ���� �ߵ� / true : ���ǿ� ������ ȿ�� �ߵ��� ���� Y/N UI ���� ���·�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Condition)
	bool bUserDecidable;

	// Deprecated
	bool isChildOf(const UDMSTimingCondition& i) const;

	bool CheckCondition(UObject* Caller, UDMSSequence* iSeq) const;

	// TODO :: Move to library 
	TArray<UObject*> GetCompareTarget(UObject* Caller, UDMSSequence* iSeq, const EDMSObjectSelectorFlag& Flag) const;
};

/**
 * 	========================================
 *
 *	Timing Condition Wrapper :: Wrapper for exposing condition object to Blueprint in various ways.
 *	Similar to Effect Node Wrapper. 
 *	 
 *	=========================================
 */
UCLASS(Abstract, Const, DefaultToInstanced, EditInlineNew, ClassGroup = (Condition))
class PROJECTDMSGAME_API UDMSTimingConditionWrapper : public UObject
{
	GENERATED_BODY()

public:
	virtual bool CheckCondition(UObject* Caller, UDMSSequence* iSeq) const{return false;};
};

UCLASS(BlueprintType, ClassGroup = (Condition), meta = (DisplayName = "Make Timing Condition in def"))
class PROJECTDMSGAME_API UDMSTimingConditionWrapper_Manual : public UDMSTimingConditionWrapper
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,Instanced, Category = Condition)
	UDMSTimingCondition* Condition;
	virtual bool CheckCondition(UObject* Caller, UDMSSequence* iSeq) const override;
};

UCLASS(BlueprintType, ClassGroup = (Condition), meta = (DisplayName = "Use BP Timing Condition"))
class PROJECTDMSGAME_API UDMSTimingConditionWrapper_Preset : public UDMSTimingConditionWrapper
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = Condition, meta = (DisplayName = "Condition Class"))
	TSubclassOf<UDMSTimingCondition> Condition;

	virtual bool CheckCondition(UObject* Caller, UDMSSequence* iSeq) const override;
};