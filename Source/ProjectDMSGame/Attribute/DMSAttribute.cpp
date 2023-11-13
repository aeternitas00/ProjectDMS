// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/DMSAttribute.h"

bool UDMSAttribute::TryModAttribute(const FDMSAttributeModifier& Modifier, float& OutValue, bool Apply)
{
	switch (Modifier.ValueModifier.ModifierType)
	{
		case EDMSModifierType::MT_Add:
			OutValue = Value + Modifier.ValueModifier.Value;
			if(Apply)	Value += Modifier.ValueModifier.Value; 			
			break;
		case EDMSModifierType::MT_Sub:
			OutValue = Value - Modifier.ValueModifier.Value;
			if (Apply)	Value -= Modifier.ValueModifier.Value;			
			break;
		case EDMSModifierType::MT_Set:
			OutValue = Modifier.ValueModifier.Value;
			if (Apply)	Value = Modifier.ValueModifier.Value;
			break;
		case EDMSModifierType::MT_Mul:
			OutValue = Value * Modifier.ValueModifier.Value;
			if (Apply)	Value *= Modifier.ValueModifier.Value; 			
			break;
		case EDMSModifierType::MT_Div:
			if (Modifier.ValueModifier.Value != 0)	{
				OutValue = Value / Modifier.ValueModifier.Value;
				if (Apply) Value /= Modifier.ValueModifier.Value;
			}
			else	return false;
			break;
		default: 
			break;
	}
	if(Apply) OnAttributeModified.Broadcast(this);
	return true;
}

void UDMSAttribute::BindOnModified(const FOnAttributeModifiedSignature& iDelegate)
{
	//if (iObject==nullptr) return;
	//OnAttributeModified.AddDynamic();
	OnAttributeModified.Add(iDelegate);
}

void UDMSAttribute::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);
	auto s = ToSerialized();
	Ar << s;

	//if (Ar.IsSaving()) {}
	//else if (Ar.IsLoading()) {}
}