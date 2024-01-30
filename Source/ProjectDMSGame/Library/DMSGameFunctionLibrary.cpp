// Fill out your copyright notice in the Description page of Project Settings.


#include "DMSGameFunctionLibrary.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Card/DMSCardBase.h"
#include "Card/DMSCardDefinition.h"
#include "Card/DMSCardData.h"
#include "Effect/DMSEffectInstance.h"



UDMSAttribute* UDMSGameFunctionLibrary::GetAttributeFromActor(AActor* iActor, const FGameplayTagContainer& AttributeTag)
{
	if (iActor == nullptr) 	return nullptr;

	//UActorComponent* Comp;
	//
	//Comp = iActor->GetComponentByClass(UDMSAttributeComponent::StaticClass());

	UDMSAttributeComponent* AttComp = iActor->GetComponentByClass<UDMSAttributeComponent>();
	if (AttComp == nullptr) 	return nullptr;

	return AttComp->GetAttribute(AttributeTag);
}

//bool UDMSGameFunctionLibrary::CheckCardIsCommitable(ADMSCardBase* iCard)
//{
//	//bool rv = iCard->GetCardDefinition()->CardEffectSets.Contains(TAG_DMS_EffectType_Commit);
//	//return rv;
//	return false;
//}


void UDMSGameFunctionLibrary::SetCardDataOwner(const FDMSCardData& Data, UObject* Owner)
{
	for (auto EI : Data.AttachedEffect)
	{
		if (EI != nullptr)
			EI->Rename(nullptr, Owner);
	}
}

