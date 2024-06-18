// Copyright JeongWoo Lee


#include "Attribute/DMSAttribute.h"
#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEffectorInterface.h"

UDMSAttribute::UDMSAttribute():AttributeValue(nullptr)
{
}

void UDMSAttribute::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDMSAttribute,AttributeValue);
}

void UDMSAttribute::BindOnModified(const FOnAttributeModifiedSignature& iDelegate)
{
	//if (iObject==nullptr) return;
	//OnAttributeModified.AddDynamic();
	OnAttributeModified.Add(iDelegate);
}

void UDMSAttribute::ApplyModifier(const FDMSAttributeModifier& Modifier)
{
	//if(!Modifier.ModifierOp->Predict(this,Modifier.Value)) return;

	AttributeValue->ExecuteModifier(Modifier);
	// Make this multicast
	OnAttributeModified.Broadcast(this);
}

void UDMSAttribute::GetDeltaAfterModify(const FDMSAttributeModifier& Modifier,TObjectPtr<UDMSAttributeValue>& OutValue)
{
	//AttributeValue->GetDeltaAfterModify(Modifier,OutValue);
}

void UDMSAttribute::GetDeltasAfterModify(const FDMSAttributeModifier& OriginalModifier, ADMSActiveEffect* OriginalActiveEffect, TArray<FDMSAttributeModifier>& OutModifiers)
{
	AttributeValue->GetDeltasAfterModify(OriginalModifier,OriginalActiveEffect,OutModifiers);
}

void UDMSAttribute::GenerateValue(const TSubclassOf<UDMSAttributeValue>& ValueClass)
{
	if(AttributeValue!=nullptr) {AttributeValue->MarkAsGarbage();}
	
	AttributeValue = NewObject<UDMSAttributeValue>(this,ValueClass);
}

void UDMSAttribute::CopyValueFromOther(UDMSAttributeValue* Value)
{	
	if(AttributeValue!=nullptr) {AttributeValue->MarkAsGarbage();}
	
	AttributeValue = DuplicateObject<UDMSAttributeValue>(Value, this);
}

//void UDMSAttributeValue::ExecuteModifierDefinition(UDMSAttributeModifierDefinition* Modifier)
//{
//	Modifier->ApplyModifierDefinition(this);	
//}

void UDMSAttributeValue::ExecuteModifier_Implementation(const FDMSAttributeModifier& Modifier) { Modifier.ModifierOp->ExecuteOp(this, Modifier.Value); }

//void UDMSAttributeValue::GetDeltaAfterModify(const FDMSAttributeModifier& Modifier,TObjectPtr<UDMSAttributeValue>& OutValue)
//{
//	//OutValue->Value = Modifier.Value->Value;
//}

void UDMSAttributeValue::GetDeltasAfterModify(const FDMSAttributeModifier& OriginalModifier, ADMSActiveEffect* OriginalActiveEffect, TArray<FDMSAttributeModifier>& OutModifiers)
{
	//OutModifiers.Add(NewModifier);
}

FArchive& operator<<(FArchive& Ar, FDMSAttributeDefinition& AttDefinition)
{
	TArray<FGameplayTag> TagArr;
	if (Ar.IsSaving()){
		TagArr = AttDefinition.DefaultTag.GetGameplayTagArray();
		Ar << TagArr;
	}
	else {
		Ar << TagArr;
		AttDefinition.DefaultTag = FGameplayTagContainer::CreateFromArray(TagArr);
	}
	Ar << AttDefinition.DefaultValue;
	return Ar;
}
