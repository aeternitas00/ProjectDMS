// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Effect/DMSEffectDefinition.h"
#include "Attribute/DMSAttributeComponent.h"
#include "DMSEffect_ModAtt.generated.h"

// BP���� �⺻���� �͵� ���� Ŀ���� ����Ʈ ���� ���� �����ϰ�
// Ex) BP_EffectInstance_DealDamage ?
UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew, ClassGroup = (Effect), meta = (DisplayName = "Mod Attribute Effect Base"))
class UDMSEffect_ModAtt: public UDMSEffectDefinition
{
	GENERATED_BODY()

public:
	// �ڼ����� ǥ���ϱ� ���� �Ļ� Ű������� + ".~~" �ϴ� ����? ex) ModifyAttribute.Deal 
	// ( ������ ���� ���迡 ���ϴ� ����Ʈ���� ���� ����. --> HP�� ��ȭ���� �� > { HP ���ظ� �Ծ��� �� , HP ȸ���� ���� �� } )
	UDMSEffect_ModAtt() { Keyword = TEXT("ModifyAttribute"); };

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, Category = Effect, meta = (DisplayName = "Modifying Value"))
	FDMSAttributeModifier Value;
	
	virtual void Work_Implementation(UDMSEffectInstance* iEI) override; // temp
	//virtual FString Literalize_Implementation() override { return Keyword.ToString(); };
};
