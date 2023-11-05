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

/**
 *	Definition of "Decision Step": It is a step that a player must go through when trying to activate an effect. 
 *	When a player tries to activate an effect, the necessary information for that effect is collected, and the resulting changes are previewed to the player based on that information. 
 *	The player can then make a final decision on whether to activate the effect while considering the preview, and the "Decision Step" ends.
 *
 *	This widget is designed for the step of "collecting the necessary information", where the player has the choice to select the information and confirm it. 
 *	For example, if the effect can be activated on "one enemy", the widget for this would prompt the player to choose from the list of available enemies as the target.
 */

 // 셀렉터 아웃풋을 어떻게 처리할 지에 대한 정의
UCLASS(BlueprintType,Blueprintable, EditInlineNew, Abstract)
class DMSCORE_API UDMSSelectorBehaviorDefinition : public UObject
{
	GENERATED_BODY()
public:
	virtual void SetupFormDelegates(UDMSSequence* Sequence, TArray<FDMSSelectorRequestForm>& FormArr, bool SelectForEachEI) {}
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
	virtual void SetupFormDelegates(UDMSSequence* Sequence, TArray<FDMSSelectorRequestForm>& FormArr, bool SelectForEachEI);
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
	TArray<FDMSSelectorRequestForm> GenerateRequestForm(UDMSSequence* Sequence,bool SelectForEachEI);
	// 리퀘스트 폼에 들어갈 캔디데이트를 생성하는 함수 ( 사실상 이것을 위해 존재하는 클래스라고 보면 됨. )
	virtual TArray<UDMSDataObject*> GenerateCandidates(UDMSSequence* Sequence, UDMSEffectInstance* TargetEI);
};

UCLASS()
class DMSCORE_API USelectorRequestGenerator_FromSequenceData : public UDMSSelectorRequestGenerator
{
	GENERATED_BODY()
public:
	UPROPERTY( BlueprintReadWrite , EditDefaultsOnly )
	FGameplayTag DataTag;

	virtual TArray<UDMSDataObject*> GenerateCandidates ( UDMSSequence* Sequence , UDMSEffectInstance* TargetEI );

};

UCLASS()
class DMSCORE_API  USelectorRequestGenerator_FromEIData : public UDMSSelectorRequestGenerator
{
	GENERATED_BODY()
public:
	UPROPERTY( BlueprintReadWrite , EditDefaultsOnly )
	FGameplayTag DataTag;

	virtual TArray<UDMSDataObject*> GenerateCandidates ( UDMSSequence* Sequence , UDMSEffectInstance* TargetEI );

};
// USelectorRequestGenerator_WithTG : public UDMSSelectorRequestGenerator
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

	TArray<FDMSSelectorRequestForm> SetupRequestForm(UDMSSequence* Sequence);
};

