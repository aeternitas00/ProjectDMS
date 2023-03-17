// Fill out your copyright notice in the Description page of Project Settings.

#include "EffectSet/DMSEffect_ModAtt.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Effect/DMSEffectInstance.h"



// 자손임을 표현하기 위해 파생 키워드들은 + ".~~" 하는 형태? ex) ModifyAttribute.Deal 
// ( 일종의 포함 관계에 속하는 이펙트들의 구분 위함. --> HP가 변화했을 때 > { HP 피해를 입었을 때 , HP 회복을 했을 때 } )

UDMSEffect_ModAtt::UDMSEffect_ModAtt() :bCreateIfNull(false)
{ 
	Keyword = TEXT("ModifyAttribute"); 
	bHasPairedSelector=true;
	//PairedSelector=UDMSSelector_ModAtt::StaticClass();
}

void UDMSEffect_ModAtt::Work_Implementation(UDMSEffectInstance* iEI)
{
	DMS_LOG_SCREEN(TEXT("%s : ModAtt"), *iEI->GetName());

	//IDMSAttributeInterface* AttInterface = Cast<IDMSAttributeInterface>(iEI->GetOuter());

	//if (AttInterface==nullptr) {
	//	DMS_LOG_SCREEN(TEXT("%s : Outer (%s) doesn't implements AttributeInterface"), *iEI->GetName(),*iEI->GetOuter()->GetName());
	//	//if (bCreateIfNull) {

	//	//}
	//	return;
	//}
	
	AActor* tOuter = iEI->GetTypedOuter<AActor>();
	if(tOuter== nullptr) 	{
		DMS_LOG_SCREEN(TEXT("%s : Outer (%s) is not actor"), *iEI->GetName(),*iEI->GetOuter()->GetName());
		return;
	}

	UDMSAttributeComponent* AttComp = Cast<UDMSAttributeComponent>(tOuter->GetComponentByClass(UDMSAttributeComponent::StaticClass()));
	if (AttComp == nullptr)
	{	
		if(!bCreateIfNull)	return;
		AttComp = Cast<UDMSAttributeComponent>(tOuter->AddComponentByClass(UDMSAttributeComponent::StaticClass(),false,FTransform(),false));
	}
	
	if (bCreateIfNull)	AttComp->MakeAttribute(Value.AttributeName);
	AttComp->TryModAttribute(Value);
}

void UDMSEffect_ModAtt::InitializePairedSelector(UDMSEffectElementSelectorWidget* WidgetInstance)
{
	auto CastedWidget = Cast<UDMSSelector_ModAtt>(WidgetInstance);
	if (CastedWidget==nullptr) return;
	CastedWidget->SelectorData = SelectorData;
}
