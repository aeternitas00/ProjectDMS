// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/DMSAttribute.h"


UDMSAttribute::UDMSAttribute()
{
}



void UDMSAttribute::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// .. 
}


UDMSAttribute_Numeric::UDMSAttribute_Numeric() : Value(0.0f)
{
}

void UDMSAttribute::BindOnModified(const FOnAttributeModifiedSignature& iDelegate)
{
	//if (iObject==nullptr) return;
	//OnAttributeModified.AddDynamic();
	OnAttributeModified.Add(iDelegate);
}

void UDMSAttribute::ApplyModifier(UDMSAttributeModifier* Modifier)
{
	if(!Modifier->Predict(this)) return;

	AttributeValue->ExecuteOp(Modifier);
	// Make this multicast
	OnAttributeModified.Broadcast(this);
}

void UDMSAttributeValue::ExecuteOp_Implementation(UDMSAttributeModifier* Modifier)
{

}

void UDMSAttributeValue_Numeric::ExecuteOp_Implementation(UDMSAttributeModifier* Modifier)
{
	auto ValueModifier = Cast<UDMSAttributeValue_Numeric>(Modifier->AttributeValue);
	switch(Modifier->AttributeModifierType)
	{
		case EDMSModifierType::MT_Additive:
			Value += ValueModifier->Value;
			break;
		case EDMSModifierType::MT_Override:
			Value = ValueModifier->Value;
			break;
		case EDMSModifierType::MT_Multiplicative:
			Value *= ValueModifier->Value;
			break;
		default:
			break;
	}


}

void UDMSAttributeValue_Numeric::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDMSAttributeValue_Numeric, Value);
}


//void UDMSAttribute_Numeric::Serialize(FArchive& Ar)
//{
//	Super::Serialize(Ar);
//	auto s = ToSerialized();
//	Ar << s;
//
//	//if (Ar.IsSaving()) {}
//	//else if (Ar.IsLoading()) {}
//}



bool UDMSAttributeModifier_Numeric::Predict_Implementation(UDMSAttribute* Target) const
{
	return false;
}
