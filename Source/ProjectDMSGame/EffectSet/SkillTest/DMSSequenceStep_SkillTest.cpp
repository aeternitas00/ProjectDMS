// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectSet/SkillTest/DMSSequenceStep_SkillTest.h"
#include "Effect/DMSEffectInstance.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Attribute/DMSAttribute.h"
#include "Attribute/DMSAttributeValue_Object.h"
#include "Attribute/DMSAttributeValue_Numeric.h"
#include "Library/DMSGameFunctionLibrary.h"
#include "Player/DMSPlayerController.h"
#include "Card/DMSCardDefinition.h"
#include "Card/DMSCardManagerComponent.h"
#include "Conditions/DMSConditionObject.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Library/DMSDataObjectHelperLibrary.h"
#include "GameModes/DMSGameState.h"
#include "Selector/DMSDecisionDefinition.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Step_SkillTest, "Step.SkillTest");
UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Step_SkillTest_Committable, "Step.SkillTest.Committable");
UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Step_SkillTest_Data_TestDiffModifier, "Step.SkillTest.Data.TestDiffModifier");

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Step_SkillTest_Data_TestResult, "Step.Arkham.SkillTest.TestResult");

void UDMSSequenceStepDefinition_SkillTest::SetupTargets(UDMSSequenceStep* InstancedStep, TArray<TObjectPtr<AActor>>& Arr, TObjectPtr<UDMSTargetGenerator>& Generator)
{
	for (auto& TObj : Generator->GetTargets(InstancedStep->OwnerSequence->GetSourceObject(), InstancedStep->OwnerSequence))
	{
		AActor* CastedObj = Cast<AActor>(TObj);
		if (CastedObj==nullptr || !CastedObj->Implements<UDMSEffectorInterface>()
			/*|| CastedObj->GetComponentByClass<UDMSAttributeComponent>() == nullptr*/)	continue;

		Arr.Add(Cast<AActor>(TObj));
	}
}

FGameplayTagContainer UDMSSequenceStepDefinition_SkillTest::GetSeqAttributeTag_Tester()
{
	return FGameplayTag::RequestGameplayTag("Step.Arkham.SkillTest.SkillValue").GetSingleTagContainer();
}

FGameplayTagContainer UDMSSequenceStepDefinition_SkillTest::GetSeqAttributeTag_TestTarget()
{
	return FGameplayTag::RequestGameplayTag("Step.Arkham.SkillTest.Difficulty").GetSingleTagContainer();
}

