// Fill out your copyright notice in the Description page of Project Settings.

#include "EffectSet/DMSEffect_ModAtt.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Effect/DMSEffectInstance.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Effect_ModAttribute, "Effect.ModAttribute");

// 자손임을 표현하기 위해 파생 키워드들은 + ".~~" 하는 형태? ex) ModifyAttribute.Deal 
// ( 일종의 포함 관계에 속하는 이펙트들의 구분 위함. --> HP가 변화했을 때 > { HP 피해를 입었을 때 , HP 회복을 했을 때 } )

UDMSEffect_ModAtt::UDMSEffect_ModAtt() :bCreateIfNull(false)
{ 
	//Keyword = TEXT("ModifyAttribute"); 
	EffectTag = TAG_DMS_Effect_ModAttribute;
	bHasPairedSelector=true;
}

void UDMSEffect_ModAtt::Work_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnWorkCompleted& OnWorkCompleted)
{
	//DMS_LOG_SCREEN(TEXT("%s : ModAtt"), *iEI->GetName());
	
	// predict에 valid check 를 다 하고가니 이런거 필요 한가 다시 생각해보긴 해야할텐데...
	
	AActor* tOuter = Cast<AActor>(iEI->GetApplyTarget()->GetObject());
	if(tOuter== nullptr) 	{
		//DMS_LOG_SCREEN(TEXT("%s : Outer (%s) is not actor"), *iEI->GetName(),*iEI->GetOuter()->GetName());
		OnWorkCompleted.ExecuteIfBound(SourceSequence,false);
		return;
	}

	UDMSAttributeComponent* AttComp = Cast<UDMSAttributeComponent>(tOuter->GetComponentByClass(UDMSAttributeComponent::StaticClass()));
	if (AttComp == nullptr)
	{	
		if(!bCreateIfNull)	{
			OnWorkCompleted.ExecuteIfBound(SourceSequence, false); return;
		}
		AttComp = Cast<UDMSAttributeComponent>(tOuter->AddComponentByClass(UDMSAttributeComponent::StaticClass(),false,FTransform(),false));
	}
	
	if (bCreateIfNull)	AttComp->MakeAttribute(Value.AttributeTag);
	
	//float OutValue=0.0f;
	//bool rv = AttComp->TryModAttribute(Value, OutValue, false);
	//if (rv)	AttComp->TryModAttribute(Value, OutValue);

	float OutValue = 0.0f;
	AttComp->TryModAttribute(Value, OutValue);

	OnWorkCompleted.ExecuteIfBound(SourceSequence, true);
}

bool UDMSEffect_ModAtt::Predict_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI)
{
	auto Interface = iEI->GetApplyTarget();
	if (Interface == nullptr)
		return false;
	AActor* tOuter = Cast<AActor>(Interface->GetObject());
	if (tOuter == nullptr) 
		return false;

	UDMSAttributeComponent* AttComp = Cast<UDMSAttributeComponent>(tOuter->GetComponentByClass(UDMSAttributeComponent::StaticClass()));
	if (AttComp == nullptr && !bCreateIfNull)
		return false;
		
	float PredictValue=0.0f;
	bool rv = AttComp->TryModAttribute(Value, PredictValue, false);

	if (rv && bExistFailureCondition)
	{
		switch (FailureConditionOperator)
		{
		case EDMSComparisonOperator::BO_Equal:
			rv = rv && PredictValue == FailureConditionValue; break;
		case EDMSComparisonOperator::BO_Greater:
			rv = rv && PredictValue > FailureConditionValue; break;
		case EDMSComparisonOperator::BO_Less:
			rv = rv && PredictValue < FailureConditionValue; break;
		case EDMSComparisonOperator::BO_GreaterEqual:
			rv = rv && PredictValue >= FailureConditionValue; break;
		case EDMSComparisonOperator::BO_LessEqual:
			rv = rv && PredictValue <= FailureConditionValue; break;
		default: break;
		}
	}
	return rv;
}

FGameplayTagContainer UDMSEffect_ModAtt::GetEffectTags_Implementation()
{ 
	FGameplayTagContainer Rv(EffectTag);
	Rv.AddTag(Value.AttributeTag);
	return Rv;
}

void UDMSEffect_ModAtt::InitializePairedSelector(UDMSEffectElementSelectorWidget* WidgetInstance)
{
	auto CastedWidget = Cast<UDMSSelector_ModAtt>(WidgetInstance);
	if (CastedWidget==nullptr) return;
	CastedWidget->SelectorData = SelectorData;
}
