// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/DMSAttributeComponent.h"
//#include "DMSGame.h"
// Sets default values for this component's properties
UDMSAttributeComponent::UDMSAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDMSAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDMSAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UDMSAttributeComponent::TryModAttribute(const FDMSAttributeModifier& Modifier)
{
	DMS_LOG_SCREEN(TEXT("%s : TryModAttribute"), *GetName());
	if (!_Attributes.Contains(Modifier.AttributeName)) return false; // log or what

	return _Attributes[Modifier.AttributeName]->ModAttribute(Modifier);
}

bool UDMSAttributeComponent::GetAttributeValue(const FName& AttributeName, float& outValue) const
{
	bool rv = GetAttribute(AttributeName)==nullptr;
	outValue = rv ? GetAttribute(AttributeName)->GetValue() : -1.0f;
	return rv;
}

void UDMSAttributeComponent::MakeAttribute(const FName& AttributeName, const float& DefValue)
{
	if (_Attributes.Contains(AttributeName)) return; // log or what\

	UDMSAttribute* NewAtt = NewObject<UDMSAttribute>(this);
	NewAtt->Value = DefValue;

	_Attributes.Add(AttributeName, NewAtt);
}

void UDMSAttributeComponent::BindOnModifiedToAttribute(const FName& AttributeName, const FOnAttributeModifiedSignature& iDelegate)
{
	if (!_Attributes.Contains(AttributeName)) return;

	_Attributes[AttributeName]->BindOnModified(iDelegate);
}

UDMSAttribute* UDMSAttributeComponent::GetAttribute(const FName& AttributeName) const
{
	if (!_Attributes.Contains(AttributeName)) return nullptr; // log or what\

	return _Attributes[AttributeName];
}