void UDMSSequenceStepDefinition_SkillTest::Progress_ST1(UDMSSequenceStep* InstancedStep)
{
	DMS_LOG_SIMPLE(TEXT("Progress_ST1"));
	FGameplayTagContainer SkillTestAttribute = GetSeqAttributeTag_Tester();	
	FGameplayTagContainer TestTargetAttribute = GetSeqAttributeTag_TestTarget();

	auto AttComp = InstancedStep->OwnerSequence->GetComponentByClass<UDMSAttributeComponent>();

	auto SeqAttTesterValue = AttComp->MakeAttribute(SkillTestAttribute,UDMSAttributeValue_Numeric::StaticClass(), true);
	auto SeqAttTestTargetValue = AttComp->MakeAttribute(TestTargetAttribute,UDMSAttributeValue_Numeric::StaticClass(), true);
	auto SeqTesterValue = SeqAttTesterValue ? Cast<UDMSAttributeValue_Numeric>(SeqAttTesterValue->AttributeValue) : nullptr;	
	auto SeqTestTargetValue = SeqAttTestTargetValue ? Cast<UDMSAttributeValue_Numeric>(SeqAttTestTargetValue->AttributeValue) : nullptr;
	SeqTestTargetValue->SetValue(DefaultSkillTestData.DefaultTargetValue);
	if(!SeqTesterValue || !SeqTestTargetValue) {
		InstancedStep->ProgressEnd(false);
		return;
	}
	auto SeqAttTester = AttComp->GetAttribute(FGameplayTag::RequestGameplayTag("Step.Arkham.SkillTest.Tester").GetSingleTagContainer(), true);
	UDMSAttributeValue_Object* AttVTester = nullptr;
	AActor* Tester = nullptr;
	if(	SeqAttTester ) AttVTester = Cast<UDMSAttributeValue_Object>(SeqAttTester->AttributeValue);
	if( AttVTester ) Tester = Cast<AActor>(AttVTester->GetValue()[0]);
	// No specific tester ==> Use source object as tester.
	if( !Tester ) Tester = InstancedStep->OwnerSequence->GetSourcePlayer();

	UDMSAttributeValue_Numeric* TesterAttribute = Cast<UDMSAttributeValue_Numeric>(Tester->GetComponentByClass<UDMSAttributeComponent>()->GetAttribute(DefaultSkillTestData.StatName)->AttributeValue);
	if ( TesterAttribute ) SeqTesterValue->SetValue(TesterAttribute->GetValue());
	
	auto SeqAttTestTarget = AttComp->GetAttribute(FGameplayTag::RequestGameplayTag("Step.Arkham.SkillTest.TestTarget").GetSingleTagContainer(), true);
	UDMSAttributeValue_Object* AttVTestTarget  = nullptr;
	AActor* TestTarget  = nullptr;
	if(	SeqAttTestTarget ) AttVTestTarget = Cast<UDMSAttributeValue_Object>(SeqAttTestTarget->AttributeValue);
	if( AttVTestTarget ) TestTarget = Cast<AActor>(AttVTestTarget->GetValue()[0]);
	// No specific test target ==> Use first sequence's main target as test target.
	if( !TestTarget ) TestTarget = Cast<AActor>(InstancedStep->OwnerSequence->GetTargets()[0].GetObject());
	
	UDMSAttributeValue_Numeric* TestTargetAttVal  = Cast<UDMSAttributeValue_Numeric>(TestTarget->GetComponentByClass<UDMSAttributeComponent>()->GetAttribute(DefaultSkillTestData.TargetStatName)->AttributeValue);
	if ( TestTargetAttVal ) SeqTestTargetValue->SetValue(TestTargetAttVal->GetValue());
	
	AttComp->MakeAttribute(FGameplayTagContainer(TAG_DMS_Step_SkillTest_Data_TestResult),UDMSAttributeValue_Numeric::StaticClass(), true);

	BroadcastProgress(InstancedStep,FName(NAME_None),1);
	// FT WINDOW
}

void UDMSSequenceStepDefinition_SkillTest::Progress_ST2(UDMSSequenceStep* InstancedStep)
{
	// Commit cards from hand to skill test.

	// SELECTOR OR JUST BROADCASTING?
	// 일단은 그냥 브로드캐스팅으로.
	BroadcastProgress(InstancedStep,FName(NAME_None),1);

	// FT WINDOW

}

void UDMSSequenceStepDefinition_SkillTest::Progress_ST3(UDMSSequenceStep* InstancedStep)
{
	// Reveal chaos token.
	auto SeqManager		=	UDMSCoreFunctionLibrary::GetDMSSequenceManager(InstancedStep);		check(SeqManager);
	// run sequence of draw chaos token?
	auto DrawSeq = Cast<ADMSGameState>(UDMSCoreFunctionLibrary::GetDMSGameState(InstancedStep))->CreateChaosTokenDrawSeq(InstancedStep->OwnerSequence);
	
	if(DrawSeq==nullptr) {InstancedStep->ProgressEnd(false);return;}

	DrawSeq->AddToOnSequenceFinished_Native([=](bool){
		InstancedStep->ProgressEnd(true);
	});

	SeqManager->RunSequence(DrawSeq);
}

void UDMSSequenceStepDefinition_SkillTest::Progress_ST4(UDMSSequenceStep* InstancedStep)
{
	// Resolve chaos syombol effects.
	BroadcastProgress(InstancedStep,FName(NAME_None));

	// 오토 석세스 / 페일 효과는 여기서 난이도 / 스킬 밸류 0으로 고정해버리고 ST6까지 점프하는 형태로.
}

void UDMSSequenceStepDefinition_SkillTest::Progress_ST5(UDMSSequenceStep* InstancedStep)
{
	// Determine investigator's modified skillvalue
	BroadcastProgress(InstancedStep,FName(NAME_None));
}

