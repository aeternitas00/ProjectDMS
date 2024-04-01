// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectSet/SkillTest/DMSSequenceStep_SkillTest.h"
#include "Effect/DMSEffectInstance.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Library/DMSGameFunctionLibrary.h"
#include "Player/DMSPlayerController.h"
#include "Card/DMSCardDefinition.h"
#include "Card/DMSCardManagerComponent.h"
#include "Conditions/DMSConditionObject.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Library/DMSDataObjectHelperLibrary.h"
#include "Player/DMSPlayerController.h"
#include "GameModes/DMSGameStateBase.h"
#include "Selector/DMSDecisionDefinition.h"
#include "Attribute/DMSAttribute.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Step_SkillTest, "Step.SkillTest");
UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Step_SkillTest_Committable, "Step.SkillTest.Committable");
UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Step_SkillTest_Data_TestDiffModifier, "Step.SkillTest.Data.TestDiffModifier");
UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Step_SkillTest_Data_TestResult, "Step.SkillTest.Data.TestResult");

const FGameplayTag UDMSSequenceStep_SkillTest::SkillBonusTag = FGameplayTag::RequestGameplayTag("Attribute.SavedSkillBonus", false);

UDMSSequenceStep_SkillTest::UDMSSequenceStep_SkillTest()
{
	//StepTag = TAG_DMS_Step_SkillTest;
	//SkillTestData.TesterGenerator = CreateDefaultSubobject<UDMSTargetGenerator_SourceObject>("TesterGenerator");
	//SkillTestData.TestTargetGenerator = CreateDefaultSubobject<UDMSTargetGenerator_SequenceTarget>("TestTargetGenerator");
}

//void UDMSSequenceStep_SkillTest::OnStepInitiated()
//{
//	//if (SkillTestWidgetClass == nullptr) { CloseStep(false); return; }
//
//	//UDMSSequenceStep::OnStepInitiated();
//}
//
//void UDMSSequenceStep_SkillTest::OnBefore_Implementation()
//{
//	auto SM = UDMSCoreFunctionLibrary::GetDMSSequenceManager(this); check(SM);
//
//	DMS_LOG_SCREEN(TEXT("==-- SkillTestStep_BEFORE [ Depth : %d ] --=="), SM->GetDepth(OwnerSequence));
//	ProgressComplete();
//}
//
//void UDMSSequenceStep_SkillTest::OnDuring_Implementation()
//{
//	// Behavior
//	auto GS = UDMSCoreFunctionLibrary::GetDMSGameState(this); check(GS);
//	auto SM = UDMSCoreFunctionLibrary::GetDMSSequenceManager(this); check(SM);
//	auto EH = UDMSCoreFunctionLibrary::GetDMSEffectHandler(this); check(EH);
//	auto SelM = UDMSCoreFunctionLibrary::GetDMSSelectorManager(this); check(SelM);
//
//	DMS_LOG_SCREEN(TEXT("==-- SkillTestStep_DURING [ Depth : %d ] --=="), SM->GetDepth(OwnerSequence));
//
//	// Select Tester 
//	FDMSSelectorRequestForm SkillTestForm;
//	SkillTestForm.SelectorClass = SkillTestWidgetClass;
//
//	auto Handle = SelM->RequestCreateSelector(SkillTestForm);
//	auto SourcePlayer = Cast<IDMSEffectorInterface>(OwnerSequence->GetSourceObject())->GetOwningPlayerController();
//	Handle->CreateSelectorWidget(SourcePlayer);
//	UDMSSelector_SkillTest* SkillTestWidget = Cast<UDMSSelector_SkillTest>(Handle->Widget);
//
//	// Custom widget initialize
//	SetupTargets(SkillTestWidget->TesterCandidates,SkillTestData.TesterGenerator);
//	if (!SkillTestData.bTestToStaticValue)
//		SetupTargets(SkillTestWidget->TestTargetCandidates,SkillTestData.TestTargetGenerator);
//	SkillTestWidget->EIIndexMax = 1; SkillTestWidget->CurrentEIIndex = 0; 
//	if ( IsTestByEachApplyTarget() ){	
//		SkillTestWidget->Updaters.Reserve(OwnerSequence->GetAllEIs().Num());
//		SkillTestWidget->UsedBonusValues.Reserve(OwnerSequence->GetAllEIs().Num());
//		SkillTestWidget->EIIndexMax = OwnerSequence->GetAllEIs().Num();
//	}
//	
//	SkillTestWidget->OwnerStep = this;
//
//	Handle->SetupDelegates([this,SkillTestWidget](){
//		// WidgetCompleted
//		SkillTestWidget->CloseSelector();
//		UDMSDataObjectSet* DataSet = OwnerSequence->SequenceDatas;
//		float Result = MAX_FLT;
//		if ( !DataSet->GetValidDataValue<float>(TAG_DMS_Step_SkillTest_Data_TestResult, Result) )
//			{ProgressComplete(false);return;}
//
//		if (Result>=0.0f)	OnSkillTestCompleted();	
//		else OnSkillTestFailed();
//	},	[this,SkillTestWidget](){
//		// WidgetCanceled
//		SkillTestWidget->CloseSelector();
//		ProgressComplete(false);
//	});
//	Handle->SetupSelector();	
//	Handle->RunSelector();	
//}
//
//void UDMSSequenceStep_SkillTest::OnAfter_Implementation()
//{
//	auto SM = UDMSCoreFunctionLibrary::GetDMSSequenceManager(this); check(SM);
//
//	DMS_LOG_SCREEN(TEXT("==-- SkillTestStep_AFTER [ Depth : %d ] --=="), SM->GetDepth(OwnerSequence));
//
//	ProgressComplete();
//}

