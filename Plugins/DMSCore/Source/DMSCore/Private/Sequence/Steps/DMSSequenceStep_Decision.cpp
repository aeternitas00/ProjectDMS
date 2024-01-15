// Fill out your copyright notice in the Description page of Project Settings.


#include "Sequence/Steps/DMSSequenceStep_Decision.h"
#include "Sequence/DMSSeqManager.h"
#include "Common/DMSTargetGenerator.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Effect/DMSEffectDefinition.h"
#include "Effect/DMSEffectorOwnableInterface.h"
#include "Effect/DMSEffectHandler.h"
#include "Effect/DMSEffectInstance.h"
#include "Player/DMSPlayerControllerBase.h"
#include "GameFramework/PlayerController.h"
#include "GameModes/DMSGameStateBase.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Step_Decision, "Step.Decision");

UDMSSequenceStep_Decision::UDMSSequenceStep_Decision()
{
	//StepTag = TAG_DMS_Step_Decision;
	DecisionMaker = CreateDefaultSubobject<UDMSTargetGenerator_SourcePlayer>("DecisionMaker");
}



FGameplayTag UDMSSequenceStep_Decision::GetStepTag_Implementation() const{return TAG_DMS_Step_Decision;}

void UDMSSequenceStep_Decision::OnBefore_Implementation()
{
	// Behavior
	auto SM = UDMSCoreFunctionLibrary::GetDMSSequenceManager(); check(SM);

	//DMS_LOG_SCREEN(TEXT("==-- DecisionStep_BEFORE [ Depth : %d ] --=="), SM->GetDepth(OwnerSequence));
	ProgressComplete();
}

void UDMSSequenceStep_Decision::OnDuring_Implementation()
{
	// Behavior
	auto GS = UDMSCoreFunctionLibrary::GetDMSGameState(); check(GS);
	auto SM = UDMSCoreFunctionLibrary::GetDMSSequenceManager(); check(SM);
	auto EH = UDMSCoreFunctionLibrary::GetDMSEffectHandler(); check(EH);
	auto SelM = UDMSCoreFunctionLibrary::GetDMSSelectorManager(); check(SelM);

	//DMS_LOG_SCREEN(TEXT("==-- DecisionStep_DURING [ Depth : %d ] --=="), SM->GetDepth(OwnerSequence));
	
	ADMSPlayerControllerBase* WidgetOwner = nullptr;

	auto DecisionMakers = DecisionMaker->GetTargets(OwnerSequence->GetSourceObject(), OwnerSequence);
	
	// 디시전을 여러명이서 순차 진행하는 케이스가 있을까? 


	TArray<ADMSPlayerControllerBase*> CastedDecisionMakers;

	for (auto DM : DecisionMakers)
	{
		if (!DM->Implements<UDMSEffectorInterface>()) continue;
		CastedDecisionMakers.AddUnique(Cast<IDMSEffectorInterface>(DM)->GetOwningPlayerController());
	}

	//if (CastedDecisionMakers.Num()>1){

	//	FDMSSelectorRequestForm NewForm;	
	//	NewForm.SelectorClass= SM->DecisionMakerSelector;
	//	NewForm.SelectAmount=1;
	//	NewForm.OnCompleted.BindUObject();
	//	for (auto& PC : CastedDecisionMakers)
	//	{
	//		auto Data = NewObject<UDMSDataObject>(this);
	//		Data->Set(PC);
	//		NewForm.Candidates.Add(Data);
	//	}
	//	auto SelHandle = SelM->RequestCreateSelector(NewForm);
	//	SelHandle->SetupSelector(GS->GetLeaderPlayerController());
	//	SelHandle->RunSelector();
	//	
	//}
	//else if (CastedDecisionMakers.Num()==1)
	//	WidgetOwner = CastedDecisionMakers[0];
	
	WidgetOwner = CastedDecisionMakers.Num() == 0 ? nullptr : CastedDecisionMakers[0];
	
	TArray<FDMSSelectorRequestForm> DecisionForms;

	for ( auto& DD : DecisionDefinitions )
		DecisionForms.Append(DD.SetupRequestForm(OwnerSequence));

	if ( WidgetOwner == nullptr || !WidgetOwner->SetupWidgetQueue(OwnerSequence, SelM->RequestCreateSelectors(DecisionForms))) {
		DMS_LOG_SIMPLE(TEXT("==== %s : EI Data Selection Canceled  ===="), *OwnerSequence->GetName());
		
		ProgressComplete(false); return;
	}

	RunWidgetQueue(WidgetOwner, [=](UDMSSequence* pSequence) {

		// Run sequence ( Notifying steps and apply )
		DMS_LOG_SIMPLE(TEXT("==== %s : EI Data Selection Completed  ===="), *pSequence->GetName());

		// Prevent self notifing 
		for (auto EI : pSequence->GetAllEIs())
		{
			EI->ChangeEIState(EDMSEIState::EIS_Default);
		}

		ProgressComplete();

	});
}


void UDMSSequenceStep_Decision::OnAfter_Implementation()
{
	// Behavior
	auto SM = UDMSCoreFunctionLibrary::GetDMSSequenceManager(); check(SM);
	//DMS_LOG_SCREEN(TEXT("==-- DecisionStep_AFTER [ Depth : %d ] --=="), SM->GetDepth(OwnerSequence));
	ProgressComplete();
}
