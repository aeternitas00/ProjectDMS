// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *	Working...
 *
 *	=========================================
 * 
 *	기본적으로 게임 오브젝트 탐색을 위한 범용 컴페어러를 만들 생각으로 구현 중이었는데 아직 잘 모르겠음. ( 컨디션 체킹, 셀렉터에 후보군 리스트 만들기 위한 탐색 등... )
 *	필요성이라던가 구현방식에 대해 좀 더 생각해보기.
 */

#include "ProjectDMS.h"
#include "Kismet/KismetMathLibrary.h"
#include "Common/DMSCommons.h"
#include "DMSObjectSelectionComparer.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,Abstract, EditInlineNew)
class UDMSObjectSelectionComparer : public UObject
{
	GENERATED_BODY()

public:

	virtual bool Compare(UObject* SourceObject, UObject* TargetObject ){return true;}

};

UCLASS(BlueprintType, meta = (DisplayName = "Comparer : Notified Obj == Target"))
class UDMSObjectSelectionComparer_IsSelf : public UDMSObjectSelectionComparer
{
	GENERATED_BODY()

public:

	virtual bool Compare(UObject* SourceObject, UObject* TargetObject) override { 
		//DMS_LOG_SCREEN(TEXT("%s == %s"),*SourceObject->GetName(), *TargetObject->GetName());
		return SourceObject == TargetObject;
	}

};

UCLASS(BlueprintType, meta = (DisplayName = "Comparer : Class of Notified Obj == Class of Target"))
class UDMSObjectSelectionComparer_IsSpecificType: public UDMSObjectSelectionComparer
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
	UDMSObjectSelectionComparer_IsSpecificType(): SourceType(nullptr), TargetType(nullptr){}
	virtual bool Compare(UObject* SourceObject, UObject* TargetObject) override { 
		bool A = (SourceType != nullptr) ? 
		( bAllowSourceChildClass ? UKismetMathLibrary::ClassIsChildOf(SourceObject->GetClass(), SourceType) : SourceObject->GetClass() == SourceType) : true;
		bool B = (TargetType != nullptr) ? 
		( bAllowTargetChildClass ? UKismetMathLibrary::ClassIsChildOf(TargetObject->GetClass(), TargetType) : TargetObject->GetClass() == TargetType) : true;
		return bIsAndOperator ? (A && B) : (A || B);
	}
};



USTRUCT(BlueprintType)
struct FDMSObjectSelectionRules
{
	GENERATED_BODY()

public:
	FDMSObjectSelectionRules(){}

	// Special rules;
	UPROPERTY(EditDefaultsOnly,Instanced)
	TArray<UDMSObjectSelectionComparer*> Comparers;

	FString ComparersCombinator;

	//UPROPERTY 노출 가능한 구조?
	bool CompareObject(UObject* Caller, TArray<UObject*> Targets);
	bool CompareObject(UObject* iObj) { return false; }
};
