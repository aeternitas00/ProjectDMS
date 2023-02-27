// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Effect/DMSEffectDefinition.h"
#include "DMSEffect_ActivateEffect.generated.h"

// 카드뿐만 아니고 이펙트 사용한 클래스 전반이 사용 가능하게? ( enemy 등등 )
// Usage : IDMSEffectorInterface 를 구현한 Outer가 자신이 가지고 있는 이펙트들 중 하나를 발동 시키도록 함.
// 
UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew, ClassGroup = (Effect), meta = (DisplayName = "Activate Effect Base"))
class UDMSEffect_ActivateEffect : public UDMSEffectDefinition
{
	GENERATED_BODY()

public:
	UDMSEffect_ActivateEffect();
	
	UPROPERTY(EditDefaultsOnly)
	uint8 EffectIdx;

	// 공백이 아닐 경우 EI데이터에서 이 이름의 데이터 값을 먼저 읽고, 이것을 EffectIdx 대신 사용함. 
	// 값이 uint8형이 아니거나, 잘못된 값일 경우 ( OOR 등 ) EffectIdx를 사용.
	UPROPERTY(EditDefaultsOnly)
	FName ReferenceDataName;

	virtual void Work_Implementation(UDMSEffectInstance* iEI) override; // temp
	//virtual FString Literalize_Implementation() override { return Keyword.ToString(); };
};
