// Copyright JeongWoo Lee


#include "Attribute/DMSAttributeComponent.h"
#include "Effect/DMSEffectorInterface.h"
//#include "DMSGame.h"

UDMSAttributeComponent::UDMSAttributeComponent():UDMSSpawnableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bReplicateUsingRegisteredSubObjectList = true;
}


bool UDMSAttributeComponent::ContainAttribute(const FGameplayTagContainer& Tag, bool Exact) const
{
	auto ParentAtt = Cast<UDMSAttributeComponent>(ParentComponent);
	auto Pred = [Tag](UDMSAttribute* Item){ return Item->AttributeTag == Tag;};
	if (Attributes.IndexOfByPredicate(Pred)!=INDEX_NONE) return true;
	return ParentAtt == nullptr || Exact ? false : ParentAtt->ContainAttribute(Tag);
}

bool UDMSAttributeComponent::ContainAttributeByQuery(const FGameplayTagQuery& Query, bool Exact) const
{
	auto ParentAtt = Cast<UDMSAttributeComponent>(ParentComponent);
	for (auto& Attribute : Attributes){
		if (Query.Matches(Attribute->AttributeTag)) return true;
	}
	return ParentAtt == nullptr || Exact ? false : ParentAtt->ContainAttributeByQuery(Query);
}

UDMSAttribute* UDMSAttributeComponent::GetAttribute(const FGameplayTagContainer& AttributeTag, bool Exact) const
{
	//if (!ContainAttribute(AttributeTag)) return nullptr; // log or what\

	auto ParentAtt = Cast<UDMSAttributeComponent>(ParentComponent);
	auto Pred = [AttributeTag](UDMSAttribute* Item){ return Item->AttributeTag == AttributeTag;};
	int ItemIdx = Attributes.IndexOfByPredicate(Pred);
	UDMSAttribute* ThisFound = ItemIdx != INDEX_NONE ? Attributes[ItemIdx] : nullptr;
	if ( ThisFound == nullptr ) return ParentAtt && !Exact ? ParentAtt->GetAttribute(AttributeTag) : nullptr;
	return ThisFound;
}

TArray<UDMSAttribute*> UDMSAttributeComponent::GetAttributesByQuery(const FGameplayTagQuery& TargetQuery, bool Exact) const
{
	TArray<UDMSAttribute*> ThisFound;

	if (!ContainAttributeByQuery(TargetQuery, Exact)) return ThisFound; // log or what

	auto ParentAtt = Cast<UDMSAttributeComponent>(ParentComponent);

	for (auto& Attribute : Attributes){
		if (TargetQuery.Matches(Attribute->AttributeTag))
			ThisFound.Add(Attribute);
	}
	if ( ThisFound.Num() == 0 ) return ParentAtt && !Exact ? ParentAtt->GetAttributesByQuery(TargetQuery) : ThisFound;
	return ThisFound;
}


UDMSAttributeValue* UDMSAttributeComponent::GetAttributeValue(const FGameplayTagContainer& AttributeName, bool Exact) const
{
	auto Att = GetAttribute(AttributeName,Exact);
	return Att ? Att->AttributeValue : nullptr;
}

UDMSAttribute* UDMSAttributeComponent::MakeAttribute(const FGameplayTagContainer& AttributeName,const TSubclassOf<UDMSAttributeValue>& AttributeValueClass, bool Exact)
{
	UDMSAttribute* Rv = GetAttribute(AttributeName,Exact);
	if (Rv && Rv->AttributeValue.IsA(AttributeValueClass)) return Rv;

	Rv = NewObject<UDMSAttribute>(this);
	Rv->AttributeTag = AttributeName;
	Rv->GenerateValue(AttributeValueClass);
	RegisterAttribute_Internal(Rv);

	return Rv;
}

UDMSAttribute* UDMSAttributeComponent::GenerateAndSetAttribute(const FGameplayTagContainer& AttributeName, UDMSAttributeValue* AttributeValue, bool Exact)
{
	UDMSAttribute* NewAtt = GetAttribute(AttributeName,Exact);
	if (NewAtt == nullptr) {
		NewAtt = NewObject<UDMSAttribute>(this);
		NewAtt->AttributeTag = AttributeName;
		NewAtt->CopyValueFromOther(AttributeValue);

		RegisterAttribute_Internal(NewAtt);
	}
	else{
		NewAtt->CopyValueFromOther(AttributeValue);
	}

	return NewAtt;
}

void UDMSAttributeComponent::RemoveAttribute(const FGameplayTagContainer& AttributeName, bool Exact)
{
	UDMSAttribute* Att = GetAttribute(AttributeName, Exact);
	if (Att == nullptr) return;
	Attributes.Remove(Att);
	Att->MarkAsGarbage();
}

UDMSAttribute* UDMSAttributeComponent::DuplicateAttribute(UDMSAttribute* Attribute, bool Exact)
{
	if (!Attribute || ContainAttribute(Attribute->AttributeTag,Exact)) return nullptr; // log or what\

	UDMSAttribute* NewAtt = DuplicateObject(Attribute,this);
	NewAtt->AttributeTag = Attribute->AttributeTag;
	RegisterAttribute_Internal(NewAtt);
	return NewAtt;
}

void UDMSAttributeComponent::RegisterAttribute_Internal(UDMSAttribute* NewAttribute)
{
	AddReplicatedSubObject(NewAttribute);
	Attributes.Add(NewAttribute);

	OnAttributeAdded.Broadcast(NewAttribute);
}

bool UDMSAttributeComponent::PredictModifier(const FGameplayTagContainer& AttributeTag, const FDMSAttributeModifier& Modifier) const
{
	//DMS_LOG_SCREEN(TEXT("%s : TryModAttribute"), *GetName());
	if (!ContainAttribute(AttributeTag)) return false; // log or what

	return Modifier.ModifierOp->Predict(GetAttribute(AttributeTag),Modifier.Value);
}

void UDMSAttributeComponent::ApplyModifier(const FGameplayTagContainer& AttributeTag, const FDMSAttributeModifier& Modifier) 
{
	//DMS_LOG_SCREEN(TEXT("%s : TryModAttribute"), *GetName());
	if (!PredictModifier(AttributeTag, Modifier)) return; // log or what

	GetAttribute(AttributeTag)->ApplyModifier(Modifier);
}

void UDMSAttributeComponent::BindOnModifiedToAttribute(const FGameplayTagContainer& AttributeTag, const FOnAttributeModifiedSignature& iDelegate)
{
	if (!ContainAttribute(AttributeTag)) return;

	GetAttribute(AttributeTag)->BindOnModified(iDelegate);
}

void UDMSAttributeComponent::BindOnAttributeAdded(const FOnAttributeModifiedSignature& iDelegate)
{
	OnAttributeAdded.Add(iDelegate);
}

void UDMSAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDMSAttributeComponent, Attributes);
}
