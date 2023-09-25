// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "EffectSet/DMSEffect_SkillTest.h"
#include "Sequence/DMSSequenceStep.h"
#include "DMSSequenceStep_SkillTest.generated.h"

class ADMSCardBase;

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Step_SkillTest)


USTRUCT(BlueprintType)
struct FDMSSkillTestData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FGameplayTag StatName;

	// Get stat from who?
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced)
	TObjectPtr<UDMSTargetGenerator> TesterTargetor;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bTestToStaticValue;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (EditCondition = "bTestToStaticValue", EditConditionHides))
	float StaticTargetValue;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool CanCommitable;

	// ENum TestMethod ?
	// 1. Test with ( Source[StatName] + Static Value )
	// 2. Test with ( Source[StatName] + Target[StatName] )
	// 3. Custom? ( Multiple stat )

};

/**
 * 
 */
UCLASS()
class PROJECTDMSGAME_API UDMSSequenceStep_SkillTest : public UDMSSequenceStep
{
	GENERATED_BODY()

protected:
	TArray<TObjectPtr<AActor>> Testers;
	float SourceValue;
	
public:
	UDMSSequenceStep_SkillTest();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FDMSSkillTestData SkillTestData;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Effect, meta = (EditCondition = "bIsUsingSelector", EditConditionHides))
	TSubclassOf<UDMSSelector_SkillTest> SkillTestWidgetClass;

	static const FGameplayTag SkillBonusTag;

	virtual void OnStepInitiated() override;

	virtual void OnBefore_Implementation() override;
	virtual void OnDuring_Implementation() override;
	virtual void OnAfter_Implementation() override;
};


UCLASS(Blueprintable, Abstract)
class UDMSSelector_SkillTest : public UDMSConfirmWidgetBase
{
	GENERATED_BODY()


public:
	/**
	 *
	 */
	UPROPERTY(BlueprintReadOnly)
	FDMSSkillTestData SkillTestData;

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<ADMSCardBase>> CommitableCards;

	UPROPERTY(BlueprintReadWrite)
	float OutBonusValue;

	//virtual void OnPopupSelector_Implementation() override;
	//virtual void OnCloseSelector_Implementation() override;

	//virtual UDMSDataObjectSet* MakeOutputData_Implementation();
	//virtual	bool SetupWidget_Implementation();

	//friend class UDMSEffect_SkillTest;
};