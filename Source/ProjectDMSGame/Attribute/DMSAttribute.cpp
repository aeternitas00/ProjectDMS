// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/DMSAttribute.h"

bool UDMSAttribute::ModAttribute(const FDMSAttributeModifier& Modifier)
{
	switch (Modifier.ModifierType)
	{
		case EDMSModifierType::MT_Add:
			Value += Modifier.Value; break;
		case EDMSModifierType::MT_Sub:
			Value -= Modifier.Value; break;
		case EDMSModifierType::MT_Set:
			Value = Modifier.Value; break;
		case EDMSModifierType::MT_Mul:
			Value *= Modifier.Value; break;
		case EDMSModifierType::MT_Div:
			if (Modifier.Value != 0) {
				Value /= Modifier.Value; break;
			}
			else
				return false;
			break;
		default: 
			return false;
			break;
	}
	return true;
}