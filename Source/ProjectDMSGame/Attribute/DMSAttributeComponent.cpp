// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/DMSAttributeComponent.h"
#include "Effect/DMSEffectorInterface.h"
//#include "DMSGame.h"

UDMSAttributeComponent::UDMSAttributeComponent()
{

}


bool UDMSAttributeComponent::ContainAttribute(const FGameplayTag& Tag) const
{
	auto ParentAtt = Cast<UDMSAttributeComponent>(ParentComponent);
	if (Attributes.Contains(Tag)) return true;
	return ParentAtt == nullptr ? false : ParentAtt->ContainAttribute(Tag);
}

bool UDMSAttributeComponent::TryModAttribute(const FDMSAttributeModifier& Modifier,float& OutValue,bool Apply)
{
	//DMS_LOG_SCREEN(TEXT("%s : TryModAttribute"), *GetName());
	if (!ContainAttribute(Modifier.AttributeTag)) return false; // log or what

	return GetAttribute(Modifier.AttributeTag)->TryModAttribute(Modifier, OutValue, Apply);
}

bool UDMSAttributeComponent::GetAttributeValue(const FGameplayTag& AttributeName, float& outValue) const
{
	if (!ContainAttribute(AttributeName)) return false;

	outValue = GetAttribute(AttributeName)->Value;

	return true;
}

TArray<FDMSSerializedAttribute> UDMSAttributeComponent::ToSerialized()
{
	TArray<FDMSSerializedAttribute> rv;
	for (auto& Att : Attributes)
		rv.Add(FDMSSerializedAttribute(Att.Key, Att.Value->GetValue()));
	return rv;
}

void UDMSAttributeComponent::MakeAttribute(const FGameplayTag& AttributeName, const float& DefValue)
{
	if (ContainAttribute(AttributeName)) return; // log or what\

	UDMSAttribute* NewAtt = NewObject<UDMSAttribute>(this);
	NewAtt->Value = DefValue;
	NewAtt->AttributeTag = AttributeName;
	Attributes.Add(AttributeName, NewAtt);
}

void UDMSAttributeComponent::BindOnModifiedToAttribute(const FGameplayTag& AttributeName, const FOnAttributeModifiedSignature& iDelegate)
{
	if (!ContainAttribute(AttributeName)) return;

	GetAttribute(AttributeName)->BindOnModified(iDelegate);
}

UDMSAttribute* UDMSAttributeComponent::GetAttribute(const FGameplayTag& AttributeName) const
{
	if (!ContainAttribute(AttributeName)) return nullptr; // log or what\

	auto ParentAtt = Cast<UDMSAttributeComponent>(ParentComponent);
	return Attributes[AttributeName] == nullptr ? ParentAtt->GetAttribute(AttributeName) : Attributes[AttributeName];
}