void UDMSSequenceStepDefinition_SkillTest::Progress_ST6(UDMSSequenceStep* InstancedStep)
{
	// Determine success/failure of skill test.
	auto SeqAttComp = InstancedStep->OwnerSequence->GetComponentByClass<UDMSAttributeComponent>();

	auto ResultAttribute = SeqAttComp->GetTypedAttributeValue<UDMSAttributeValue_Numeric>(FGameplayTagContainer(TAG_DMS_Step_SkillTest_Data_TestResult),true);

	auto TesterAttribute = SeqAttComp->GetTypedAttributeValue<UDMSAttributeValue_Numeric>(GetSeqAttributeTag_Tester(), true);
	auto TestTargetAttribute = SeqAttComp->GetTypedAttributeValue<UDMSAttributeValue_Numeric>(GetSeqAttributeTag_TestTarget(), true);

	if(!TesterAttribute || !TestTargetAttribute)
		ResultAttribute->SetValue(0);
	else {
		float Result = TesterAttribute->GetValue() - TestTargetAttribute->GetValue();
		ResultAttribute->SetValue(Result);
	}
	BroadcastProgress(InstancedStep,FName(NAME_None));
}

void UDMSSequenceStepDefinition_SkillTest::Progress_ST7(UDMSSequenceStep* InstancedStep)
{
	// Apply skill test results.

	// 시퀀스가 리절트 밸류를 가진 상태로 브로드캐스팅 되므로 성공/실패 값을 알 수 있음.
	BroadcastProgress(InstancedStep,FName(NAME_None));
}

void UDMSSequenceStepDefinition_SkillTest::Progress_ST8(UDMSSequenceStep* InstancedStep)
{
	// skill test ends.
	BroadcastProgress(InstancedStep,FName(NAME_None));
}

FGameplayTag UDMSSequenceStepDefinition_SkillTest::GetPureStepTag_Implementation() const
{
	return FGameplayTag::RequestGameplayTag("Step.Arkham.SkillTest");
}

FGameplayTagContainer UDMSSequenceStepDefinition_SkillTest::GetStepTag_Implementation() const
{
	FGameplayTagContainer rv;
	rv.AppendTags(DefaultSkillTestData.StatName);
	rv.AddTag(GetPureStepTag());
	// Get tags from testing context
	return rv;
}

