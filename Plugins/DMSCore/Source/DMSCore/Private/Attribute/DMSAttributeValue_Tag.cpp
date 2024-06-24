// Copyright JeongWoo Lee


#include "Attribute/DMSAttributeValue_Tag.h"
#include "Effect/DMSEffectInstance.h"

//void UDMSAttributeValue_Tag::GetDeltaAfterModify(const FDMSAttributeModifier& Modifier, TObjectPtr<UDMSAttributeValue>& OutValue)
//{
//}

void UDMSAttributeValue_Tag::GetDeltasAfterModify(const FDMSAttributeModifier& OriginalModifier, ADMSActiveEffect* OriginalActiveEffect, TArray<FDMSAttributeModifier>& OutModifiers)
{
	if(!OriginalModifier.Value->IsA<UDMSAttributeValue_Tag>() )return;
	auto CastedOriginalOp = Cast<UDMSAttributeModifierOp_Tag>(OriginalModifier.ModifierOp);
	if(!CastedOriginalOp) return;
	
	FDMSAttributeModifier NewRevertSubMod, NewRevertAddMod;
	auto SubModOp =  NewObject<UDMSAttributeModifierOp_Tag>(OriginalActiveEffect);
	SubModOp->ModifierType = EDMSModifierType_Tag::MT_Remove;
	NewRevertSubMod.ModifierOp = SubModOp;
	
	auto AddModOp =  NewObject<UDMSAttributeModifierOp_Tag>(OriginalActiveEffect);	
	AddModOp->ModifierType = EDMSModifierType_Tag::MT_Add;;
	NewRevertAddMod.ModifierOp = AddModOp;

	auto CastedSubValue = NewObject<UDMSAttributeValue_Tag>(OriginalActiveEffect);
	auto CastedAddValue = NewObject<UDMSAttributeValue_Tag>(OriginalActiveEffect);
	NewRevertSubMod.Value = CastedSubValue;
	NewRevertAddMod.Value = CastedAddValue;

	auto CastedModifierValue = Cast<UDMSAttributeValue_Tag>(OriginalModifier.Value)->GetValue();
	
	switch(CastedOriginalOp->ModifierType)
	{
		case EDMSModifierType_Tag::MT_Add:
			for(auto Adding : CastedModifierValue){
				if(!Value.HasTagExact(Adding)) CastedSubValue->AddSingleValue(Adding);
			}
			break;
		case EDMSModifierType_Tag::MT_Remove:
			for(auto Removing : CastedModifierValue){
				if(Value.HasTagExact(Removing)) CastedAddValue->AddSingleValue(Removing);
			}
			break;
		case EDMSModifierType_Tag::MT_Override:
			for(auto Adding : CastedModifierValue){
				if(!Value.HasTagExact(Adding)) CastedSubValue->AddSingleValue(Adding);
			}
			for(auto& Removing : Value){
				if(!CastedModifierValue.HasTagExact(Removing)) CastedAddValue->AddSingleValue(Removing);
			}
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

	switch(ModifierType)
	//switch(AttributeModifierType)
	{
		case EDMSModifierType_Tag::MT_Add:
			CastedValue->AddValue(CastedRightValue->GetValue());
			break;
		case EDMSModifierType_Tag::MT_Override:
			CastedValue->SetValue(CastedRightValue->GetValue());
			break;
		case EDMSModifierType_Tag::MT_Remove:
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

	switch(ModifierType)
	//switch(AttributeModifierType)
	{
		case EDMSModifierType_Tag::MT_Add:
			if(CastedValue->GetValue().Num()==0) {rv=true; break;}
			for(auto& SearchItem : CastedValue->GetValue())
			{
				if( !CastedRightValue->GetValue().HasTagExact(SearchItem) )
				{ rv=true; break; }
			}
			break;
		case EDMSModifierType_Tag::MT_Remove:
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
