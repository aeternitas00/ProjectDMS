// Fill out your copyright notice in the Description page of Project Settings.

#include "EffectSet/DMSEffect_ModAtt.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Effect/DMSEffectInstance.h"
#include "Sequence/DMSSequence.h"

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

	//DMS_LOG_SIMPLE(TEXT("%s : ModAtt"), *iEI->GetOuter()->GetName());	

	AActor* Outer=nullptr;
	UDMSAttributeComponent* AttComp=nullptr;

	FDMSAttributeModifier Modifier;
	if (!GenerateModifier(iEI, SourceSequence, Modifier)) {
		OnWorkCompleted.ExecuteIfBound(false); return;
	}

	if (!GetTargetAttComp(iEI, Outer,AttComp))
	{	
		if(!bCreateIfNull)	{
			OnWorkCompleted.ExecuteIfBound(false); return;
		}
		AttComp = Cast<UDMSAttributeComponent>(Outer->AddComponentByClass(UDMSAttributeComponent::StaticClass(),false,FTransform(),false));
	}
	
	if (bCreateIfNull)	AttComp->MakeAttribute(TargetAttributeTags,Modifier.Value->GetClass());
	
	float OutValue = 0.0f;
	
	AttComp->GetAttribute(TargetAttributeTags)->ApplyModifier(Modifier);

	OnWorkCompleted.ExecuteIfBound(true);
}

bool UDMSEffect_ModAtt::Predict_Implementation(UDMSSequence* SourceSequence, ADMSActiveEffect* iEI)
{
	AActor* Outer = nullptr;
	UDMSAttributeComponent* AttComp = nullptr;
	UDMSAttribute* Att = nullptr;
	FDMSAttributeModifier Modifier;

	bool rv=false;
	if (!GenerateModifier(iEI, SourceSequence, Modifier))	return false;

	if (!GetTargetAttComp(iEI, Outer, AttComp))
	{
		if (Outer != nullptr && bCreateIfNull ) 
			AttComp = Cast<UDMSAttributeComponent>(Outer->AddComponentByClass(UDMSAttributeComponent::StaticClass(), false, FTransform(), false));

		else return false;
	}
	Att = AttComp->GetAttribute(TargetAttributeTags);
	if (Att == nullptr) {
		if (bCreateIfNull) Att = AttComp->MakeAttribute(TargetAttributeTags,Modifier.Value->GetClass()); 
		else return false;
	}

	rv = Modifier.ModifierOp->Predict(Att,Modifier.Value);

	return rv;
}

FGameplayTagContainer UDMSEffect_ModAtt::GetEffectTags_Implementation()
{ 
	FGameplayTagContainer Rv(EffectTag);
	Rv.AppendTags(TargetAttributeTags);
	return Rv;
}


UDMSEffect_ModAtt_Variable::UDMSEffect_ModAtt_Variable()
{
	//DataPicker.ValueSelector = CreateDefaultSubobject<UDMSValueSelector_Attribute>("ValueSelector");
}

bool UDMSEffect_ModAtt_Variable::GenerateModifier_Implementation(ADMSActiveEffect* EI, UDMSSequence* SourceSequence, FDMSAttributeModifier& OutModifier)
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

bool UDMSEffect_ModAtt_FromAttribute::GenerateModifier_Implementation(ADMSActiveEffect* EI, UDMSSequence* SourceSequence, FDMSAttributeModifier& OutModifier)
{
	//UDMSDataObject* ValueData = nullptr;
	if (!ModifierOp) return false;

	UDMSAttributeComponent* AttComp=nullptr;
	
	switch (ValueAttributeSource) {
		case EDMSAttributeSourceFlag::AE: 
			AttComp= EI->GetComponentByClass<UDMSAttributeComponent>(); break;
		case EDMSAttributeSourceFlag::SourcePlayer: 
			AttComp= SourceSequence->GetSourcePlayer()->GetComponentByClass<UDMSAttributeComponent>(); break;
		case EDMSAttributeSourceFlag::SourceObject: 
			AttComp= SourceSequence->GetSourceObject()->GetComponentByClass<UDMSAttributeComponent>(); break;
		default: break;
	}
	if(!AttComp) return false;

	auto Att = AttComp->GetAttribute(ValueAttributeTags);

	if (Att == nullptr) return false;

	// 이거 값 바꿔야하므로 사본구해오기 정도로 해야함.
	auto ModifierValue = Cast<UDMSAttributeValue_Numeric>(Att->AttributeValue);

	if (!ModifierValue) return false;
	
	ModifierValue = DuplicateObject<UDMSAttributeValue_Numeric>(ModifierValue,EI);

	for (auto& Prc : ValueProcessers)
		Prc->Process(ModifierValue);
	
	
	OutModifier.ModifierOp = ModifierOp;
	OutModifier.Value = ModifierValue;
	return true;
}

void UDMSAttributeValueProcesser::Process_Implementation(UObject* iObject)
{
	if (iObject==nullptr || !iObject->IsA<UDMSAttributeValue>()) return;

	auto AttValue = Cast<UDMSAttributeValue>(iObject);

	AttValue->ExecuteModifier(ProcessorModifier);
	
}
