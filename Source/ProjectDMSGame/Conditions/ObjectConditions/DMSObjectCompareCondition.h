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

#include "Conditions/DMSObjectCondition.h"
#include "DMSObjectCompareCondition.generated.h"

class UDMSSequence;

UCLASS(BlueprintType)
class PROJECTDMSGAME_API UDMSObjectCompareCondition : public UDMSObjectConditionBase
{
	GENERATED_BODY()

public:
	//UDMSObjectCompareCondition(){}

	UPROPERTY(EditDefaultsOnly, Category = Condition)
	EDMSObjectSelectorFlag SourceFlag;

	/**
	 * If it is true, CheckOperation returns true only if all of target check was true
	 */
	UPROPERTY(EditDefaultsOnly, Category = Condition)
	bool bAllSourcesMustPassed;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly,Instanced)
	UDMSObjectComparer* Comparer;

	virtual bool CheckOperation_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence) const;
	virtual bool SingleCheckCondition_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence, UObject* CompareTarget ) const;
};

UCLASS(Blueprintable, Abstract, EditInlineNew)
class UDMSObjectComparer : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	bool Compare(UObject* SourceObject, UDMSSequence* iSeq, UObject* TargetObject, bool NullIsTrue);

	virtual bool Compare_Implementation(UObject* SourceObject, UDMSSequence* iSeq, UObject* TargetObject, bool NullIsTrue) { return NullIsTrue; }

};

UCLASS(BlueprintType, meta = (DisplayName = "Comparer : Target has Source Object"))
class UDMSObjectComparer_IsTarget : public UDMSObjectComparer
{
	GENERATED_BODY()

public:

	virtual bool Compare_Implementation(UObject* SourceObject, UDMSSequence* iSeq, UObject* TargetObject, bool NullIsTrue) override {
		//DMS_LOG_SCREEN(TEXT("%s == %s"),*SourceObject->GetName(), *CompareTarget->GetName());
		return SourceObject == TargetObject;
	}

};


UCLASS(BlueprintType, meta = (DisplayName = "Comparer : Target is in the same location"))
class UDMSObjectComparer_IsSameLocation : public UDMSObjectComparer
{
	GENERATED_BODY()

public:

	virtual bool Compare_Implementation(UObject* SourceObject, UDMSSequence* iSeq, UObject* TargetObject, bool NullIsTrue) override;

};

UCLASS(BlueprintType, meta = (DisplayName = "Comparer : Class of Source Obj == Class of CompareTarget"))
class UDMSObjectComparer_IsSpecificType : public UDMSObjectComparer
{
	GENERATED_BODY()

protected:

	// null = always true
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UClass* SourceType;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bAllowSourceChildClass;

	// null = always true
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UClass* TargetType;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bAllowTargetChildClass;

	// true = Source && CompareTarget , false Source || CompareTarget
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bIsAndOperator;

public:
	UDMSObjectComparer_IsSpecificType() : SourceType(nullptr), TargetType(nullptr) {}
	
	virtual bool Compare_Implementation(UObject* SourceObject, UDMSSequence* iSeq, UObject* TargetObject, bool NullIsTrue) override {
		bool A = (SourceType != nullptr) ?
			(bAllowSourceChildClass ? UKismetMathLibrary::ClassIsChildOf(SourceObject->GetClass(), SourceType) : SourceObject->GetClass() == SourceType) : true;
		bool B = (TargetType != nullptr) ?
			(bAllowTargetChildClass ? UKismetMathLibrary::ClassIsChildOf(TargetObject->GetClass(), TargetType) : TargetObject->GetClass() == TargetType) : true;
		return bIsAndOperator ? (A && B) : (A || B);
	}
};

