// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DMSCharacterBase.h"
#include "Character/DMSCharacterDefinition.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Location/DMSLocationBase.h"
// Sets default values
ADMSCharacterBase::ADMSCharacterBase(const FObjectInitializer& ObjectInitializer) : ADMSEffectorActorBase(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttributesComponent = CreateDefaultSubobject<UDMSAttributeComponent>(TEXT("AttributesComponent"));

	AttributesComponent->SetIsReplicated(true);
}

UDMSEffectSet* ADMSCharacterBase::GetOwningEffectSet(const FGameplayTag& iSetName)
{
	return CharacterDefinition->CharacterEffectSets.Contains(iSetName) ? CharacterDefinition->CharacterEffectSets[iSetName] : nullptr;
}

void ADMSCharacterBase::OnInitialized_Implementation()
{
	CharacterDefinition = Cast<UDMSCharacterDefinition>(OriginalData);

	Super::OnInitialized_Implementation();

	check(CharacterDefinition != nullptr);

	SetupAttributes(CharacterDefinition->DefaultAttributes);
}

void ADMSCharacterBase::SetupAttributes(const TArray<FDMSAttributeDefinition>& Attributes)
{
	for (auto& Attribute : Attributes){
		AttributesComponent->GenerateAndSetAttribute(Attribute.DefaultTag, Attribute.DefaultValue);
	}
}