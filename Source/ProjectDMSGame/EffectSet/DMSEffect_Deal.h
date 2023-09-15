// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EffectSet/DMSEffect_ModAtt.h"
#include "DMSEffect_Deal.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Effect_DealDamage)
// BP���� �⺻���� �͵� ���� Ŀ���� ����Ʈ ���� ���� �����ϰ�
// Ex) BP_EffectInstance_DealDamage ?
UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew, ClassGroup = (Effect), meta = (DisplayName = "Deal Effect Base"))
class UDMSEffect_Deal : public UDMSEffect_ModAtt
{
	GENERATED_BODY()


public:
	UDMSEffect_Deal();;
	
	//virtual void Work_Implementation(UDMSEffectInstance* iEI,  const FOnExecuteCompleted& OnWorkCompleted) override; // temp
};
