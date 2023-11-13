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
#include "Library/DMSDataObjectHelperLibrary.h"
#include "Player/DMSPlayerController.h"
#include "GameModes/DMSGameStateBase.h"
#include "Selector/DMSDecisionDefinition.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Step_SkillTest, "Step.SkillTest");
UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Step_SkillTest_Committable, "Step.SkillTest.Committable");
UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Step_SkillTest_Data_TestDiffModifier, "Step.SkillTest.Data.TestDiffModifier");
UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Step_SkillTest_Data_TestResult, "Step.SkillTest.Data.TestResult");

const FGameplayTag UDMSSequenceStep_SkillTest::SkillBonusTag = FGameplayTag::RequestGameplayTag("Attribute.SavedSkillBonus", false);

UDMSSequenceStep_SkillTest::UDMSSequenceStep_SkillTest()
{
	StepTag = TAG_DMS_Step_SkillTest;
	//SkillTestData.TesterGenerator = CreateDefaultSubobject<UDMSTargetGenerator_SourceObject>("TesterGenerator");
	//SkillTestData.TestTargetGenerator = CreateDefaultSubobject<UDMSTargetGenerator_SequenceTarget>("TestTargetGenerator");
}

void UDMSSequenceStep_SkillTest::OnStepInitiated()
{
	if (SkillTestWidgetClass == nullptr) { CloseStep(false); return; }

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
	// Behavior
	auto GS = UDMSCoreFunctionLibrary::GetDMSGameState(); check(GS);
	auto SM = UDMSCoreFunctionLibrary::GetDMSSequenceManager(); check(SM);
	auto EH = UDMSCoreFunctionLibrary::GetDMSEffectHandler(); check(EH);
	auto SelM = UDMSCoreFunctionLibrary::GetDMSSelectorManager(); check(SelM);

	DMS_LOG_SCREEN(TEXT("==-- SkillTestStep_DURING [ Depth : %d ] --=="), SM->GetDepth(OwnerSequence));

	// Select Tester 
	FDMSSelectorRequestForm SkillTestForm;
	SkillTestForm.SelectorClass = SkillTestWidgetClass;

	auto Handle = SelM->RequestCreateSelector(SkillTestForm);
	auto SourcePlayer = Cast<IDMSEffectorInterface>(OwnerSequence->GetSourceObject())->GetOwningPlayerController();
	Handle->CreateSelectorWidget(SourcePlayer);
	UDMSSelector_SkillTest* SkillTestWidget = Cast<UDMSSelector_SkillTest>(Handle->Widget);

	// Custom widget initialize
	SetupTargets(SkillTestWidget->TesterCandidates,SkillTestData.TesterGenerator);
	if (SkillTestData.bTestToStaticValue)
		SetupTargets(SkillTestWidget->TestTargetCandidates,SkillTestData.TestTargetGenerator);
	SkillTestWidget->OwnerStep = this;

	Handle->SetupDelegates([this](){
		// WidgetCompleted
		UDMSDataObjectSet* DataSet = OwnerSequence->SequenceDatas;
		float Result = MAX_FLT;
		if ( !DataSet->GetValidDataValue<float>(TAG_DMS_Step_SkillTest_Data_TestResult, Result) )
			ProgressComplete(false);

		if (Result>=0.0f)	OnSkillTestCompleted();	
		else OnSkillTestFailed();
	},	[this](){
		// WidgetCanceled
		ProgressComplete(false);
	});

	Handle->RunSelector();	
}

void UDMSSequenceStep_SkillTest::OnSkillTestCompleted_Implementation()
{
	ProgressComplete(true);
}

void UDMSSequenceStep_SkillTest::OnSkillTestFailed_Implementation()
{
	// Implements branch for failed skill test.
	ProgressComplete(false);
}

void UDMSSequenceStep_SkillTest::OnAfter_Implementation()
{
	auto SM = UDMSCoreFunctionLibrary::GetDMSSequenceManager(); check(SM);

	DMS_LOG_SCREEN(TEXT("==-- SkillTestStep_AFTER [ Depth : %d ] --=="), SM->GetDepth(OwnerSequence));

	ProgressComplete();
}

float UDMSSequenceStep_SkillTest::CalculateSkillTestResult_Implementation(AActor* Tester, AActor* TestTarget, float BonusValue)
{
	float SkillTestResult=0.0f;
	float SourceValue=0.0f;
	UDMSGameFunctionLibrary::GetAttributeFromActor(Tester,SkillTestData.StatName,SourceValue);

	// if Target don't have that att => Target Value is 0.0f 
	if (SkillTestData.bTestToStaticValue) {
		SkillTestResult = SourceValue + BonusValue - SkillTestData.StaticTargetValue;
	}
	else {
		float TargetValue = 0.0f;
		UDMSGameFunctionLibrary::GetAttributeFromActor(TestTarget, SkillTestData.StatName, TargetValue);
		SkillTestResult = SourceValue + BonusValue - TargetValue;
	}

	SkillTestResult = SkillTestData.ReverseTestResult ? SkillTestResult*-1.0f : SkillTestResult;

	return SkillTestResult;
}



void UDMSSequenceStep_SkillTest::SetupTargets(TArray<TObjectPtr<AActor>>& Arr, TObjectPtr<UDMSTargetGenerator>& Generator)
{
	for (auto& TObj : Generator->GetTargets(OwnerSequence->GetSourceObject(), OwnerSequence))
	{
		AActor* CastedObj = Cast<AActor>(TObj);
		if (CastedObj==nullptr || CastedObj->Implements<UDMSEffectorInterface>()
			|| CastedObj->GetComponentByClass<UDMSAttributeComponent>() == nullptr)	continue;

		Arr.Add(Cast<AActor>(TObj));
	}
}

void UDMSSelector_SkillTest::UpdateSkillTestResult(AActor* Tester, AActor* TestTarget,float BonusValue)
{
	float SkillTestResult=OwnerStep->CalculateSkillTestResult(Tester,TestTarget,BonusValue);

	// 셀렉트 종료시 일괄 처리.
	if (OwnerStep->SkillTestData.TestByEachApplyTarget){
		OwnerHandle->OnSelectCompleted.AddLambda([=](){
			OwnerStep->OwnerSequence->GetAllEIs()[CurrentEIIndex]->DataSet->SetData(TAG_DMS_Step_SkillTest_Data_TestResult, SkillTestResult);
		});
	}
	else{
		OwnerHandle->OnSelectCompleted.AddLambda([=](){
			OwnerStep->OwnerSequence->SequenceDatas->SetData(TAG_DMS_Step_SkillTest_Data_TestResult, SkillTestResult);
		});
	}
}
