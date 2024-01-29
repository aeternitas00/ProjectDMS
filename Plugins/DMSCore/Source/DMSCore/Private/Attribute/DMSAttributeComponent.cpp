// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/DMSAttributeComponent.h"
#include "Effect/DMSEffectorInterface.h"
//#include "DMSGame.h"

UDMSAttributeComponent::UDMSAttributeComponent():UDMSSpawnableComponent()
{
	bReplicateUsingRegisteredSubObjectList = true;
}


bool UDMSAttributeComponent::ContainAttribute(const FGameplayTagContainer& Tag) const
{
	auto ParentAtt = Cast<UDMSAttributeComponent>(ParentComponent);
	auto Pred = [Tag](const UDMSAttribute*& Item){ return Item->AttributeTag == Tag;};
	if (Attributes.IndexOfByPredicate(Pred)!=INDEX_NONE) return true;
	return ParentAtt == nullptr ? false : ParentAtt->ContainAttribute(Tag);
}

bool UDMSAttributeComponent::PredictModifier(UDMSAttributeModifier* Modifier) const
{
	//DMS_LOG_SCREEN(TEXT("%s : TryModAttribute"), *GetName());
	if (!ContainAttribute(Modifier->AttributeValue->AttributeTag)) return false; // log or what

	return Modifier->Predict(GetAttribute(Modifier->AttributeValue->AttributeTag));
}


void UDMSAttributeComponent::ApplyModifier(UDMSAttributeModifier* Modifier) 
{
	//DMS_LOG_SCREEN(TEXT("%s : TryModAttribute"), *GetName());
	if (!PredictModifier(Modifier)) return; // log or what

	GetAttribute(Modifier->AttributeValue->AttributeTag)->ApplyModifier(Modifier);
}

//bool UDMSAttributeComponent::GetAttributeValue(const FGameplayTag& AttributeName, float& outValue) const
//{
//	if (!ContainAttribute(AttributeName)) return false;
//
//	outValue = GetAttribute(AttributeName)->Value;
//
//	return true;
//}

//TArray<FDMSSerializedAttribute> UDMSAttributeComponent::ToSerialized()
//{
//	TArray<FDMSSerializedAttribute> rv;
//	for (auto& Att : Attributes)
//		rv.Add(FDMSSerializedAttribute(Att->AttributeTag, Att->lue->GetValue()));
//	return rv;
//}

void UDMSAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDMSAttributeComponent, Attributes);
}

UDMSAttribute* UDMSAttributeComponent::MakeAttribute(const FGameplayTagContainer& AttributeName,const TSubclassOf<UDMSAttribute>& AttributeClass)
{
	if (ContainAttribute(AttributeName)) return; // log or what\

	UDMSAttribute* NewAtt = NewObject<UDMSAttribute>(this,AttributeClass);
	NewAtt->AttributeTag = AttributeName;
	AddReplicatedSubObject(NewAtt);
	Attributes.Add(NewAtt);

	return NewAtt;
}

UDMSAttribute* UDMSAttributeComponent::DuplicateAttribute(UDMSAttribute* Attribute)
{
	if (ContainAttribute(Attribute->AttributeTag)) return; // log or what\

	UDMSAttribute* NewAtt = DuplicateObject(Attribute,this);
	NewAtt->AttributeTag = Attribute->AttributeTag;
	AddReplicatedSubObject(NewAtt);
	Attributes.Add(NewAtt);

	return NewAtt;
}


void UDMSAttributeComponent::BindOnModifiedToAttribute(const FGameplayTagContainer& AttributeTag, const FOnAttributeModifiedSignature& iDelegate)
{
	if (!ContainAttribute(AttributeTag)) return;

	GetAttribute(AttributeTag)->BindOnModified(iDelegate);
}

UDMSAttribute* UDMSAttributeComponent::GetAttribute(const FGameplayTagContainer& AttributeTag) const
{
	if (!ContainAttribute(AttributeTag)) return nullptr; // log or what\

	auto ParentAtt = Cast<UDMSAttributeComponent>(ParentComponent);
	auto Pred = [AttributeTag](const UDMSAttribute*& Item){ return Item->AttributeTag == AttributeTag;};
	UDMSAttribute* ThisFound = Attributes[Attributes.IndexOfByPredicate(Pred)];
	if ( ThisFound == nullptr ) return ParentAtt==nullptr ? ParentAtt->GetAttribute(AttributeTag) : nullptr;
	return ThisFound;
}