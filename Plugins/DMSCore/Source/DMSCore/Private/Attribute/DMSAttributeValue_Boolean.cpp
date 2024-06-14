// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/DMSAttributeValue_Boolean.h"
#include "Effect/DMSEffectorInterface.h"
#include "Effect/DMSEffectInstance.h"

UDMSAttributeValue_Boolean::UDMSAttributeValue_Boolean() : Value(false)
{
}

void UDMSAttributeValue_Boolean::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDMSAttributeValue_Boolean, Value);
}

void UDMSAttributeModifierOp_Boolean::ExecuteOp_Implementation(UDMSAttributeValue* AttValue, UDMSAttributeValue* ModifierValue)
{
	auto CastedValue = Cast<UDMSAttributeValue_Boolean>(AttValue);
	auto CastedRightValue = Cast<UDMSAttributeValue_Boolean>(ModifierValue);
	if(!CastedValue || !CastedRightValue) return;

	switch(_ModifierType)
	{
		case EDMSModifierType_Boolean::MT_Set:
			CastedValue->SetValue(CastedRightValue->GetValue());
			break;
		case EDMSModifierType_Boolean::MT_AndEqual:
			CastedValue->SetValue(CastedValue->GetValue() && CastedRightValue->GetValue());
			break;
		case EDMSModifierType_Boolean::MT_OrEqual:
			CastedValue->SetValue(CastedValue->GetValue() || CastedRightValue->GetValue());
			break;
		default:
			break;
	}
}

void UDMSAttributeValue_Boolean::GetDeltasAfterModify(const FDMSAttributeModifier& OriginalModifier, ADMSActiveEffect* OriginalActiveEffect, TArray<FDMSAttributeModifier>& OutModifiers)
{
	if(!OriginalModifier.Value->IsA<UDMSAttributeValue_Boolean>() )return;
	FDMSAttributeModifier NewRevertMod;
	UDMSAttributeModifierOp_Boolean* Op;
	NewRevertMod.ModifierOp = Op = NewObject<UDMSAttributeModifierOp_Boolean>(OriginalActiveEffect);
	Op->_ModifierType = EDMSModifierType_Boolean::MT_Set;
	UDMSAttributeValue_Boolean* CastedOutValue;
	NewRevertMod.Value = CastedOutValue = NewObject<UDMSAttributeValue_Boolean>(OriginalActiveEffect);
	CastedOutValue->SetValue(Value);
	OutModifiers.Add(NewRevertMod);
}

bool UDMSAttributeModifierOp_Boolean::Predict_Implementation(UDMSAttribute* Target, UDMSAttributeValue* ModifierValue)
{
	if (!Target) return false;

	auto CastedValue = Cast<UDMSAttributeValue_Boolean>(Target->AttributeValue);
	auto CastedRightValue = Cast<UDMSAttributeValue_Boolean>(ModifierValue);

	if(!CastedValue || !CastedRightValue) return false;

	return true;
}


