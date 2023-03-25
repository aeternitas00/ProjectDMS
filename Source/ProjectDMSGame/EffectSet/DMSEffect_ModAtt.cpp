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
	DMS_LOG_SCREEN(TEXT("%s : ModAtt"), *iEI->GetName());
	
	AActor* tOuter = iEI->GetTypedOuter<AActor>();
	if(tOuter== nullptr) 	{
		DMS_LOG_SCREEN(TEXT("%s : Outer (%s) is not actor"), *iEI->GetName(),*iEI->GetOuter()->GetName());
		OnWorkCompleted.ExecuteIfBound(SourceSequence);
		return;
	}

	UDMSAttributeComponent* AttComp = Cast<UDMSAttributeComponent>(tOuter->GetComponentByClass(UDMSAttributeComponent::StaticClass()));
	if (AttComp == nullptr)
	{	
		if(!bCreateIfNull)	{
			OnWorkCompleted.ExecuteIfBound(SourceSequence); return;
		}
		AttComp = Cast<UDMSAttributeComponent>(tOuter->AddComponentByClass(UDMSAttributeComponent::StaticClass(),false,FTransform(),false));
	}
	
	if (bCreateIfNull)	AttComp->MakeAttribute(Value.AttributeTag);
	AttComp->TryModAttribute(Value);

	OnWorkCompleted.ExecuteIfBound(SourceSequence);
}

void UDMSEffect_ModAtt::InitializePairedSelector(UDMSEffectElementSelectorWidget* WidgetInstance)
{
	auto CastedWidget = Cast<UDMSSelector_ModAtt>(WidgetInstance);
	if (CastedWidget==nullptr) return;
	CastedWidget->SelectorData = SelectorData;
}
