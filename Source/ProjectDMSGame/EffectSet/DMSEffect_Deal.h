// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EffectSet/DMSEffect_ModAtt.h"
#include "DMSEffect_Deal.generated.h"

// BP에서 기본적인 것들 외의 커스텀 이펙트 원형 생성 가능하게
// Ex) BP_EffectInstance_DealDamage ?
UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew, ClassGroup = (Effect), meta = (DisplayName = "Deal Effect Base"))
class UDMSEffect_Deal : public UDMSEffect_ModAtt
{
	GENERATED_BODY()


public:
	UDMSEffect_Deal(){ Keyword = TEXT("ModifyAttribute.Deal"); Value.ModifierType = EDMSModifierType::MT_Sub; };
	
	virtual void Work_Implementation(UDMSEffectInstance* iEI) override; // temp
};
