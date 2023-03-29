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

#include "DMSConditionObject.h"
#include "Common/DMSObjectSelectionComparer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/DataAsset.h"
#include "DMSTimingCondition.generated.h"

class UDMSNotifyCheckerDefinition;

UCLASS()
class PROJECTDMSGAME_API UDMSTimingConditionBase : public UDMSConditionObject
{
	GENERATED_BODY()
};

 /**
  * ========================================
  *
  *	A simple container that collect single conditions and compose them like commonly used boolean expressions.
  *	Define timing condition of Effect's. Sort of 'relative' condition.
  *	Comparing own data with passed notify data is main feature.
  *
  * =========================================
  *
  *	노티파이 매니저가 노티파이 리시버들에게 [ "SourceObjectClass"가 "EffectKeyword"를 (어떻게) "TargetObjectClass"에게 "Timing"(하기전/하는중/한후) ]
  *	추가적으로 소스 오브젝트나 타겟 오브젝트들의 클래스 단위가 아닌 객체 단위의 캔디데이트를 제공 하려면 어찌 해야할까?
  */

UCLASS(Blueprintable, BlueprintType, Const, EditInlineNew, ClassGroup = (Condition), meta = (DisplayName = "Timing Condition Base"))
class PROJECTDMSGAME_API UDMSTimingCondition : public UDMSTimingConditionBase
{
	GENERATED_BODY()

public:
	UDMSTimingCondition()/*: SourceObjectClass(nullptr), TargetObjectClass(nullptr)*/ {}

	// 'Before / During / After' What happens
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Condition)
	EDMSTimingFlag Timing;

	// 'What'
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Condition)
	//FName EffectKeyword;

	// 'What'
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Condition)
	FGameplayTagQuery EffectTagQuery;

	// 'How' 'Who' 'To who'
	UPROPERTY(EditDefaultsOnly, Instanced, Category = Condition)
	TArray<UDMSNotifyCheckerDefinition*> Checkers;

	//UPROPERTY(EditDefaultsOnly, Category = Condition)
	//FString CheckersCombinator;

	// Deprecated
	//bool isChildOf(const UDMSTimingCondition& i) const;

	virtual bool CheckCondition(UObject* Caller, UDMSSequence* iSeq) const override;

	// TODO :: Move to library 
	TArray<UObject*> GetCompareTarget(UObject* Caller, UDMSSequence* iSeq, const EDMSObjectSelectorFlag& Flag) const;
};

UCLASS(BlueprintType, ClassGroup = (Condition), meta = (DisplayName = "Use BP Condition"))
class PROJECTDMSGAME_API UDMSTimingClassWrapper : public UDMSTimingConditionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = Condition, meta = (DisplayName = "Condition Class"))
	TSubclassOf<UDMSTimingCondition> Condition;

	virtual bool CheckCondition(UObject* Caller, UDMSSequence* iSeq) const override;
};


UCLASS(BlueprintType, ClassGroup = (Condition))
class PROJECTDMSGAME_API UDMSTimingCombiner : public UDMSTimingConditionBase
{
	GENERATED_BODY()

public:
	UDMSTimingCombiner() :bIsAnd(true), bEmptyTimingIsTrue(false) {}

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced)
	TArray<UDMSTimingConditionBase*> Conditions;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bIsAnd;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bEmptyTimingIsTrue;

	virtual bool CheckCondition(UObject* Caller, UDMSSequence* iSeq) const override;
};


UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew, Abstract, EditInlineNew, meta=(DisplayName="Create Custom Notify Checker"))
class UDMSNotifyCheckerDefinition : public UObject
{
	GENERATED_BODY()

public:
	// Return true when checker doesn't meet proper data
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bAllowNull;

	UPROPERTY(EditDefaultsOnly)
	EDMSObjectSelectorFlag CompareTargetFlag;

public:

	UFUNCTION(BlueprintNativeEvent)
	bool Check(UObject* Caller, const TArray<UObject*>& CompareTarget);

	virtual bool Check_Implementation(UObject* Caller, const TArray<UObject*>& CompareTarget) { return bAllowNull; }
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
	virtual bool Check_Implementation(UObject* Caller, const TArray<UObject*>& CompareTarget);
};