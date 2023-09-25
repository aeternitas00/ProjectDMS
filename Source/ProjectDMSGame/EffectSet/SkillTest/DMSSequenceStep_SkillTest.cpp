// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectSet/SkillTest/DMSSequenceStep_SkillTest.h"
#include "Effect/DMSEffectInstance.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Library/DMSGameFunctionLibrary.h"
#include "Player/DMSPlayerController.h"
#include "Sequence/DMSSequence.h"
#include "Card/DMSCardDefinition.h"
#include "Card/DMSCardManagerComponent.h"
#include "Conditions/DMSConditionObject.h"
#include "Library/DMSCoreFunctionLibrary.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Step_SkillTest, "Step.SkillTest");

const FGameplayTag UDMSSequenceStep_SkillTest::SkillBonusTag = FGameplayTag::RequestGameplayTag("Attribute.SavedSkillBonus", false);

UDMSSequenceStep_SkillTest::UDMSSequenceStep_SkillTest()
{
	StepTag = TAG_DMS_Step_SkillTest;
}

void UDMSSequenceStep_SkillTest::OnStepInitiated()
{
	// == INIT REFERENCES == //
	for (auto& TestObj : SkillTestData.TesterTargetor->GetTargets(OwnerSequence->GetSourceObject(), OwnerSequence))
	{
		AActor* Tester = Cast<AActor>(TestObj);
		if (Tester != nullptr) Testers.Add(Tester);
	}

	AActor* SourceActor = Cast<AActor>(OwnerSequence->GetSourceObject());

	UDMSGameFunctionLibrary::GetAttributeFromActor(SourceActor, SkillTestData.StatName, SourceValue);

	UDMSSequenceStep::OnStepInitiated();
}

void UDMSSequenceStep_SkillTest::OnBefore_Implementation()
{
	auto SM = UDMSCoreFunctionLibrary::GetDMSSequenceManager(); check(SM);

	DMS_LOG_SCREEN(TEXT("==-- SkillTestStep_BEFORE [ Depth : %d ] --=="), SM->GetDepth(OwnerSequence));
	ProgressComplete();
}

void UDMSSequenceStep_SkillTest::OnDuring_Implementation()
{
	auto SM = UDMSCoreFunctionLibrary::GetDMSSequenceManager(); check(SM);

	DMS_LOG_SCREEN(TEXT("==-- SkillTestStep_DURING [ Depth : %d ] --=="), SM->GetDepth(OwnerSequence));


	//	iEI->DataSet->GetValidDataValue<float>(SkillBonusName,SkillBonus);
	//
	//	AActor* Target = Cast<AActor>(iEI->GetApplyTarget()->GetObject());
	//
	//	if (Target == nullptr){OnWorkCompleted.ExecuteIfBound(false);return;}
	//
	//	// if Target don't have that att => Target Value is 0.0f 
	//	float SkillTestResult=0.0f;
	//	if (SkillTestData.bTestToStaticValue)
	//	{
	//		SkillTestResult = SourceValue + SkillBonus - SkillTestData.StaticTargetValue;
	//	}
	//	else {
	//		float TargetValue = 0.0f;
	//		UDMSGameFunctionLibrary::GetAttributeFromActor(Target, SkillTestData.StatName, TargetValue);
	//		SkillTestResult = SourceValue + SkillBonus - TargetValue;
	//	}
	//
	//	iEI->DataSet->SetData(OutDataKey, SkillTestResult, true);
	//
	ProgressComplete();
}

void UDMSSequenceStep_SkillTest::OnAfter_Implementation()
{
	auto SM = UDMSCoreFunctionLibrary::GetDMSSequenceManager(); check(SM);

	DMS_LOG_SCREEN(TEXT("==-- SkillTestStep_AFTER [ Depth : %d ] --=="), SM->GetDepth(OwnerSequence));

	ProgressComplete();
}
