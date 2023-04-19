// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DMSCharacterBase.h"
#include "Character/DMSCharacterDefinition.h"
#include "Attribute/DMSAttributeComponent.h"

// Sets default values
ADMSCharacterBase::ADMSCharacterBase() : ADMSEffectorActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttributeComponent = CreateDefaultSubobject<UDMSAttributeComponent>(TEXT("AttributeComponent"));
}

// Called when the game starts or when spawned
//void ADMSCharacterBase::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}

// Called every frame
//void ADMSCharacterBase::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void ADMSCharacterBase::InitializeCharacter_Implementation(UDMSCharacterDefinition* NewDefinition)
{
	CharacterDefinition = NewDefinition;
	SetupAttributes(CharacterDefinition->DefaultAttributes);
}

void ADMSCharacterBase::SetupAttributes(const TArray<FDMSSerializedAttribute>& Attributes)
{
	for (auto& Attribute : Attributes)
		AttributeComponent->MakeAttribute(Attribute.AttributeTag, Attribute.Value);
}