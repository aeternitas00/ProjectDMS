// Copyright JeongWoo Lee

#include "Attribute/DMSAttributeValue_Object.h"
#include "Effect/DMSEffectorInterface.h"
#include "Effect/DMSEffectInstance.h"

//void UDMSAttributeValue_Object::GetDeltaAfterModify(const FDMSAttributeModifier& Modifier, TObjectPtr<UDMSAttributeValue>& OutValue)
//{
	//auto CastedValue = Cast<UDMSAttributeValue_Object>(OutValue);
	//auto CastedModifierValue = Cast<UDMSAttributeValue_Object>(Modifier.Value);
	//if(!CastedValue || !CastedModifierValue) return;
	//switch(Modifier.ModifierOp->AttributeModifierType){
	//	case EDMSModifierType::MT_Additive:
	//		CastedValue->AppendValue(CastedRightValue->GetValue());
	//		break;
	//	case EDMSModifierType::MT_Override:
	//		CastedValue->SetValue(CastedRightValue->GetValue());
	//		break;
	//	case EDMSModifierType::MT_Subtractive:
	//		CastedValue->RemoveValues(CastedRightValue->GetValue());
	//		break;
	//	default:
	//		break;
	//}
//}

void UDMSAttributeValue_Object::GetDeltasAfterModify(const FDMSAttributeModifier& OriginalModifier, ADMSActiveEffect* OriginalActiveEffect, TArray<FDMSAttributeModifier>& OutModifiers)
{
	if(!OriginalModifier.Value->IsA<UDMSAttributeValue_Object>() )return;
	auto CastedOriginalOp = Cast<UDMSAttributeModifierOp_Object>(OriginalModifier.ModifierOp);
	if(!CastedOriginalOp) return;

	FDMSAttributeModifier NewRevertSubMod, NewRevertAddMod;

	auto SubModOp =  NewObject<UDMSAttributeModifierOp_Object>(OriginalActiveEffect);
	SubModOp->ModifierType = EDMSModifierType_Object::MT_Remove;
	NewRevertSubMod.ModifierOp = SubModOp;

	auto AddModOp =  NewObject<UDMSAttributeModifierOp_Object>(OriginalActiveEffect);	
	AddModOp->ModifierType = EDMSModifierType_Object::MT_Add;;
	NewRevertAddMod.ModifierOp = AddModOp;

	auto CastedSubValue = NewObject<UDMSAttributeValue_Object>(OriginalActiveEffect);
	auto CastedAddValue = NewObject<UDMSAttributeValue_Object>(OriginalActiveEffect);
	NewRevertSubMod.Value = CastedSubValue;
	NewRevertAddMod.Value = CastedAddValue;

	auto CastedModifierValue = Cast<UDMSAttributeValue_Object>(OriginalModifier.Value)->GetValue();
	switch(CastedOriginalOp->ModifierType)
	{
		case EDMSModifierType_Object::MT_Add:
			for(auto Adding : CastedModifierValue){
				if(!Value.Contains(Adding)) CastedSubValue->AddValue(Adding);
			}
			break;
		case EDMSModifierType_Object::MT_Remove:
			for(auto Removing : CastedModifierValue){
				if(Value.Contains(Removing)) CastedAddValue->AddValue(Removing);
			}
			break;
		case EDMSModifierType_Object::MT_Override:
			for(auto Adding : CastedModifierValue){
				if(!Value.Contains(Adding)) CastedSubValue->AddValue(Adding);
			}
			for(auto& Removing : Value){
				if(!CastedModifierValue.Contains(Removing)) CastedAddValue->AddValue(Removing);
			}
			break;
		default:
			break;
	}
	OutModifiers.Add(NewRevertSubMod);
	OutModifiers.Add(NewRevertAddMod);
}

void UDMSAttributeValue_Object::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDMSAttributeValue_Object, Value);
}

void UDMSAttributeModifierOp_Object::ExecuteOp_Implementation(UDMSAttributeValue* AttValue, UDMSAttributeValue* ModifierValue)
{
	auto CastedValue = Cast<UDMSAttributeValue_Object>(AttValue);
	auto CastedRightValue = Cast<UDMSAttributeValue_Object>(ModifierValue);
	if(!CastedValue || !CastedRightValue) return;

	switch(ModifierType)
	//switch(AttributeModifierType)
	{
		case EDMSModifierType_Object::MT_Add:
			CastedValue->AppendValue(CastedRightValue->GetValue());
			break;
		case EDMSModifierType_Object::MT_Override:
			CastedValue->SetValue(CastedRightValue->GetValue());
			break;
		case EDMSModifierType_Object::MT_Remove:
			CastedValue->RemoveValues(CastedRightValue->GetValue());
			break;
		default:
			break;
	}
}

bool UDMSAttributeModifierOp_Object::Predict_Implementation(UDMSAttribute* Target, UDMSAttributeValue* ModifierValue)
{
	auto CastedValue = Cast<UDMSAttributeValue_Object>(Target->AttributeValue);
	auto CastedRightValue = Cast<UDMSAttributeValue_Object>(ModifierValue);
	if(!CastedValue || !CastedRightValue) return false;
	bool rv=false;
	switch(ModifierType)
	//switch(AttributeModifierType)
	{
		case EDMSModifierType_Object::MT_Add:
			if(CastedValue->GetValue().Num()==0) {rv=true; break;}
			for(auto& SearchItem : CastedValue->GetValue())
			{
				if( !CastedRightValue->GetValue().Contains(SearchItem) )
				{ rv=true; break; }
			}
			break;
		case EDMSModifierType_Object::MT_Remove:
			if(CastedRightValue->GetValue().Num()==0) {rv=true; break;}
			for(auto& SearchItem : CastedRightValue->GetValue())
			{
				if( CastedValue->GetValue().Contains(SearchItem) )
				{ rv=true; break; }
			}
			break;
		default:
			rv=true; break;
	}
	return rv;
}
