// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 * =========================================
 */

#include "Selector/DMSDecisionDefinition.h"
#include "DMSDecisionDefinition_Object.generated.h"

UCLASS()
class DMSCORE_API UDMSSelectorRequestForm_Object : public UDMSSelectorRequestForm
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<UObject>> Candidates;
};

UCLASS()
class DMSCORE_API UDMSDecisionDefinition_Object : public UDMSDecisionDefinitionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced)
	TObjectPtr<USelReqGenerator_ObjCand> Generator;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced)
	TObjectPtr<UDMSSelBehavDefinition_Object> Behavior;
};

// 셀렉터 아웃풋을 어떻게 처리할 지에 대한 정의
UCLASS(Abstract)
class DMSCORE_API UDMSSelBehavDefinition_Object : public UDMSSelectorBehaviorDefinition
{
	GENERATED_BODY()

};

UCLASS()
class DMSCORE_API UDMSSelBehavDefinition_Object_UpdateSeqAtt: public UDMSSelBehavDefinition_Object
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	FGameplayTagContainer AttributeTags;

public:
	virtual void SetupFormDelegates(ADMSSequence* Sequence, TArray<UDMSSelectorRequestForm*>& FormArr, bool SelectForEachEI);
};



// Update target of current sequence.
UCLASS()
class DMSCORE_API UDMSSelBehavDefinition_Object_UpdateTarget: public UDMSSelBehavDefinition_Object
{
	GENERATED_BODY()
public:
	virtual void SetupFormDelegates(ADMSSequence* Sequence, TArray<UDMSSelectorRequestForm*>& FormArr, bool SelectForEachEI);
};

//UCLASS()
//class DMSCORE_API UDMSSelectorBehaviorDefinition_UpdateData: public UDMSSelBehavDefinition_Object
//{
//	GENERATED_BODY()
//
//public:
//	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
//	FGameplayTag OutKey;
//
//public:
//	virtual void SetupFormDelegates(ADMSSequence* Sequence, TArray<UDMSSelectorRequestForm*>& FormArr, bool SelectForEachEI);
//};


UCLASS()
class DMSCORE_API USelReqGenerator_ObjCand : public UDMSSelectorRequestGenerator
{
	GENERATED_BODY()
protected:
	virtual UDMSSelectorRequestForm* CreateNewRequestForm(ADMSSequence* Sequence);
public:
	virtual void CollectCandidates(ADMSSequence* Sequence, ADMSActiveEffect* TargetEI, UDMSSelectorRequestForm* CollectingRequestForm);
	virtual TArray<UObject*> CollectObjects(ADMSSequence* Sequence, ADMSActiveEffect* TargetEI) {return {};}
};

// Merge to WithTG->FromAttribute
UCLASS()
class DMSCORE_API USelReqGenerator_ObjCand_FromAtt : public USelReqGenerator_ObjCand
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced)
	TObjectPtr<UDMSTargetGenerator> AttributeOwners;

	UPROPERTY( BlueprintReadWrite , EditDefaultsOnly )
	FGameplayTagQuery AttributeTagQuery;

	virtual TArray<UObject*> CollectObjects(ADMSSequence* Sequence, ADMSActiveEffect* TargetEI);
};

UCLASS()
class DMSCORE_API USelReqGenerator_ObjCand_WithTG : public USelReqGenerator_ObjCand
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced)
	TObjectPtr<UDMSTargetGenerator> Candidates;

	virtual TArray<UObject*> CollectObjects(ADMSSequence* Sequence, ADMSActiveEffect* TargetEI);
};
