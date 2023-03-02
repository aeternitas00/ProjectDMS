// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Effect/DMSEffectDefinition.h"
#include "Attribute/DMSAttributeComponent.h"
#include "DMSEffect_ActivateCard.generated.h"

// Deprecated
UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew, ClassGroup = (Effect), meta = (DisplayName = "Activate Card Base (Dep?)"))
class UDMSEffect_ActivateCard : public UDMSEffectDefinition
{
	GENERATED_BODY()

public:
	// �ڼ����� ǥ���ϱ� ���� �Ļ� Ű������� + ".~~" �ϴ� ����? ex) ModifyAttribute.Deal 
	// ( ������ ���� ���迡 ���ϴ� ����Ʈ���� ���� ����. --> HP�� ��ȭ���� �� > { HP ���ظ� �Ծ��� �� , HP ȸ���� ���� �� } )
	UDMSEffect_ActivateCard();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly);
	FName SetName;

	virtual void Work_Implementation(UDMSEffectInstance* iEI) override; // temp
	//virtual FString Literalize_Implementation() override { return Keyword.ToString(); };
};