bool UDMSSequenceStepDefinition_SkillTest::GetProgressOps_Implementation(const FGameplayTag& ProgressTag, TArray<FProgressExecutor>& OutExecutor)
{
	FGameplayTag BaseTag = FGameplayTag::RequestGameplayTag("Step.Arkham.SkillTest");
	if(ProgressTag.MatchesTagExact(BaseTag)) {
		for(int i=1;i<=8;i++){
			FName FunctionName(FString::Printf(TEXT("Progress_ST%d"),i));
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


// LEGACY

//const FGameplayTag UDMSSequenceStep_SkillTest::SkillBonusTag = FGameplayTag::RequestGameplayTag("Attribute.SavedSkillBonus", false);
//
//UDMSSequenceStep_SkillTest::UDMSSequenceStep_SkillTest()
//{
//	//StepTag = TAG_DMS_Step_SkillTest;
//	//SkillTestData.TesterGenerator = CreateDefaultSubobject<UDMSTargetGenerator_SourceObject>("TesterGenerator");
//	//SkillTestData.TestTargetGenerator = CreateDefaultSubobject<UDMSTargetGenerator_SequenceTarget>("TestTargetGenerator");
//}

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
//	UDMSSelectorRequestForm SkillTestForm;
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
//		SkillTestWidget->Updaters.Reserve(OwnerSequence->GetAllActiveEffects().Num());
//		SkillTestWidget->UsedBonusValues.Reserve(OwnerSequence->GetAllActiveEffects().Num());
//		SkillTestWidget->EIIndexMax = OwnerSequence->GetAllActiveEffects().Num();
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

//void UDMSSequenceStep_SkillTest::OnSkillTestCompleted_Implementation()
//{
//	//ProgressComplete(true);
//}
//
//void UDMSSequenceStep_SkillTest::OnSkillTestFailed_Implementation()
//{
//	// Implements branch for failed skill test.
//	//ProgressComplete(false);
//}
//
//bool UDMSSequenceStep_SkillTest::IsTestByEachApplyTarget() const 
//{ 
//	//return SkillTestData.TestByEachApplyTarget && OwnerSequence->IsTargetted(); 
//}

//FGameplayTagContainer UDMSSequenceStep_SkillTest::GetStepTag_Implementation() const 
//{
//	return SkillTestData.IsCommittable ? FGameplayTagContainer(TAG_DMS_Step_SkillTest_Committable) : FGameplayTagContainer(TAG_DMS_Step_SkillTest); 
//}



//float UDMSSequenceStep_SkillTest::CalculateSkillTestResult_Implementation(AActor* Tester, AActor* TestTarget, float BonusValue)
//{
//	float SkillTestResult=0.0f;
//	float SourceValue=0.0f;
//	UDMSGameFunctionLibrary::GetAttributeFromActor(Tester,SkillTestData.StatName);
//
//	// if Target don't have that att => Target Value is 0.0f 
//	if (SkillTestData.bTestToStaticValue) {
//		SkillTestResult = SourceValue + BonusValue - SkillTestData.DefaultTargetValue;
//	}
//	else {
//		float TargetValue = 0.0f;
//		UDMSGameFunctionLibrary::GetAttributeFromActor(TestTarget, SkillTestData.StatName);
//		SkillTestResult = SourceValue + BonusValue - TargetValue;
//	}
//
//	SkillTestResult = SkillTestData.ReverseTestResult ? SkillTestResult*-1.0f : SkillTestResult;
//
//	return SkillTestResult;
//}
//
//void UDMSSequenceStep_SkillTest::SetupTargets(TArray<TObjectPtr<AActor>>& Arr, TObjectPtr<UDMSTargetGenerator>& Generator)
//{
//	for (auto& TObj : Generator->GetTargets(OwnerSequence->GetSourceObject(), OwnerSequence))
//	{
//		AActor* CastedObj = Cast<AActor>(TObj);
//		if (CastedObj==nullptr || !CastedObj->Implements<UDMSEffectorInterface>()
//			|| CastedObj->GetComponentByClass<UDMSAttributeComponent>() == nullptr)	continue;
//
//		Arr.Add(Cast<AActor>(TObj));
//	}
//}
//
//void UDMSSelector_SkillTest::PushResultUpdater(AActor* Tester, AActor* TestTarget,float BonusValue)
//{
//	if (CurrentEIIndex>=EIIndexMax) return;
//
//	float SkillTestResult=OwnerStep->CalculateSkillTestResult(Tester,TestTarget,BonusValue);
//
//	// 셀렉트 종료시 일괄 처리.
//	if (OwnerStep->IsTestByEachApplyTarget()){
//		Updaters.Push([=]() __declspec(noinline) {
//			DMS_LOG_SCREEN(TEXT("Update Data"));
//			//OwnerStep->OwnerSequence->GetAllActiveEffects()[CurrentEIIndex]->DataSet->SetData(TAG_DMS_Step_SkillTest_Data_TestResult, SkillTestResult);
//		});
//	}
//	else{
//		Updaters.Push( [=]() __declspec(noinline) {
//			DMS_LOG_SCREEN(TEXT("Update Data"));
//			//OwnerStep->OwnerSequence->SequenceDatas->SetData(TAG_DMS_Step_SkillTest_Data_TestResult, SkillTestResult);
//		});
//	}
//	UsedBonusValues.Push(BonusValue);
//	CurrentEIIndex++;
//}
//
//void UDMSSelector_SkillTest::PopResultUpdater()
//{
//	if(CurrentEIIndex <= 0) return;
//	Updaters.Pop(false);
//	UsedBonusValues.Pop(false);
//	CurrentEIIndex--;
//}
//
//void UDMSSelector_SkillTest::UpdateSkillTestResult()
//{
//	for(auto& Updater : Updaters)
//		Updater();
//}
//
//float UDMSSelector_SkillTest::GetUsableBonus(AActor* Tester)
//{
//	auto Attribute = Cast<UDMSAttributeValue_Numeric>(Tester->GetComponentByClass<UDMSAttributeComponent>()->GetAttribute(FGameplayTagContainer(UDMSSequenceStep_SkillTest::SkillBonusTag)));
//	float rv= Attribute ? Attribute->GetValue() : 0.0f;
//	for(auto& UsedBonus : UsedBonusValues)
//		rv-=UsedBonus;
//	
//	return rv;
//}