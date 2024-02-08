// Fill out your copyright notice in the Description page of Project Settings.

#include "EffectSet/DMSEffect_ModAtt.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Effect/DMSEffectInstance.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Effect_ModAttribute, "Effect.ModAttribute");

// 자손임을 표현하기 위해 파생 키워드들은 + ".~~" 하는 형태? ex) ModifyAttribute.Deal 
// ( 일종의 포함 관계에 속하는 이펙트들의 구분 위함. --> HP가 변화했을 때 > { HP 피해를 입었을 때 , HP 회복을 했을 때 } )

UDMSEffect_ModAtt::UDMSEffect_ModAtt() :bCreateIfNull(false)
{ 
	EffectTag = TAG_DMS_Effect_ModAttribute;
	
}

bool UDMSEffect_ModAtt::GetTargetAttComp(ADMSActiveEffect* iEI, AActor*& OutTarget, UDMSAttributeComponent*& OutComp)
{
	UObject* Object = iEI->GetApplyTargetInterface()->GetObject();

	if (Object == nullptr)
		return false;
		
	OutTarget = Cast<AActor>(Object);
	if (OutTarget == nullptr)
		return false;

	OutComp = Cast<UDMSAttributeComponent>(OutTarget->GetComponentByClass(UDMSAttributeComponent::StaticClass()));
	return OutComp!=nullptr;
}

void UDMSEffect_ModAtt::Work_Implementation(UDMSSequence* SourceSequence, ADMSActiveEffect* iEI, const FOnExecuteCompleted& OnWorkCompleted)
{
	// predict에 valid check 를 다 하고가니 이런거 필요 한가 다시 생각해보긴 해야할텐데...

	DMS_LOG_SIMPLE(TEXT("%s : ModAtt"), *iEI->GetOuter()->GetName());	

	AActor* Outer=nullptr;
	UDMSAttributeComponent* AttComp=nullptr;

	FDMSAttributeModifier Modifier;
	if (!GenerateModifier(iEI, Modifier)) {
		OnWorkCompleted.ExecuteIfBound(false); return;
	}

	if (!GetTargetAttComp(iEI, Outer,AttComp))
	{	
		if(!bCreateIfNull)	{
			OnWorkCompleted.ExecuteIfBound(false); return;
		}
		AttComp = Cast<UDMSAttributeComponent>(Outer->AddComponentByClass(UDMSAttributeComponent::StaticClass(),false,FTransform(),false));
	}
	
	if (bCreateIfNull)	AttComp->MakeAttribute(Modifier.ModifierOp->AttributeTag,Modifier.Value->GetClass());
	
	float OutValue = 0.0f;
	
	AttComp->GetAttribute(Modifier.ModifierOp->AttributeTag)->ApplyModifier(Modifier);

	OnWorkCompleted.ExecuteIfBound(true);
}

bool UDMSEffect_ModAtt::Predict_Implementation(UDMSSequence* SourceSequence, ADMSActiveEffect* iEI)
{
	AActor* Outer = nullptr;
	UDMSAttributeComponent* AttComp = nullptr;
	UDMSAttribute* Att = nullptr;
	FDMSAttributeModifier Modifier;
	float PredictValue=0.0f;
	bool rv=false;
	if (!GenerateModifier(iEI, Modifier))	return false;

	if (!GetTargetAttComp(iEI, Outer, AttComp))
	{
		if (Outer != nullptr && bCreateIfNull ) 
			AttComp = Cast<UDMSAttributeComponent>(Outer->AddComponentByClass(UDMSAttributeComponent::StaticClass(), false, FTransform(), false));

		else return false;
	}
	Att = AttComp->GetAttribute(Modifier.ModifierOp->AttributeTag);
	if (Att == nullptr) {
		if (bCreateIfNull) Att = AttComp->MakeAttribute(Modifier.ModifierOp->AttributeTag,Modifier.Value->GetClass()); 
		else return false;
	}

	rv = Modifier.ModifierOp->Predict(Att);

	return rv;
}

FGameplayTagContainer UDMSEffect_ModAtt::GetEffectTags_Implementation()
{ 
	FGameplayTagContainer Rv(EffectTag);
//	Rv.AddTag(GenerateModifier().AttributeTag);
	return Rv;
}

FGameplayTagContainer UDMSEffect_ModAtt_Static::GetEffectTags_Implementation()
{ 
	FGameplayTagContainer Rv(EffectTag);
	Rv.AppendTags(StaticModifier.ModifierOp->AttributeTag);
	return Rv;
}


UDMSEffect_ModAtt_Variable::UDMSEffect_ModAtt_Variable()
{
	//DataPicker.ValueSelector = CreateDefaultSubobject<UDMSValueSelector_Attribute>("ValueSelector");
}

bool UDMSEffect_ModAtt_Variable::GenerateModifier_Implementation(ADMSActiveEffect* EI,FDMSAttributeModifier& OutModifier)
{
	// Get Input Data ( Skip if data doesn't exist. )
	//UDMSDataObject* ValueData = nullptr;
	UDMSDataObject* ValueData = DataPicker.Get(EI->DataSet);

	if (ValueData == nullptr || !ValueData->TypeCheck<FDMSAttributeModifier>()) 
		return false;

	else
		OutModifier = ValueData->Get<FDMSAttributeModifier>();
	
	return true;
}

bool UDMSEffect_ModAtt_FromAttribute::GenerateModifier_Implementation(ADMSActiveEffect* EI,FDMSAttributeModifier& OutModifier)
{
	//UDMSDataObject* ValueData = nullptr;
	if (!ModifierOp) return false;

	UDMSAttributeComponent* AEAttComp = EI->GetComponentByClass<UDMSAttributeComponent>();

	if(!AEAttComp) return false;

	auto AEAtt = AEAttComp->GetAttribute(ActiveEffectValueTags);

	if (AEAtt == nullptr) return false;

	auto ModifierValue = Cast<UDMSAttributeValue_Numeric>(AEAtt->AttributeValue);

	if (!ModifierValue) return false;
	
	
	OutModifier.ModifierOp = ModifierOp;
	OutModifier.Value = ModifierValue;
	return true;
}
