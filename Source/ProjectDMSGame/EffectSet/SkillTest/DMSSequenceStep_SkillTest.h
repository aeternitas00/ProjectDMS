// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Selector/DMSConfirmWidgetBase.h"
#include "Selector/DMSSelectorManager.h"
#include "Sequence/DMSSequenceStep.h"
#include "DMSSequenceStep_SkillTest.generated.h"

class ADMSCardBase;
class UDMSSelectorHandle;
class UDMSSelectorRequestGenerator;
class UDMSTargetGenerator;

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Step_SkillTest)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Step_SkillTest_Committable)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Step_SkillTest_Data_TestDiffModifier)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Step_SkillTest_Data_TestResult)

USTRUCT(BlueprintType)
struct FDMSSkillTestData
{
	GENERATED_BODY()

	// NOTE :: OR ARRAY of tag cont?
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FGameplayTagContainer StatName;
	
	// Leave this empty if test is for static value.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FGameplayTagContainer TargetStatName;

	// Default value to use when the corresponding numeric attribute for the test target cannot be found.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float DefaultTargetValue;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bStopSequenceIfTestFailed = 1;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float MinResultForTestSuccess = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool IsChainable;

	// Get from decision step. or source player is Tester / sequence target test target;
	// Get stat from who? ( only one )
	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced)
	//TObjectPtr<UDMSTargetGenerator> TesterGenerator;

	// Get stat from who? ( only one )
	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced, meta = (EditCondition = "!bTestToStaticValue", EditConditionHides))
	//TObjectPtr<UDMSTargetGenerator> TestTargetGenerator;

	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	//bool bTestToStaticValue;

	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	//bool CanUseBonusStat;

	// 최종적인 이펙트 적용 대상 마다 같은 테스트를 반복할지 결정.
	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	//bool TestByEachApplyTarget;

	// 스킬 결과 값의 반전 ( 실패시 진행하는 효과에 유용 )
	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	//bool ReverseTestResult;

	// ENum TestMethod ?
	// 1. Test with ( Source[StatName] + Static Value )
	// 2. Test with ( Source[StatName] + Target[StatName] )
	// 3. Custom? ( Multiple stat )

};


/**
* Skill test is for one tester : one test target 
* Require Active effect. ( have to set target before run this step )
*/
UCLASS()
class PROJECTDMSGAME_API UDMSSequenceStepDefinition_SkillTest : public UDMSSequenceStepDefinition
{
	GENERATED_BODY()

protected: 

public: 
	UDMSSequenceStepDefinition_SkillTest(){}

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = SkillTest)
	FDMSSkillTestData DefaultSkillTestData;

	void SetupTargets(UDMSSequenceStep* InstancedStep, TArray<TObjectPtr<AActor>>& Arr, TObjectPtr<UDMSTargetGenerator>& Generator);

	FGameplayTagContainer GetSeqAttributeTag_Tester();

	FGameplayTagContainer GetSeqAttributeTag_TestTarget();

	FGameplayTagContainer GetSeqAttributeTag_Wild();

	UFUNCTION()
	void Progress_ST1(UDMSSequenceStep* InstancedStep);
	UFUNCTION()
	void Progress_ST2(UDMSSequenceStep* InstancedStep);
	UFUNCTION()
	void Progress_ST3(UDMSSequenceStep* InstancedStep);
	UFUNCTION()
	void Progress_ST4(UDMSSequenceStep* InstancedStep);
	UFUNCTION()
	void Progress_ST5(UDMSSequenceStep* InstancedStep);
	UFUNCTION()
	void Progress_ST6(UDMSSequenceStep* InstancedStep);
	UFUNCTION()
	void Progress_ST7(UDMSSequenceStep* InstancedStep);
	UFUNCTION()
	void Progress_ST8(UDMSSequenceStep* InstancedStep);
	UFUNCTION()
	void PostSkillTest(UDMSSequenceStep* InstancedStep);

	virtual TArray<FDMSStepProgressMetaData> GetOrderedProgressData_Implementation() const;
	virtual FGameplayTag GetPureStepTag_Implementation() const;
	virtual FGameplayTagContainer GetStepTag_Implementation(const UDMSSequenceStep* InstancedStep) const;
};

