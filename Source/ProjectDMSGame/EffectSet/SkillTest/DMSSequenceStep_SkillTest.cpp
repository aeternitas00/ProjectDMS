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
#include "Player/DMSPlayerController.h"
#include "GameModes/DMSGameStateBase.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Step_SkillTest, "Step.SkillTest");

const FGameplayTag UDMSSequenceStep_SkillTest::SkillBonusTag = FGameplayTag::RequestGameplayTag("Attribute.SavedSkillBonus", false);

UDMSSequenceStep_SkillTest::UDMSSequenceStep_SkillTest()
{
	StepTag = TAG_DMS_Step_SkillTest;
	SkillTestData.TesterGenerator = CreateDefaultSubobject<UDMSTargetGenerator_SourceObject>("TesterGenerator");
	SkillTestData.TestTargetGenerator = CreateDefaultSubobject<UDMSTargetGenerator_SequenceTarget>("TestTargetGenerator");
}

void UDMSSequenceStep_SkillTest::OnStepInitiated()
{
	if (SkillTestWidgetClass == nullptr) { CloseStep(false); return; }
	// == INIT REFERENCES == //

	SetupTargets(Testers,SkillTestData.TesterGenerator);
	SetupTargets(TestTargets, SkillTestData.TestTargetGenerator);
	//AActor* SourceActor = Cast<AActor>(OwnerSequence->GetSourceObject());

	//UDMSGameFunctionLibrary::GetAttributeFromActor(SourceActor, SkillTestData.StatName, SourceValue);

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
	
//	TArray<UDMSConfirmWidgetBase*> Widgets;
//	
//	// To Leader player
//;
//	auto NewWidget = CreateWidget<UDMSConfirmWidgetBase>(UDMSCoreFunctionLibrary::GetDMSGameState()->GetLeaderPlayerController(), SkillTestWidgetClass);
//	NewWidget->CurrentSequence = OwnerSequence;
//	Widgets.Add(NewWidget);
//
//	if (!OwnerSequence->SetupWidgetQueue(Widgets))
//	{
//		DMS_LOG_SIMPLE(TEXT("Can't setup Skill test widget"));
//		ProgressComplete(false);
//		return;
//	}
//
//	OwnerSequence->RunWidgetQueue(
//		[=](UDMSSequence* pSequence) {
//			// Calculate skill test result
//			CalculateSkillTestResult();
//			
//			ProgressComplete();
//		},
//		[=](UDMSSequence* pSequence) {
//			// Skillt test canceled
//			DMS_LOG_SIMPLE(TEXT("Skill Test canceled"));
//
//			ProgressComplete(false);
//		}
//	);

	ProgressComplete();
}

void UDMSSequenceStep_SkillTest::OnAfter_Implementation()
{
	auto SM = UDMSCoreFunctionLibrary::GetDMSSequenceManager(); check(SM);

	DMS_LOG_SCREEN(TEXT("==-- SkillTestStep_AFTER [ Depth : %d ] --=="), SM->GetDepth(OwnerSequence));

	ProgressComplete();
}

void UDMSSequenceStep_SkillTest::SetupTargets(TArray<TObjectPtr<UObject>>& Arr, TObjectPtr<UDMSTargetGenerator>& Generator)
{
	for (auto& TObj : Generator->GetTargets(OwnerSequence->GetSourceObject(), OwnerSequence))
	{
		if (!TObj->IsA<AActor>() || !TObj->Implements<UDMSEffectorInterface>())	continue;

		Arr.Add(TObj);
	}
}

float UDMSSequenceStep_SkillTest::CalculateSkillTestResult()
{

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
	return 0.0f;
}
//
//UDMSDataObjectSet* UDMSSelector_SkillTest::MakeOutputData_Implementation()
//{
//	UDMSDataObjectSet* rv = NewObject<UDMSDataObjectSet>(CurrentSequence);
//
//	rv->SetData(UDMSSequenceStep_SkillTest::SkillBonusTag, OutBonusValue, true);
//
//	return rv;
//}
