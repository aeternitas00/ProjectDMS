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

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FGameplayTagContainer StatName;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (EditCondition = "!bTestToStaticValue", EditConditionHides))
	FGameplayTagContainer TargetStatName;

	// Get stat from who? ( only one )
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced)
	TObjectPtr<UDMSTargetGenerator> TesterGenerator;

	// Get stat from who? ( only one )
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced, meta = (EditCondition = "!bTestToStaticValue", EditConditionHides))
	TObjectPtr<UDMSTargetGenerator> TestTargetGenerator;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bTestToStaticValue;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (EditCondition = "bTestToStaticValue", EditConditionHides))
	float StaticTargetValue;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool CanUseBonusStat;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool IsCommittable;

	// 최종적인 이펙트 적용 대상 마다 같은 테스트를 반복할지 결정.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool TestByEachApplyTarget;

	// 스킬 결과 값의 반전 ( 실패시 진행하는 효과에 유용 )
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool ReverseTestResult;

	// ENum TestMethod ?
	// 1. Test with ( Source[StatName] + Static Value )
	// 2. Test with ( Source[StatName] + Target[StatName] )
	// 3. Custom? ( Multiple stat )

};


/**
 * Skill test is for one tester : one test target 
 */
UCLASS()
class PROJECTDMSGAME_API UDMSSequenceStep_SkillTest : public UDMSSequenceStep
{
	GENERATED_BODY()

protected: 

public: 
	UDMSSequenceStep_SkillTest();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FDMSSkillTestData SkillTestData;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UDMSSelector_SkillTest> SkillTestWidgetClass;

	static const FGameplayTag SkillBonusTag;

	void SetupTargets(TArray<TObjectPtr<AActor>>& Arr, TObjectPtr<UDMSTargetGenerator>& Generator);
	//virtual void OnStepInitiated() override;

	//virtual void OnBefore_Implementation() override;
	//virtual void OnDuring_Implementation() override;
	//virtual void OnAfter_Implementation() override;

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	float CalculateSkillTestResult(AActor* Tester, AActor* TestTarget,float BonusValue);
	virtual float CalculateSkillTestResult_Implementation(AActor* Tester, AActor* TestTarget,float BonusValue);
	//...
	// Branch for failed Skill test?
	UFUNCTION(BlueprintNativeEvent)
	void OnSkillTestCompleted();
	virtual void OnSkillTestCompleted_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void OnSkillTestFailed();
	virtual void OnSkillTestFailed_Implementation();

	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool IsTestByEachApplyTarget() const;

	virtual FGameplayTagContainer GetStepTag_Implementation() const;
};

// 변종 테스트의 경우는 상속 받아서 변경하는 것으로 구현하는게 맞을듯.


UCLASS(Blueprintable, Abstract)
class PROJECTDMSGAME_API UDMSSelector_SkillTest : public UDMSSelectorBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDMSSequenceStep_SkillTest> OwnerStep;

	UPROPERTY(BlueprintReadOnly)
	int CurrentEIIndex;

	UPROPERTY(BlueprintReadOnly)
	int EIIndexMax;

	TArray<std::function<void()>> Updaters;
	TArray<float> UsedBonusValues;


	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<AActor>> TesterCandidates;

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<AActor>> TestTargetCandidates;

	UFUNCTION(BlueprintCallable)
	void PushResultUpdater(AActor* Tester, AActor* TestTarget = nullptr, float BonusValue = 0.0f);

	UFUNCTION(BlueprintCallable)
	void PopResultUpdater();

	UFUNCTION(BlueprintCallable)
	void UpdateSkillTestResult();

	UFUNCTION(BlueprintCallable)
	float GetUsableBonus(AActor* Tester);
};



/**
* Skill test is for one tester : one test target 
*/
UCLASS()
class PROJECTDMSGAME_API UDMSSequenceStepDefinition_SkillTest : public UDMSSequenceStepDefinition
{
	GENERATED_BODY()

protected: 

public: 
	UDMSSequenceStepDefinition_SkillTest(){}

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FDMSSkillTestData SkillTestData;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UDMSSelector_SkillTest> SkillTestWidgetClass;

	void SetupTargets(TArray<TObjectPtr<AActor>>& Arr, TObjectPtr<UDMSTargetGenerator>& Generator){}

	UFUNCTION()
	void Progress_ST1(UDMSSequenceStep* InstancedStep){}
	UFUNCTION()
	void Progress_ST2(UDMSSequenceStep* InstancedStep){}
	UFUNCTION()
	void Progress_ST3(UDMSSequenceStep* InstancedStep){}
	UFUNCTION()
	void Progress_ST4(UDMSSequenceStep* InstancedStep){}
	UFUNCTION()
	void Progress_ST5(UDMSSequenceStep* InstancedStep){}
	UFUNCTION()
	void Progress_ST6(UDMSSequenceStep* InstancedStep){}
	UFUNCTION()
	void Progress_ST7(UDMSSequenceStep* InstancedStep){}
	UFUNCTION()
	void Progress_ST8(UDMSSequenceStep* InstancedStep){}

	virtual FGameplayTag GetPureStepTag_Implementation() const;
	virtual FGameplayTagContainer GetStepTag_Implementation() const;
	virtual bool GetProgressOps_Implementation(const FGameplayTag& ProgressTag,TArray<FProgressExecutor>& OutExecutor);
};
