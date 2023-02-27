// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Effect/DMSEffectDefinition.h"
#include "DMSEffect_ActivateEffect.generated.h"

// ī��Ӹ� �ƴϰ� ����Ʈ ����� Ŭ���� ������ ��� �����ϰ�? ( enemy ��� )
// Usage : IDMSEffectorInterface �� ������ Outer�� �ڽ��� ������ �ִ� ����Ʈ�� �� �ϳ��� �ߵ� ��Ű���� ��.
// 
UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew, ClassGroup = (Effect), meta = (DisplayName = "Activate Effect Base"))
class UDMSEffect_ActivateEffect : public UDMSEffectDefinition
{
	GENERATED_BODY()

public:
	UDMSEffect_ActivateEffect();
	
	UPROPERTY(EditDefaultsOnly)
	uint8 EffectIdx;

	// ������ �ƴ� ��� EI�����Ϳ��� �� �̸��� ������ ���� ���� �а�, �̰��� EffectIdx ��� �����. 
	// ���� uint8���� �ƴϰų�, �߸��� ���� ��� ( OOR �� ) EffectIdx�� ���.
	UPROPERTY(EditDefaultsOnly)
	FName ReferenceDataName;

	virtual void Work_Implementation(UDMSEffectInstance* iEI) override; // temp
	//virtual FString Literalize_Implementation() override { return Keyword.ToString(); };
};
