// Copyright JeongWoo Lee


#include "Attribute/DMSAttributeValue_Tag.h"
#include "Effect/DMSEffectInstance.h"

//void UDMSAttributeValue_Tag::GetDeltaAfterModify(const FDMSAttributeModifier& Modifier, TObjectPtr<UDMSAttributeValue>& OutValue)
//{
//}

void UDMSAttributeValue_Tag::GetDeltasAfterModify(const FDMSAttributeModifier& OriginalModifier, ADMSActiveEffect* OriginalActiveEffect, TArray<FDMSAttributeModifier>& OutModifiers)
{
	if(!OriginalModifier.Value->IsA<UDMSAttributeValue_Tag>() )return;
	FDMSAttributeModifier NewRevertSubMod, NewRevertAddMod;
	NewRevertSubMod.ModifierOp = NewObject<UDMSAttributeModifierOp_Tag>(OriginalActiveEffect);
	NewRevertSubMod.ModifierOp->AttributeModifierType = EDMSModifierType::MT_Subtractive;
	NewRevertAddMod.ModifierOp = NewObject<UDMSAttributeModifierOp_Tag>(OriginalActiveEffect);
	NewRevertAddMod.ModifierOp->AttributeModifierType = EDMSModifierType::MT_Additive;

	auto CastedSubValue = NewObject<UDMSAttributeValue_Tag>(OriginalActiveEffect);
	auto CastedAddValue = NewObject<UDMSAttributeValue_Tag>(OriginalActiveEffect);
	NewRevertSubMod.Value = CastedSubValue;
	NewRevertAddMod.Value = CastedAddValue;

	auto CastedModifierValue = Cast<UDMSAttributeValue_Tag>(OriginalModifier.Value)->GetValue();
	switch(OriginalModifier.ModifierOp->AttributeModifierType)
	{
		case EDMSModifierType::MT_Additive:
			for(auto Adding : CastedModifierValue){
				if(!Value.HasTagExact(Adding)) CastedSubValue->AddSingleValue(Adding);
			}
			break;
		case EDMSModifierType::MT_Subtractive:
			for(auto Removing : CastedModifierValue){
				if(Value.HasTagExact(Removing)) CastedAddValue->AddSingleValue(Removing);
			}
			break;
		case EDMSModifierType::MT_Override:
			for(auto Adding : CastedModifierValue){
				if(!Value.HasTagExact(Adding)) CastedSubValue->AddSingleValue(Adding);
			}
			for(auto& Removing : Value){
				if(!CastedModifierValue.HasTagExact(Removing)) CastedAddValue->AddSingleValue(Removing);
			}
			break;
		case EDMSModifierType::MT_Multiplicative:
			break;
		default:
			break;
	}
	OutModifiers.Add(NewRevertSubMod);
	OutModifiers.Add(NewRevertAddMod);
}

void UDMSAttributeValue_Tag::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDMSAttributeValue_Tag, Value);
}

void UDMSAttributeModifierOp_Tag::ExecuteOp_Implementation(UDMSAttributeValue* AttValue, UDMSAttributeValue* ModifierValue)
{
	auto CastedValue = Cast<UDMSAttributeValue_Tag>(AttValue);
	auto CastedRightValue = Cast<UDMSAttributeValue_Tag>(ModifierValue);
	if(!CastedValue || !CastedRightValue) return;

	switch(AttributeModifierType)
	{
		case EDMSModifierType::MT_Additive:
			CastedValue->AddValue(CastedRightValue->GetValue());
			break;
		case EDMSModifierType::MT_Override:
			CastedValue->SetValue(CastedRightValue->GetValue());
			break;
		case EDMSModifierType::MT_Subtractive:
			CastedValue->RemoveValue(CastedRightValue->GetValue());
			break;
		default:
			break;
	}
}

bool UDMSAttributeModifierOp_Tag::Predict_Implementation(UDMSAttribute* Target, UDMSAttributeValue* ModifierValue)
{
	auto CastedValue = Cast<UDMSAttributeValue_Tag>(Target->AttributeValue);
	auto CastedRightValue = Cast<UDMSAttributeValue_Tag>(ModifierValue);
	if(!CastedValue || !CastedRightValue) return false;
	bool rv=false;
	switch(AttributeModifierType)
	{
		case EDMSModifierType::MT_Additive:
			if(CastedValue->GetValue().Num()==0) {rv=true; break;}
			for(auto& SearchItem : CastedValue->GetValue())
			{
				if( !CastedRightValue->GetValue().HasTagExact(SearchItem) )
				{ rv=true; break; }
			}
			break;
		case EDMSModifierType::MT_Subtractive:
			if(CastedRightValue->GetValue().Num()==0) {rv=true; break;}
			for(auto& SearchItem : CastedRightValue->GetValue())
			{
				if( CastedValue->GetValue().HasTagExact(SearchItem) )
				{ rv=true; break; }
			}
			break;
		default:
			rv=true; break;
	}
	return rv;
}