void UDMSSequenceStep_SkillTest::OnSkillTestCompleted_Implementation()
{
	//ProgressComplete(true);
}

void UDMSSequenceStep_SkillTest::OnSkillTestFailed_Implementation()
{
	// Implements branch for failed skill test.
	//ProgressComplete(false);
}

inline bool UDMSSequenceStep_SkillTest::IsTestByEachApplyTarget() const { return SkillTestData.TestByEachApplyTarget && OwnerSequence->IsTargetted(); }

inline FGameplayTagContainer UDMSSequenceStep_SkillTest::GetStepTag_Implementation() const {
	return SkillTestData.IsCommittable ? FGameplayTagContainer(TAG_DMS_Step_SkillTest_Committable) : FGameplayTagContainer(TAG_DMS_Step_SkillTest); 
}



float UDMSSequenceStep_SkillTest::CalculateSkillTestResult_Implementation(AActor* Tester, AActor* TestTarget, float BonusValue)
{
	float SkillTestResult=0.0f;
	float SourceValue=0.0f;
	UDMSGameFunctionLibrary::GetAttributeFromActor(Tester,SkillTestData.StatName);

	// if Target don't have that att => Target Value is 0.0f 
	if (SkillTestData.bTestToStaticValue) {
		SkillTestResult = SourceValue + BonusValue - SkillTestData.StaticTargetValue;
	}
	else {
		float TargetValue = 0.0f;
		UDMSGameFunctionLibrary::GetAttributeFromActor(TestTarget, SkillTestData.StatName);
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
		if (CastedObj==nullptr || !CastedObj->Implements<UDMSEffectorInterface>()
			|| CastedObj->GetComponentByClass<UDMSAttributeComponent>() == nullptr)	continue;

		Arr.Add(Cast<AActor>(TObj));
	}
}

void UDMSSelector_SkillTest::PushResultUpdater(AActor* Tester, AActor* TestTarget,float BonusValue)
{
	if (CurrentEIIndex>=EIIndexMax) return;

	float SkillTestResult=OwnerStep->CalculateSkillTestResult(Tester,TestTarget,BonusValue);

	// 셀렉트 종료시 일괄 처리.
	if (OwnerStep->IsTestByEachApplyTarget()){
		Updaters.Push([=]() __declspec(noinline) {
			DMS_LOG_SCREEN(TEXT("Update Data"));
			OwnerStep->OwnerSequence->GetAllEIs()[CurrentEIIndex]->DataSet->SetData(TAG_DMS_Step_SkillTest_Data_TestResult, SkillTestResult);
		});
	}
	else{
		Updaters.Push( [=]() __declspec(noinline) {
			DMS_LOG_SCREEN(TEXT("Update Data"));
			OwnerStep->OwnerSequence->SequenceDatas->SetData(TAG_DMS_Step_SkillTest_Data_TestResult, SkillTestResult);
		});
	}
	UsedBonusValues.Push(BonusValue);
	CurrentEIIndex++;
}

void UDMSSelector_SkillTest::PopResultUpdater()
{
	if(CurrentEIIndex <= 0) return;
	Updaters.Pop(false);
	UsedBonusValues.Pop(false);
	CurrentEIIndex--;
}

void UDMSSelector_SkillTest::UpdateSkillTestResult()
{
	for(auto& Updater : Updaters)
		Updater();
}

float UDMSSelector_SkillTest::GetUsableBonus(AActor* Tester)
{
	auto Attribute = Cast<UDMSAttributeValue_Numeric>(Tester->GetComponentByClass<UDMSAttributeComponent>()->GetAttribute(FGameplayTagContainer(UDMSSequenceStep_SkillTest::SkillBonusTag)));
	float rv= Attribute ? Attribute->GetValue() : 0.0f;
	for(auto& UsedBonus : UsedBonusValues)
		rv-=UsedBonus;
	
	return rv;
}

