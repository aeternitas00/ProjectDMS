// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/DMSAttributeValue_Numeric.h"
#include "Effect/DMSEffectorInterface.h"

UDMSAttributeValue_Numeric::UDMSAttributeValue_Numeric() : Value(0.0f)
{
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
		case EDMSModifierType::MT_Subtractive:
			CastedValue->SetValue(CastedValue->GetValue() - CastedRightValue->GetValue());
			break;
		default:
			break;
	}
}

void UDMSAttributeValue_Numeric::GetDeltaAfterModify(const FDMSAttributeModifier& Modifier,TObjectPtr<UDMSAttributeValue>& OutValue)
{
	if(!OutValue->IsA<UDMSAttributeValue_Numeric>() || !Modifier.Value->IsA<UDMSAttributeValue_Numeric>() )return;
	auto CastedOutValue = Cast<UDMSAttributeValue_Numeric>(OutValue);
	auto CastedModifierValue = Cast<UDMSAttributeValue_Numeric>(Modifier.Value);
	switch(Modifier.ModifierOp->AttributeModifierType)
	{
		case EDMSModifierType::MT_Additive:
			CastedOutValue->Value=CastedModifierValue->Value;
			break;
		case EDMSModifierType::MT_Subtractive:
			CastedOutValue->Value=CastedModifierValue->Value * -1;
			break;
		case EDMSModifierType::MT_Override:
			CastedOutValue->Value=CastedModifierValue->Value - Value;
			break;
		case EDMSModifierType::MT_Multiplicative:
			CastedOutValue->Value = (Value*CastedModifierValue->Value) - Value;
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


