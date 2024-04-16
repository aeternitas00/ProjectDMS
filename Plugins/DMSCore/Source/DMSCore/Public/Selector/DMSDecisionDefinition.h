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

 // 셀렉터 아웃풋을 어떻게 처리할 지에 대한 정의
UCLASS(BlueprintType,Blueprintable, EditInlineNew, Abstract)
class DMSCORE_API UDMSSelectorBehaviorDefinition : public UObject
{
	GENERATED_BODY()
public:
	virtual void SetupFormDelegates(ADMSSequence* Sequence, TArray<FDMSSelectorRequestForm>& FormArr, bool SelectForEachEI) {}
};

// 위를 베이스로 / EI 데이터를 업데이트 한다 라는 정의
UCLASS()
class DMSCORE_API UDMSSelectorBehaviorDefinition_UpdateData: public UDMSSelectorBehaviorDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	FGameplayTag OutKey;

public:
	virtual void SetupFormDelegates(ADMSSequence* Sequence, TArray<FDMSSelectorRequestForm>& FormArr, bool SelectForEachEI);
};

// Update target of current sequence.
UCLASS()
class DMSCORE_API UDMSSelectorBehaviorDefinition_UpdateTarget: public UDMSSelectorBehaviorDefinition
{
	GENERATED_BODY()
public:
	virtual void SetupFormDelegates(ADMSSequence* Sequence, TArray<FDMSSelectorRequestForm>& FormArr, bool SelectForEachEI);
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
public:

	// 리퀘스트 폼을 제작 
	TArray<FDMSSelectorRequestForm> GenerateRequestForm(ADMSSequence* Sequence,bool SelectForEachEI);
	// 리퀘스트 폼에 들어갈 캔디데이트를 생성하는 함수 ( 사실상 이것을 위해 존재하는 클래스라고 보면 됨. )
	virtual TArray<UObject*> GenerateCandidates(ADMSSequence* Sequence, ADMSActiveEffect* TargetEI);
};

UCLASS()
class DMSCORE_API USelectorRequestGenerator_FromSequenceData : public UDMSSelectorRequestGenerator
{
	GENERATED_BODY()
public:
	UPROPERTY( BlueprintReadWrite , EditDefaultsOnly )
	FGameplayTagContainer DataTag;

	virtual TArray<UObject*> GenerateCandidates ( ADMSSequence* Sequence , ADMSActiveEffect* TargetEI );

};

UCLASS()
class DMSCORE_API USelectorRequestGenerator_FromEIData : public UDMSSelectorRequestGenerator
{
	GENERATED_BODY()
public:
	UPROPERTY( BlueprintReadWrite , EditDefaultsOnly )
	FGameplayTagContainer DataTag;

	virtual TArray<UObject*> GenerateCandidates ( ADMSSequence* Sequence , ADMSActiveEffect* TargetEI );

};
UCLASS()
class DMSCORE_API USelectorRequestGenerator_WithTG : public UDMSSelectorRequestGenerator
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced)
	TObjectPtr<UDMSTargetGenerator> TargetGenerator;

	virtual TArray<UObject*> GenerateCandidates(ADMSSequence* Sequence , ADMSActiveEffect* TargetEI);

};
// USelectorRequestGenerator_[Custom] : public UDMSSelectorRequestGenerator

USTRUCT(BlueprintType)
struct FDMSDecisionDefinition
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced)
	TObjectPtr<UDMSSelectorRequestGenerator> Generator;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced)
	TObjectPtr<UDMSSelectorBehaviorDefinition> Behavior;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool SelectForEachEI;

	TArray<FDMSSelectorRequestForm> SetupRequestForm(ADMSSequence* Sequence);
};

