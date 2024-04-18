// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 * =========================================
 */

#include "DMSCoreIncludes.h"
#include "Selector/DMSSelectorManager.h"
#include "DMSDecisionDefinition.generated.h"

class UDMSTargetGenerator;
class ADMSActiveEffect;
class UDMSSelectorRequestGenerator;
class UDMSSelectorBehaviorDefinition;

UCLASS(BlueprintType,Blueprintable, EditInlineNew, Abstract)
class DMSCORE_API UDMSDecisionDefinitionBase : public UObject
{
	GENERATED_BODY()
public:
	// Sample
	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced)
	//TObjectPtr<UDMSSelectorRequestGenerator> Generator;

	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced)
	//TObjectPtr<UDMSSelectorBehaviorDefinition> Behavior;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool SelectForEachAE;

	UFUNCTION(BlueprintCallable)
	TArray<UDMSSelectorRequestForm*> CreateRequestForm(ADMSSequence* Sequence);
};


//USTRUCT(BlueprintType)
//struct FDMSDecisionDefinition
//{
//	GENERATED_BODY()
//
//	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced)
//	TObjectPtr<UDMSSelectorRequestGenerator> Generator;
//
//	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced)
//	TObjectPtr<UDMSSelectorBehaviorDefinition> Behavior;
//
//	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
//	bool SelectForEachEI;
//
//	TArray<UDMSSelectorRequestForm*> CreateRequestForm(ADMSSequence* Sequence){return {};}
//};


 // 셀렉터 아웃풋을 어떻게 처리할 지에 대한 정의
UCLASS(BlueprintType,Blueprintable, EditInlineNew, Abstract)
class DMSCORE_API UDMSSelectorBehaviorDefinition : public UObject
{
	GENERATED_BODY()
public:
	virtual void SetupFormDelegates(ADMSSequence* Sequence, TArray<UDMSSelectorRequestForm*>& FormArr, bool SelectForEachEI) {}
};


// 셀렉터 리퀘스트를 생성하는 방법에 대한 정의
UCLASS(Blueprintable, BlueprintType, EditInlineNew, Abstract)
class DMSCORE_API UDMSSelectorRequestGenerator : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	uint8 SelectAmount;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<UDMSSelectorBase> SelectorClass;

protected:
	virtual UDMSSelectorRequestForm* CreateNewRequestForm(ADMSSequence* Sequence){return nullptr;}
public:
	
	// 리퀘스트 폼을 제작 
	TArray<UDMSSelectorRequestForm*> GenerateRequestForm(ADMSSequence* Sequence, bool SelectForEachEI);
	// 리퀘스트 폼에 들어갈 캔디데이트를 생성하는 함수 ( 사실상 이것을 위해 존재하는 클래스라고 보면 됨. )
	virtual void CollectCandidates(ADMSSequence* Sequence, ADMSActiveEffect* TargetEI, UDMSSelectorRequestForm* CollectingRequestForm){}
};


// USelectorRequestGenerator_[Custom] : public UDMSSelectorRequestGenerator
