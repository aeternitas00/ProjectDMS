// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DMSCharacterBase.h"
#include "Character/DMSCharacterDefinition.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Location/DMSLocationBase.h"
// Sets default values
ADMSCharacterBase::ADMSCharacterBase() : ADMSEffectorActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttributeComponent = CreateDefaultSubobject<UDMSAttributeComponent>(TEXT("AttributeComponent"));
}

UDMSEffectSet* ADMSCharacterBase::GetOwningEffectSet(const FName& iSetName)
{
	return CharacterDefinition->CharacterEffectSets.Contains(iSetName) ? CharacterDefinition->CharacterEffectSets[iSetName] : nullptr;
}

void ADMSCharacterBase::InitializeCharacter_Implementation(const UDMSCharacterDefinition* NewDefinition)
{
	CharacterDefinition = NewDefinition;
	SetupAttributes(CharacterDefinition->DefaultAttributes);
}

void ADMSCharacterBase::SetupAttributes(const TArray<FDMSSerializedAttribute>& Attributes)
{
	for (auto& Attribute : Attributes)
		AttributeComponent->MakeAttribute(Attribute.AttributeTag, Attribute.Value);
}