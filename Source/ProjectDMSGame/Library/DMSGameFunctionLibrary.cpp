// Fill out your copyright notice in the Description page of Project Settings.


#include "DMSGameFunctionLibrary.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Card/DMSCardBase.h"
#include "Card/DMSCardDefinition.h"
#include "Card/DMSCardData.h"
#include "Effect/DMSEffectInstance.h"



bool UDMSGameFunctionLibrary::GetAttributeFromActor(AActor* iActor, const FGameplayTag& Name, float& outValue)
{
	if (iActor == nullptr) goto INVSOURCE;

	//UActorComponent* Comp;
	//
	//Comp = iActor->GetComponentByClass(UDMSAttributeComponent::StaticClass());

	UDMSAttributeComponent* AttComp = iActor->GetComponentByClass<UDMSAttributeComponent>();
	if (AttComp == nullptr) goto INVSOURCE;

	float SourceValue;
	if (!AttComp->GetAttributeValue(Name, SourceValue)) {
	INVSOURCE:
		outValue = 0.0f; return false;
	}

	outValue = SourceValue;
	return true;
}

bool UDMSGameFunctionLibrary::CheckCardIsCommitable(ADMSCardBase* iCard)
{
	bool rv = iCard->GetCardDefinition()->CardEffectSets.Contains(TAG_DMS_EffectType_Commit);
	return rv;
}


void UDMSGameFunctionLibrary::SetCardDataOwner(const FDMSCardData& Data, UObject* Owner)
{
	for (auto EI : Data.AttachedEffect)
	{
		if (EI != nullptr)
			EI->Rename(nullptr, Owner);
	}
}

