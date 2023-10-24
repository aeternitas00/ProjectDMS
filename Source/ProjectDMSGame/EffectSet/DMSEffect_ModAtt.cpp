// Fill out your copyright notice in the Description page of Project Settings.

#include "EffectSet/DMSEffect_ModAtt.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Effect/DMSEffectInstance.h"


UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Effect_ModAttribute, "Effect.ModAttribute");

// �ڼ����� ǥ���ϱ� ���� �Ļ� Ű������� + ".~~" �ϴ� ����? ex) ModifyAttribute.Deal 
// ( ������ ���� ���迡 ���ϴ� ����Ʈ���� ���� ����. --> HP�� ��ȭ���� �� > { HP ���ظ� �Ծ��� �� , HP ȸ���� ���� �� } )

UDMSEffect_ModAtt::UDMSEffect_ModAtt() :bCreateIfNull(false)
{ 
	EffectTag = TAG_DMS_Effect_ModAttribute;
	
}

bool UDMSEffect_ModAtt::GetTargetAttComp(UDMSEffectInstance* iEI, AActor*& OutTarget, UDMSAttributeComponent*& OutComp)
{
	auto Interface = iEI->GetApplyTarget();
	if (Interface == nullptr)
		return false;
	AActor* tOuter = Cast<AActor>(Interface->GetObject());
	if (tOuter == nullptr)
		return false;

	OutComp = Cast<UDMSAttributeComponent>(tOuter->GetComponentByClass(UDMSAttributeComponent::StaticClass()));
	return true;
}

void UDMSEffect_ModAtt::Work_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnExecuteCompleted& OnWorkCompleted)
{
	// predict�� valid check �� �� �ϰ��� �̷��� �ʿ� �Ѱ� �ٽ� �����غ��� �ؾ����ٵ�...

	DMS_LOG_SIMPLE(TEXT("%s : ModAtt"), *iEI->GetOuter()->GetName());	

	AActor* Outer=nullptr;
	UDMSAttributeComponent* AttComp=nullptr;

	FDMSAttributeModifier Value;
	if (!GenerateModifier(iEI, Value)) {
		OnWorkCompleted.ExecuteIfBound(false); return;
	}

	if (!GetTargetAttComp(iEI, Outer,AttComp))
	{	
		if(!bCreateIfNull)	{
			OnWorkCompleted.ExecuteIfBound(false); return;
		}
		AttComp = Cast<UDMSAttributeComponent>(Outer->AddComponentByClass(UDMSAttributeComponent::StaticClass(),false,FTransform(),false));
	}
	
	if (bCreateIfNull)	AttComp->MakeAttribute(Value.AttributeTag);
	
	float OutValue = 0.0f;
	AttComp->TryModAttribute(Value, OutValue);


	OnWorkCompleted.ExecuteIfBound(true);
}

bool UDMSEffect_ModAtt::Predict_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI)
{
	AActor* Outer = nullptr;
	UDMSAttributeComponent* AttComp;

	FDMSAttributeModifier Value;
	if (!GenerateModifier(iEI, Value)) 	return false;

	if (!GetTargetAttComp(iEI, Outer, AttComp))
	{
		if (Outer == nullptr || !bCreateIfNull) return false;
		AttComp = Cast<UDMSAttributeComponent>(Outer->AddComponentByClass(UDMSAttributeComponent::StaticClass(), false, FTransform(), false));
		AttComp->MakeAttribute(Value.AttributeTag);
	}
	float PredictValue=0.0f;
	bool rv = AttComp->TryModAttribute(Value, PredictValue, false);

	if (rv && bExistFailureCondition)
	{
		switch (FailureConditionOperator)
		{
		case EDMSComparisonOperator::BO_Equal:
			rv = !(rv && PredictValue == FailureConditionValue); break;
		case EDMSComparisonOperator::BO_Greater:
			rv = !(rv && PredictValue > FailureConditionValue); break;
		case EDMSComparisonOperator::BO_Less:
			rv = rv && !(PredictValue < FailureConditionValue); break;
		case EDMSComparisonOperator::BO_GreaterEqual:
			rv = rv && !(PredictValue >= FailureConditionValue); break;
		case EDMSComparisonOperator::BO_LessEqual:
			rv = rv && !(PredictValue <= FailureConditionValue); break;
		default: break;
		}
	}
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
	Rv.AddTag(Value.AttributeTag);
	return Rv;
}


UDMSEffect_ModAtt_Variable::UDMSEffect_ModAtt_Variable()
{
	//SelectorData.ValueSelector = CreateDefaultSubobject<UDMSValueSelector_Attribute>("ValueSelector");
}

bool UDMSEffect_ModAtt_Variable::GenerateModifier(UDMSEffectInstance* EI, FDMSAttributeModifier& OutValue)
{
	auto ValueData = EI->DataSet->GetData(SelectorData.OutDataKey);
	if (ValueData == nullptr || !ValueData->TypeCheck<FDMSAttributeModifier>()) 
		return false;

	else
		OutValue = ValueData->Get<FDMSAttributeModifier>();
	
	return true;
}
