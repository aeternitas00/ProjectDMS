// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Effect/DMSEffectDefinition.h"
#include "Attribute/DMSAttributeComponent.h"
#include "DMSEffect_ModAtt.generated.h"

// BP에서 기본적인 것들 외의 커스텀 이펙트 원형 생성 가능하게
// Ex) BP_EffectInstance_DealDamage ?
UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew, ClassGroup = (Effect), meta = (DisplayName = "Mod Attribute Effect Base"))
class UDMSEffect_ModAtt: public UDMSEffectDefinition
{
	GENERATED_BODY()

public:
	// 자손임을 표현하기 위해 파생 키워드들은 + ".~~" 하는 형태? ex) ModifyAttribute.Deal 
	// ( 일종의 포함 관계에 속하는 이펙트들의 구분 위함. --> HP가 변화했을 때 > { HP 피해를 입었을 때 , HP 회복을 했을 때 } )
	UDMSEffect_ModAtt() { Keyword = TEXT("ModifyAttribute"); };

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, Category = Effect, meta = (DisplayName = "Modifying Value"))
	FDMSAttributeModifier Value;
	
	virtual void Work_Implementation(UDMSEffectInstance* iEI) override; // temp
	//virtual FString Literalize_Implementation() override { return Keyword.ToString(); };
};
