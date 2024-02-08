// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/DMSAttribute.h"


UDMSAttribute::UDMSAttribute():AttributeValue(nullptr)
{
}



void UDMSAttribute::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDMSAttribute,AttributeValue);
}


UDMSAttributeValue_Numeric::UDMSAttributeValue_Numeric() : Value(0.0f)
{
}

void UDMSAttribute::BindOnModified(const FOnAttributeModifiedSignature& iDelegate)
{
	//if (iObject==nullptr) return;
	//OnAttributeModified.AddDynamic();
	OnAttributeModified.Add(iDelegate);
}

void UDMSAttribute::ApplyModifier(const FDMSAttributeModifier& Modifier)
{
	if(!Modifier.ModifierOp->Predict(this)) return;

	AttributeValue->ExecuteOp(Modifier);
	// Make this multicast
	OnAttributeModified.Broadcast(this);
}

void UDMSAttribute::GenerateValue(const TSubclassOf<UDMSAttributeValue>& ValueClass)
{
	if(AttributeValue!=nullptr) return;
	
	AttributeValue = NewObject<UDMSAttributeValue>(this,ValueClass);
}

void UDMSAttribute::DuplicateValue(UDMSAttributeValue* Value)
{	
	if(AttributeValue!=nullptr) return;
	
	AttributeValue = DuplicateObject<UDMSAttributeValue>(Value, this);
}

void UDMSAttributeValue_Numeric::ExecuteOp_Implementation(const FDMSAttributeModifier& Modifier)
{
	auto CastedMod = Cast<UDMSAttributeModifierOp_Numeric>(Modifier.ModifierOp);
	auto ModifierValue = Cast<UDMSAttributeValue_Numeric>(Modifier.Value);
	if (!CastedMod || !ModifierValue) return;
	switch(Modifier.ModifierOp->AttributeModifierType)
	{
		case EDMSModifierType::MT_Additive:
			Value += ModifierValue->Value;
			break;
		case EDMSModifierType::MT_Override:
			Value = ModifierValue->Value;
			break;
		case EDMSModifierType::MT_Multiplicative:
			Value *= ModifierValue->Value;
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

bool UDMSAttributeModifierOp_Numeric::Predict_Implementation(UDMSAttribute* Target) const
{
	if (Target==nullptr) return false; 

	UDMSAttributeValue_Numeric* Value = Cast<UDMSAttributeValue_Numeric>(Target->AttributeValue);

	if (!Value) return false;

	float PredictValue = Value->GetValue();

	if (bExistFailureCondition)
	{
		switch (FailureConditionOperator)
		{
			case EDMSComparisonOperator::BO_Equal:
				return PredictValue == FailureConditionValue; break;
			case EDMSComparisonOperator::BO_Greater:
				return PredictValue > FailureConditionValue; break;
			case EDMSComparisonOperator::BO_Less:
				return PredictValue < FailureConditionValue; break;
			case EDMSComparisonOperator::BO_GreaterEqual:
				return PredictValue >= FailureConditionValue; break;
			case EDMSComparisonOperator::BO_LessEqual:
				return PredictValue <= FailureConditionValue; break;
			default: break;
		}
	}
	return true;
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