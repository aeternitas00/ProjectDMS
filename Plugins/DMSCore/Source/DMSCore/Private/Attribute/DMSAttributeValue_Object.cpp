// Fill out your copyright notice in the Description page of Project Settings.

#include "Attribute/DMSAttributeValue_Object.h"
#include "Effect/DMSEffectorInterface.h"

void UDMSAttributeValue_Object::GetDeltaAfterModify(const FDMSAttributeModifier& Modifier, TObjectPtr<UDMSAttributeValue>& OutValue)
{
}

void UDMSAttributeValue_Object::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDMSAttributeValue_Object, Value);
}

void UDMSAttributeModifierOp_Object::ExecuteOp_Implementation(UDMSAttributeValue* AttValue, UDMSAttributeValue* ModifierValue)
{
	auto CastedValue = Cast<UDMSAttributeValue_Object>(AttValue);
	auto CastedRightValue = Cast<UDMSAttributeValue_Object>(ModifierValue);
	if(!CastedValue || !CastedRightValue) return;

	switch(AttributeModifierType)
	{
		case EDMSModifierType::MT_Additive:
			CastedValue->AppendValue(CastedRightValue->GetValue());
			break;
		case EDMSModifierType::MT_Override:
			CastedValue->SetValue(CastedRightValue->GetValue());
			break;
		case EDMSModifierType::MT_Subtractive:
			CastedValue->RemoveValues(CastedRightValue->GetValue());
			break;
		default:
			break;
	}
}

bool UDMSAttributeModifierOp_Object::Predict_Implementation(UDMSAttribute* Target, UDMSAttributeValue* ModifierValue)
{
	// false if remove failed?
	return true;
}
