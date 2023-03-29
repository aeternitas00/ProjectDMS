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
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly,Instanced)
	UDMSObjectSelectionComparer_* Comparer;

	virtual bool CheckCondition_Single(UObject* Caller, UDMSSequence* iSeq, UObject* Target ) const;
};

UCLASS(Blueprintable, Abstract, EditInlineNew)
class UDMSObjectSelectionComparer_ : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	bool Compare(UObject* SourceObject, UDMSSequence* iSeq, UObject* TargetObject);

	virtual bool Compare_Implementation(UObject* SourceObject, UDMSSequence* iSeq, UObject* TargetObject) { return true; }

};

UCLASS(BlueprintType, meta = (DisplayName = "Comparer : Obj == Target"))
class UDMSObjectSelectionComparer_IsSelf_ : public UDMSObjectSelectionComparer_
{
	GENERATED_BODY()

public:

	virtual bool Compare_Implementation(UObject* SourceObject, UDMSSequence* iSeq, UObject* TargetObject) override {
		//DMS_LOG_SCREEN(TEXT("%s == %s"),*SourceObject->GetName(), *TargetObject->GetName());
		return SourceObject == TargetObject;
	}

};

UCLASS(BlueprintType, meta = (DisplayName = "Comparer : Class of Obj == Class of Target"))
class UDMSObjectSelectionComparer_IsSpecificType_ : public UDMSObjectSelectionComparer_
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

	// true = Source && Target , false Source || Target
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bIsAndOperator;

public:
	UDMSObjectSelectionComparer_IsSpecificType_() : SourceType(nullptr), TargetType(nullptr) {}
	
	virtual bool Compare_Implementation(UObject* SourceObject, UDMSSequence* iSeq, UObject* TargetObject) override {
		bool A = (SourceType != nullptr) ?
			(bAllowSourceChildClass ? UKismetMathLibrary::ClassIsChildOf(SourceObject->GetClass(), SourceType) : SourceObject->GetClass() == SourceType) : true;
		bool B = (TargetType != nullptr) ?
			(bAllowTargetChildClass ? UKismetMathLibrary::ClassIsChildOf(TargetObject->GetClass(), TargetType) : TargetObject->GetClass() == TargetType) : true;
		return bIsAndOperator ? (A && B) : (A || B);
	}
};