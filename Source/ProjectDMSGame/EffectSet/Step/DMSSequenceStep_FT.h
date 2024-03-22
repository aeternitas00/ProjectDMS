// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Selector/DMSConfirmWidgetBase.h"
#include "Selector/DMSSelectorManager.h"
#include "Sequence/DMSSequenceStep.h"
#include "DMSSequenceStep_FT.generated.h"

class UDMSSelectorHandle;
class UDMSSelectorRequestGenerator;

/**
 * Skill test is for one tester : one test target 
 */
UCLASS()
class PROJECTDMSGAME_API UDMSSequenceStep_FT : public UDMSSequenceStepDefinition
{
	GENERATED_BODY()

protected: 

public: 
	UDMSSequenceStep_FT(){}

	virtual FGameplayTagContainer GetStepTag_Implementation() const;
};

//UCLASS(Blueprintable, Abstract)
//class UDMSSelector_SkillTest : public UDMSSelectorBase
//{
//	GENERATED_BODY()
//
//public:
//	UPROPERTY(BlueprintReadOnly)
//	TObjectPtr<UDMSSequenceStep_SkillTest> OwnerStep;
//
//	UPROPERTY(BlueprintReadOnly)
//	int CurrentEIIndex;
//
//	UPROPERTY(BlueprintReadOnly)
//	int EIIndexMax;
//
//	TArray<std::function<void()>> Updaters;
//	TArray<float> UsedBonusValues;
//
//
//	UPROPERTY(BlueprintReadOnly)
//	TArray<TObjectPtr<AActor>> TesterCandidates;
//
//	UPROPERTY(BlueprintReadOnly)
//	TArray<TObjectPtr<AActor>> TestTargetCandidates;
//
//	UFUNCTION(BlueprintCallable)
//	void PushResultUpdater(AActor* Tester, AActor* TestTarget = nullptr, float BonusValue = 0.0f);
//
//	UFUNCTION(BlueprintCallable)
//	void PopResultUpdater();
//
//	UFUNCTION(BlueprintCallable)
//	void UpdateSkillTestResult();
//
//	UFUNCTION(BlueprintCallable)
//	float GetUsableBonus(AActor* Tester);
//};