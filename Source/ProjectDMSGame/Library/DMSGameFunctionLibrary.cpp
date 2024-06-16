// Copyright JeongWoo Lee


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

void UDMSGameFunctionLibrary::SetCardDataOwner(const FDMSCardData& Data, UObject* Owner)
{
	for (auto EI : Data.AttachedEffect)
	{
		if (EI != nullptr)
			EI->Rename(nullptr, Owner);
	}
}

