// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectSet/DMSEffect_SkillTest.h"
#include "Effect/DMSEffectInstance.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Player/DMSPlayerController.h"

const FName UDMSEffect_SkillTest::SkillBonusName = TEXT("SkillBonus");

UDMSEffect_SkillTest::UDMSEffect_SkillTest()
{
	Keyword = TEXT("SkillTest");
	bHasPairedSelector = true;
}

void UDMSEffect_SkillTest::Work_Implementation(UDMSEffectInstance* iEI)
{
	AActor* SourceActor;
	float SourceValue;

	if (SkillTestData.bSourceFlag)	SourceActor = Cast<AActor>(iEI->SourceObject);
	else	SourceActor = iEI->SourceController;

	if ( !UDMSCoreFunctionLibrary::GetAttributeFromActor(SourceActor, SkillTestData.StatName, SourceValue)) return;

	float SkillBonus =0.0f;

	auto Data = iEI->DataSet->GetData(SkillBonusName);
	if (Data != nullptr && Data->TypeCheck<float>()) SkillBonus = Data->Get<float>();

	AActor* Target = iEI->GetTypedOuter<AActor>();
	float TargetValue = 0.0f;

	// if Target don't have that att => Target Value is 0.0f 
	if (Target == nullptr)return;
	UDMSCoreFunctionLibrary::GetAttributeFromActor(Target, SkillTestData.StatName, TargetValue);

	float SkillTestResult = SkillTestData.bTestToStaticValue ? SourceValue + SkillBonus - TargetValue : SourceValue + SkillBonus - SkillTestData.StaticTargetValue;

	iEI->DataSet->SetData(TEXT("SkillTestResult"), SkillTestResult,true );
}

void UDMSEffect_SkillTest::InitializePairedSelector(UDMSEffectElementSelectorWidget* WidgetInstance)
{
	Cast<UDMSSelector_SkillTest>(WidgetInstance)->SkillTestData = SkillTestData;
}

bool UDMSSelector_SkillTest::SetupWidget_Implementation()
{ 
	return true; 
}

void UDMSSelector_SkillTest::OnPopupSelector_Implementation()
{
	auto PC = Cast<ADMSPlayerController>(GetOwningPlayer());
	if(PC==nullptr) return;
	//PC->InteractionMode=EDMSCardInteractionMode::PIM_Skill;
}


void UDMSSelector_SkillTest::OnCloseSelector_Implementation()
{
	auto PC = Cast<ADMSPlayerController>(GetOwningPlayer());
	if (PC == nullptr) return;
	//PC->InteractionMode = EDMSCardInteractionMode::PIM_Play;
}