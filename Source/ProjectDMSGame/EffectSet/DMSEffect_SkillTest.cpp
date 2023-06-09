// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectSet/DMSEffect_SkillTest.h"
#include "Effect/DMSEffectInstance.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Player/DMSPlayerController.h"
#include "Sequence/DMSSequence.h"
#include "card/DMSCardDefinition.h"
#include "Card/DMSCardManagerComponent.h"
#include "Conditions/DMSConditionObject.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Effect_SkillTest, "Effect.SkillTest");

const FGameplayTag UDMSEffect_SkillTest::SkillBonusName = FGameplayTag::RequestGameplayTag("Attribute.SavedSkillBonus",false);

UDMSEffect_SkillTest::UDMSEffect_SkillTest()
{
	EffectTag = TAG_DMS_Effect_SkillTest;
	OutDataKey = EffectTag;
	bHasPairedSelector = true;
}

void UDMSEffect_SkillTest::Work_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnWorkCompleted& OnWorkCompleted)
{
	AActor* SourceActor;
	float SourceValue;

	if (!SkillTestData.bGetAttributeTargetIsPlayer)	SourceActor = Cast<AActor>(iEI->SourceObject);
	else	SourceActor = iEI->SourcePlayer;

	if ( !UDMSCoreFunctionLibrary::GetAttributeFromActor(SourceActor, SkillTestData.StatName, SourceValue)){ OnWorkCompleted.ExecuteIfBound(SourceSequence, false); return;}

	float SkillBonus =0.0f;

	iEI->DataSet->GetValidDataValue<float>(SkillBonusName,SkillBonus);

	AActor* Target = Cast<AActor>(iEI->GetApplyTarget()->GetObject());

	if (Target == nullptr){OnWorkCompleted.ExecuteIfBound(SourceSequence, false);return;}

	// if Target don't have that att => Target Value is 0.0f 
	float SkillTestResult=0.0f;
	if (SkillTestData.bTestToStaticValue)
	{
		SkillTestResult = SourceValue + SkillBonus - SkillTestData.StaticTargetValue;
	}
	else {
		float TargetValue = 0.0f;
		UDMSCoreFunctionLibrary::GetAttributeFromActor(Target, SkillTestData.StatName, TargetValue);
		SkillTestResult = SourceValue + SkillBonus - TargetValue;
	}

	iEI->DataSet->SetData(OutDataKey, SkillTestResult, true);

	OnWorkCompleted.ExecuteIfBound(SourceSequence, true);
}

void UDMSEffect_SkillTest::InitializePairedSelector(UDMSEffectElementSelectorWidget* WidgetInstance)
{
	Cast<UDMSSelector_SkillTest>(WidgetInstance)->SkillTestData = SkillTestData;
}


//void UDMSSelector_SkillTest::OnPopupSelector_Implementation()
//{
//	auto PC = Cast<ADMSPlayerController>(GetOwningPlayer());
//	if(PC==nullptr) return;
//	//PC->InteractionMode=EDMSCardInteractionMode::PIM_Skill;
//}
//
//
//void UDMSSelector_SkillTest::OnCloseSelector_Implementation()
//{
//	auto PC = Cast<ADMSPlayerController>(GetOwningPlayer());
//	if (PC == nullptr) return;
//	//PC->InteractionMode = EDMSCardInteractionMode::PIM_Play;
//}

UDMSDataObjectSet* UDMSSelector_SkillTest::MakeOutputData_Implementation()
{
	UDMSDataObjectSet* rv = NewObject<UDMSDataObjectSet>(OwnerSeq);

	rv->SetData(UDMSEffect_SkillTest::SkillBonusName, OutBonusValue, true);

	return rv;
}

bool UDMSSelector_SkillTest::SetupWidget_Implementation()
{
	// LEGACY ( Decision broadcasting 하는 Notifymanager 사용으로 로직 변경 )
	//auto CM = OwnerSeq->SourcePlayer->FindComponentByClass<UDMSCardManagerComponent>();
	//if(CM == nullptr) return false;

	//auto Cards = CM->GetAllCards();
	//for (auto Card : Cards)
	//{
	//	DMS_LOG_SIMPLE(TEXT("%s"),*OwnerSeq->OriginalEffectNode->GenerateTagContainer().ToString());
	//	if (!UDMSCoreFunctionLibrary::CheckCardIsCommitable(Card)) continue;
	//	auto& CommitEffectSet = Card->GetCardDefinition()->CardEffectSets[TAG_DMS_EffectType_Commit];

	//	for (auto EffectNode : CommitEffectSet->EffectNodes)
	//	{
	//		if(EffectNode->GetEffectNode()->Conditions->CheckCondition(Card,OwnerSeq))
	//			CommitableCards.AddUnique(Card);
	//		// 체크 컨디션을 할 때 '매뉴얼 체킹' 만 따로 통과시키는걸 만들어야할까?
	//	}

	//}

	return true;

}
