#include "Common/DMSCommons.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_EffectKeyword_PlayCard,"EffectKeyword.Play.Card");
UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_EffectKeyword_Cost, "EffectKeyword.Cost");

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_EffectType_Activatable, "EffectType.Activatable");
UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_EffectType_Effect, "EffectType.Effect");
UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_EffectType_Cost, "EffectType.Cost");
UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_EffectType_Commit, "EffectType.Commit");

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Attribute_Resource,"Attribute.Resource");
UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Attribute_ActionPoint,"Attribute.ActionPoint");
UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Attribute_HP,"Attribute.HP");
UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Attribute_STR,"Attribute.STR");
UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Attribute_INT,"Attribute.INT");
UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Attribute_DEX,"Attribute.DEX");
UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Attribute_SavedSkillBonus,"Attribute.SavedSkillBonus");

void UDMSDataObjectSet::Inherit(UDMSDataObjectSet* Parent)
{
	if (Parent==nullptr)return;
	for (auto r : Parent->DataMap)
	{
		if (r.Value==nullptr) continue;
		if (r.Value->IsInheriting()){
			UDMSDataObject* tDS = NewObject<UDMSDataObject>(GetOuter());
			tDS->CopyValue(r.Value);
			tDS->SetInheriting(true);
			DataMap.Add(r.Key, tDS);
		}
	}
}

void UDMSDataObjectSet::Merge(UDMSDataObjectSet* iSet)
{
	if (iSet == nullptr)return;
	for (auto r : iSet->DataMap)
	{
		if (r.Value == nullptr) continue;
		// Handling confliction?
		//UDMSDataObject* tDS = DuplicateObject<UDMSDataObject>(r.Value, GetOuter());
		DataMap.Add(r);
	}
}
