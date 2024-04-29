// Fill out your copyright notice in the Description page of Project Settings.


#include "Misc/DMSChaosTokenBase.h"
#include "Misc/DMSChaosTokenDefinition.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Effect/DMSEIManagerComponent.h"

ADMSChaosTokenBase::ADMSChaosTokenBase(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	AttributeComponent = CreateDefaultSubobject<UDMSAttributeComponent>(TEXT("AttributesComponent"));
	AttributeComponent->SetIsReplicated(true);

	bReplicates=true;
}

UDMSEffectSet* ADMSChaosTokenBase::GetOwningEffectSet(const FGameplayTag& iSetName)
{
	auto TokenData = Cast<UDMSChaosTokenDefinition>(OriginalData);

	return TokenData ? TokenData->EffectSet : nullptr;
}

void ADMSChaosTokenBase::OnInitialized_Implementation()
{
	Super::OnInitialized_Implementation();

	EIManagerComponent->SetupOwnEffect(GetOwningEffectSet(FGameplayTag::EmptyTag), FGameplayTag::EmptyTag);

	auto TokenData = Cast<UDMSChaosTokenDefinition>(OriginalData);
	
	if(!TokenData) return;

	for(auto& Attribute : TokenData->DefaultAttributes)
		AttributeComponent->GenerateAndSetAttribute(Attribute.DefaultTag, Attribute.DefaultValue);	
}
