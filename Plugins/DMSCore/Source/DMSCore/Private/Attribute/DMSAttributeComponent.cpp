// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/DMSAttributeComponent.h"
#include "Effect/DMSEffectorInterface.h"
//#include "DMSGame.h"

UDMSAttributeComponent::UDMSAttributeComponent():UDMSSpawnableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bReplicateUsingRegisteredSubObjectList = true;
}


bool UDMSAttributeComponent::ContainAttribute(const FGameplayTagContainer& Tag) const
{
	auto ParentAtt = Cast<UDMSAttributeComponent>(ParentComponent);
	auto Pred = [Tag](UDMSAttribute* Item){ return Item->AttributeTag == Tag;};
	if (Attributes.IndexOfByPredicate(Pred)!=INDEX_NONE) return true;
	return ParentAtt == nullptr ? false : ParentAtt->ContainAttribute(Tag);
}

bool UDMSAttributeComponent::ContainAttributeByQuery(const FGameplayTagQuery& Query) const
{
	auto ParentAtt = Cast<UDMSAttributeComponent>(ParentComponent);
	for (auto& Attribute : Attributes){
		if (Query.Matches(Attribute->AttributeTag)) return true;
	}
	return ParentAtt == nullptr ? false : ParentAtt->ContainAttributeByQuery(Query);
}

bool UDMSAttributeComponent::PredictModifier(const FDMSAttributeModifier& Modifier) const
{
	//DMS_LOG_SCREEN(TEXT("%s : TryModAttribute"), *GetName());
	if (!ContainAttribute(Modifier.ModifierOp->AttributeTag)) return false; // log or what

	return Modifier.ModifierOp->Predict(GetAttribute(Modifier.ModifierOp->AttributeTag));
}


void UDMSAttributeComponent::ApplyModifier(const FDMSAttributeModifier& Modifier) 
{
	//DMS_LOG_SCREEN(TEXT("%s : TryModAttribute"), *GetName());
	if (!PredictModifier(Modifier)) return; // log or what

	GetAttribute(Modifier.ModifierOp->AttributeTag)->ApplyModifier(Modifier);
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

UDMSAttribute* UDMSAttributeComponent::MakeAttribute(const FGameplayTagContainer& AttributeName,const TSubclassOf<UDMSAttributeValue>& AttributeValueClass)
{
	if (ContainAttribute(AttributeName)) return nullptr; // log or what\

	UDMSAttribute* NewAtt = NewObject<UDMSAttribute>(this);
	NewAtt->AttributeTag = AttributeName;
	NewAtt->GenerateValue(AttributeValueClass);
	AddReplicatedSubObject(NewAtt);
	Attributes.Add(NewAtt);

	return NewAtt;
}

UDMSAttribute* UDMSAttributeComponent::GenerateAndSetAttribute(const FGameplayTagContainer& AttributeName, UDMSAttributeValue* AttributeValue)
{
	if (ContainAttribute(AttributeName)) return nullptr; 

	UDMSAttribute* NewAtt = NewObject<UDMSAttribute>(this);
	NewAtt->AttributeTag = AttributeName;
	NewAtt->DuplicateValue(AttributeValue);
	AddReplicatedSubObject(NewAtt);
	Attributes.Add(NewAtt);

	return NewAtt;

}

UDMSAttribute* UDMSAttributeComponent::DuplicateAttribute(UDMSAttribute* Attribute)
{
	if (!Attribute || ContainAttribute(Attribute->AttributeTag)) return nullptr; // log or what\

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
	auto Pred = [AttributeTag](UDMSAttribute* Item){ return Item->AttributeTag == AttributeTag;};
	UDMSAttribute* ThisFound = Attributes[Attributes.IndexOfByPredicate(Pred)];
	if ( ThisFound == nullptr ) return ParentAtt==nullptr ? ParentAtt->GetAttribute(AttributeTag) : nullptr;
	return ThisFound;
}

TArray<UDMSAttribute*> UDMSAttributeComponent::GetAttributesByQuery(const FGameplayTagQuery& TargetQuery) const
{
	TArray<UDMSAttribute*> ThisFound;

	if (!ContainAttributeByQuery(TargetQuery)) return ThisFound; // log or what\

	auto ParentAtt = Cast<UDMSAttributeComponent>(ParentComponent);
	
	for (auto& Attribute : Attributes){
		if (TargetQuery.Matches(Attribute->AttributeTag))
			ThisFound.Add(Attribute);
	}
	if ( ThisFound.Num() == 0 ) return ParentAtt==nullptr ? ParentAtt->GetAttributesByQuery(TargetQuery) : ThisFound;
	return ThisFound;
}