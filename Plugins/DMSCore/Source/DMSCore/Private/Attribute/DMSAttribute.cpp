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
	if(!Modifier.ModifierOp->Predict(this,Modifier.Value)) return;

	AttributeValue->ExecuteModifier(Modifier);
	// Make this multicast
	OnAttributeModified.Broadcast(this);
}

void UDMSAttribute::GenerateValue(const TSubclassOf<UDMSAttributeValue>& ValueClass)
{
	if(AttributeValue!=nullptr) {AttributeValue->MarkAsGarbage();}
	
	AttributeValue = NewObject<UDMSAttributeValue>(this,ValueClass);
}

void UDMSAttribute::DuplicateValue(UDMSAttributeValue* Value)
{	
	if(AttributeValue!=nullptr) {AttributeValue->MarkAsGarbage();}
	
	AttributeValue = DuplicateObject<UDMSAttributeValue>(Value, this);
}

void UDMSAttributeValue_Numeric::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDMSAttributeValue_Numeric, Value);
}

void UDMSAttributeModifierOp_Numeric::ExecuteOp_Implementation(UDMSAttributeValue* AttValue, UDMSAttributeValue* ModifierValue)
{
	auto CastedValue = Cast<UDMSAttributeValue_Numeric>(AttValue);
	auto CastedRightValue = Cast<UDMSAttributeValue_Numeric>(ModifierValue);
	if(!CastedValue || !CastedRightValue) return;

	switch(AttributeModifierType)
	{
		case EDMSModifierType::MT_Additive:
			CastedValue->SetValue(CastedValue->GetValue() + CastedRightValue->GetValue());
			break;
		case EDMSModifierType::MT_Override:
			CastedValue->SetValue(CastedRightValue->GetValue());
			break;
		case EDMSModifierType::MT_Multiplicative:
			CastedValue->SetValue(CastedValue->GetValue() * CastedRightValue->GetValue());
			break;
		default:
			break;
	}
}

bool UDMSAttributeModifierOp_Numeric::Predict_Implementation(UDMSAttribute* Target, UDMSAttributeValue* ModifierValue)
{
	if (!Target) return false;

	auto CastedValue = Cast<UDMSAttributeValue_Numeric>(Target->AttributeValue);
	auto CastedRightValue = Cast<UDMSAttributeValue_Numeric>(ModifierValue);

	if(!CastedValue || !CastedRightValue) return false;

	bool rv = true;

	if (bExistFailureCondition)
	{
		UDMSAttributeValue_Numeric* TempValue = DuplicateObject<UDMSAttributeValue_Numeric>(CastedValue,Target);

		ExecuteOp(TempValue,ModifierValue);

		float PredictValue = TempValue->GetValue();
	
		switch (FailureConditionOperator)
		{
			case EDMSComparisonOperator::BO_Equal:
				rv = PredictValue == FailureConditionValue; break;
			case EDMSComparisonOperator::BO_Greater:
				rv =  PredictValue > FailureConditionValue; break;
			case EDMSComparisonOperator::BO_Less:
				rv =  PredictValue < FailureConditionValue; break;
			case EDMSComparisonOperator::BO_GreaterEqual:
				rv =  PredictValue >= FailureConditionValue; break;
			case EDMSComparisonOperator::BO_LessEqual:
				rv =  PredictValue <= FailureConditionValue; break;
			default: break;
		}
		TempValue->MarkAsGarbage();
	}

	return rv;
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

void UDMSAttributeValue::ExecuteModifier_Implementation(const FDMSAttributeModifier& Modifier) { Modifier.ModifierOp->ExecuteOp(this, Modifier.Value); }
