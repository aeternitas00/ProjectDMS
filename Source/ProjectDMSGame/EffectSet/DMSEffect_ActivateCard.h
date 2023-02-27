// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Effect/DMSEffectDefinition.h"
#include "Attribute/DMSAttributeComponent.h"
#include "DMSEffect_ActivateCard.generated.h"

// 카드뿐만 아니고 이펙트 사용한 클래스 전반이 사용 가능하게? ( enemy 등등 )
// ==> '이펙트 셋'을 보유 할수 있는 을 의미하는 인터페이스를 구현??
// 

// Deprecated
UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew, ClassGroup = (Effect), meta = (DisplayName = "Activate Card Base (Dep?)"))
class UDMSEffect_ActivateCard : public UDMSEffectDefinition
{
	GENERATED_BODY()

public:
	// 자손임을 표현하기 위해 파생 키워드들은 + ".~~" 하는 형태? ex) ModifyAttribute.Deal 
	// ( 일종의 포함 관계에 속하는 이펙트들의 구분 위함. --> HP가 변화했을 때 > { HP 피해를 입었을 때 , HP 회복을 했을 때 } )
	UDMSEffect_ActivateCard();
	
	virtual void Work_Implementation(UDMSEffectInstance* iEI) override; // temp
	//virtual FString Literalize_Implementation() override { return Keyword.ToString(); };
};
