// Copyright JeongWoo Lee


#include "Attribute/DMSAttributeValue_Numeric.h"
#include "Effect/DMSEffectorInterface.h"
#include "Effect/DMSEffectInstance.h"

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
	float Temporal = 0.0f;
	switch(ModifierType)
	//switch(AttributeModifierType)
	{
		case EDMSModifierType_Numeric::MT_Additive:
			Temporal = CastedValue->GetValue() + CastedRightValue->GetValue();			
			break;
		case EDMSModifierType_Numeric::MT_Override:
			Temporal = CastedRightValue->GetValue();
			break;
		case EDMSModifierType_Numeric::MT_Multiplicative:
			Temporal = CastedValue->GetValue() * CastedRightValue->GetValue();
			break;
		case EDMSModifierType_Numeric::MT_Subtractive:
			Temporal = CastedValue->GetValue() - CastedRightValue->GetValue();
			break;
		default:
			break;
	}

	if (OperatorFlag == EDMSNumericOperatorFlag::OF_Clamp)
	{
		if(Temporal<FlagMinValue) Temporal = FlagMinValue;
		else if(Temporal>FlagMaxValue) Temporal = FlagMaxValue;	
	}

	CastedValue->SetValue(Temporal);
}

void UDMSAttributeValue_Numeric::OnRep_Value()
{
}

UDMSAttributeValue* UDMSAttributeValue_Numeric::GetDeltaAfterModify(const FDMSAttributeModifier& OriginalModifier, ADMSActiveEffect* OriginalActiveEffect)
{
	if(!OriginalModifier.Value->IsA<UDMSAttributeValue_Numeric>() )return nullptr;
	auto OriginalOp = Cast<UDMSAttributeModifierOp_Numeric>(OriginalModifier.ModifierOp);
	if(!OriginalOp) return nullptr;

	UDMSAttributeValue_Numeric* OutValue = NewObject<UDMSAttributeValue_Numeric>(OriginalActiveEffect);
	OutValue->SetValue(GetValue());
	OutValue->ExecuteModifier(OriginalModifier);
	OutValue->SetValue(OutValue->GetValue() - GetValue());
	
	return OutValue;
}

void UDMSAttributeValue_Numeric::GetDeltasAfterModify(const FDMSAttributeModifier& OriginalModifier, ADMSActiveEffect* OriginalActiveEffect, TArray<FDMSAttributeModifier>& OutModifiers)
{
	if(!OriginalModifier.Value->IsA<UDMSAttributeValue_Numeric>() )return;
	auto OriginalOp = Cast<UDMSAttributeModifierOp_Numeric>(OriginalModifier.ModifierOp);
	if(!OriginalOp) return;

	UDMSAttributeValue_Numeric* NewOutValue = NewObject<UDMSAttributeValue_Numeric>(OriginalActiveEffect);
	NewOutValue->SetValue(GetValue());
	NewOutValue->ExecuteModifier(OriginalModifier);
	NewOutValue->SetValue(NewOutValue->GetValue() - GetValue());

	FDMSAttributeModifier NewRevertMod;
	auto NewOp = NewObject<UDMSAttributeModifierOp_Numeric>(OriginalActiveEffect);
	NewOp->ModifierType = EDMSModifierType_Numeric::MT_Subtractive;
	NewRevertMod.ModifierOp=NewOp;
	NewRevertMod.Value=NewOutValue;

	OutModifiers.Add(NewRevertMod);
}

bool UDMSAttributeModifierOp_Numeric::Predict_Implementation(UDMSAttribute* Target, UDMSAttributeValue* ModifierValue)
{
	if (!Target) return false;

	auto CastedValue = Cast<UDMSAttributeValue_Numeric>(Target->AttributeValue);
	auto CastedRightValue = Cast<UDMSAttributeValue_Numeric>(ModifierValue);

	if(!CastedValue || !CastedRightValue) return false;

	bool rv = true;


	if (OperatorFlag == EDMSNumericOperatorFlag::OF_FailCondition)
	//if (bExistFailureCondition)
	{
		UDMSAttributeValue_Numeric* TempValue = DuplicateObject<UDMSAttributeValue_Numeric>(CastedValue,Target);

		ExecuteOp(TempValue,ModifierValue);

		float PredictValue = TempValue->GetValue();
	
		rv = PredictValue>=FlagMinValue && PredictValue<=FlagMaxValue;

		//switch (FlagOperator)
		//{
		//	case EDMSComparisonOperator::BO_Equal:
		//		rv = PredictValue == FlagMaxValue; break;
		//	case EDMSComparisonOperator::BO_Greater:
		//		rv =  PredictValue > FlagMaxValue; break;
		//	case EDMSComparisonOperator::BO_Less:
		//		rv =  PredictValue < FlagMaxValue; break;
		//	case EDMSComparisonOperator::BO_GreaterEqual:
		//		rv =  PredictValue >= FlagMaxValue; break;
		//	case EDMSComparisonOperator::BO_LessEqual:
		//		rv =  PredictValue <= FlagMaxValue; break;
		//	default: break;
		//}
		TempValue->MarkAsGarbage();
	}

	return rv;
}