void UDMSSequenceStepDefinition_SkillTest::Progress_ST1(UDMSSequenceStep* InstancedStep)
{
	FGameplayTagContainer SkillTestAttribute = DefaultSkillTestData.StatName;
	SkillTestAttribute.AddTagFast(FGameplayTag::RequestGameplayTag("Step.Arkham.SkillTest"));
	FGameplayTagContainer TestTargetAttribute;
	if(!DefaultSkillTestData.bTestToStaticValue){
		TestTargetAttribute = DefaultSkillTestData.TargetStatName;
		TestTargetAttribute.AddTagFast(FGameplayTag::RequestGameplayTag("Step.Arkham.SkillTest"));
	}
	else {
		TestTargetAttribute=FGameplayTag::RequestGameplayTag("Step.Arkham.SkillTest").GetSingleTagContainer();
	}
	for(auto& EI : InstancedStep->OwnerSequence->GetAllEIs()){
		auto AttComp = EI->GetComponentByClass<UDMSAttributeComponent>();
		AttComp->MakeAttribute(SkillTestAttribute,UDMSAttributeValue_Numeric::StaticClass());	
		AttComp->MakeAttribute(TestTargetAttribute,UDMSAttributeValue_Numeric::StaticClass());
	}
	BroadcastProgress(InstancedStep,FName(NAME_None));
	// FT WINDOW
}

void UDMSSequenceStepDefinition_SkillTest::Progress_ST2(UDMSSequenceStep* InstancedStep)
{
	// Commit cards from hand to skill test.

	// SELECTOR OR JUST BROADCASTING?
	// 일단은 그냥 브로드캐스팅으로.
	BroadcastProgress(InstancedStep,FName(NAME_None));

	// FT WINDOW
}

void UDMSSequenceStepDefinition_SkillTest::Progress_ST3(UDMSSequenceStep* InstancedStep)
{
	// Reveal chaos token.

	// run sequence of draw chaos token?


	InstancedStep->ProgressEnd(true);
}

void UDMSSequenceStepDefinition_SkillTest::Progress_ST4(UDMSSequenceStep* InstancedStep)
{
	// Resolve chaos syombol effects.
	InstancedStep->ProgressEnd(true);
}

void UDMSSequenceStepDefinition_SkillTest::Progress_ST5(UDMSSequenceStep* InstancedStep)
{
	// Determine investigator's modified skillvalue
	InstancedStep->ProgressEnd(true);
}

void UDMSSequenceStepDefinition_SkillTest::Progress_ST6(UDMSSequenceStep* InstancedStep)
{
	// Determine success/failure of skill test.
	InstancedStep->ProgressEnd(true);
}

void UDMSSequenceStepDefinition_SkillTest::Progress_ST7(UDMSSequenceStep* InstancedStep)
{
	// Apply skill test results.
	InstancedStep->ProgressEnd(true);
}

void UDMSSequenceStepDefinition_SkillTest::Progress_ST8(UDMSSequenceStep* InstancedStep)
{
	// skill test ends.
	InstancedStep->ProgressEnd(true);
}

FGameplayTag UDMSSequenceStepDefinition_SkillTest::GetPureStepTag_Implementation() const
{
	return FGameplayTag::RequestGameplayTag("Step.Arkham.SkillTest");
}

FGameplayTagContainer UDMSSequenceStepDefinition_SkillTest::GetStepTag_Implementation() const
{
	FGameplayTagContainer rv;
	rv.AddTag(GetPureStepTag());
	// Get tags from testing context
	return rv;
}

bool UDMSSequenceStepDefinition_SkillTest::GetProgressOps_Implementation(const FGameplayTag& ProgressTag, TArray<FProgressExecutor>& OutExecutor)
{
	FGameplayTag BaseTag = FGameplayTag::RequestGameplayTag("Step.Arkham.SkillTest");
	if(ProgressTag.MatchesTagExact(BaseTag)) {
		for(int i=1;i<=8;i++){
			FName FunctionName(FString::Printf(TEXT("Progres_ST%d"),i));
			FGameplayTag ExactTag(FGameplayTag::RequestGameplayTag(*FString::Printf(TEXT("Step.Arkham.SkillTest.ST%d"),i)));
			OutExecutor.Add(FProgressExecutor(this,ExactTag,FunctionName));
		}
		return true;
	}
	else if(ProgressTag.MatchesTag(BaseTag)){
		// Alt :: Using native defined tag 
		TArray<FString> Words;	ProgressTag.ToString().ParseIntoArray(Words,TEXT("."));
		FName FunctionName(FString::Printf(TEXT("Progress_%s"), *Words.Last()));
		if ( FindFunction(FunctionName) == nullptr ) return false;

		OutExecutor.Add(FProgressExecutor(this,ProgressTag,FunctionName));		
	}
	return false;
}
