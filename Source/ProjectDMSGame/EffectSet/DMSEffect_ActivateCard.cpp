// Fill out your copyright notice in the Description page of Project Settings.

#include "EffectSet/DMSEffect_ActivateCard.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEIManagerComponent.h"
#include "GameModes/DMSGameMode.h"
#include "Card/DMSCardBase.h"
#include "Card/DMSCardDefinition.h"
#include "Library/DMSCoreFunctionLibrary.h"

UDMSEffect_ActivateCard::UDMSEffect_ActivateCard() 
{ 
	Keyword = TEXT("ActivateCard"); 

	//auto Selector = CreateDefaultSubobject<UDMSEffectElementSelectorWidget>("EffectSelector");

	//Selectors.Add(Selector);
}

void UDMSEffect_ActivateCard::Work_Implementation(UDMSEffectInstance* iEI)
{
	//REQUIREMENT CHECK
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("ActivateCard"));
	DMS_LOG_SCREEN(TEXT("%s : ActivateCard"), *iEI->GetName());
	UDMSSeqManager* SeqMan = UDMSCoreFunctionLibrary::GetDMSSequenceManager();

	if (!SeqMan) return;

	UDMSEIManagerComponent* CardEffector = Cast<UDMSEIManagerComponent>(iEI->GetOuter());
	ADMSCardBase* TargetCard = Cast<ADMSCardBase>(CardEffector->GetOwner());

	if (!TargetCard) return;

	//SelectedIndex from iEI->Data

	// Make Sequence for all Effects[{SelectedIndexs}];
	// 지금은 전체순회만 구현

	for (auto NodeWrapper : TargetCard->GetCardDefinition()->CardEffects->EffectNodes)
	{
		auto Node = NodeWrapper->GetEffectNode();
		SeqMan->RequestCreateSequence(iEI->SourceObject, iEI->SourceController, Node,{});
	}
}