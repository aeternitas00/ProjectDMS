// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Effect/DMSEffectDefinition.h"
#include "DMSEffect_MoveCard.generated.h"

class ADMSCardBase;
class UDMSCardContainerComponent;
/**
 * 
 * ī�� ����Ʈó�� �ν��Ͻ��� �ϼ��� �ְ� �ʿ��� �� �����ؼ� ���� �ְԵ� �� ���� ( �÷��̾� ���ۿ� ���� �ൿ )
 * 
 */
UCLASS(Blueprintable , ClassGroup = (Effect), meta = (DisplayName = "Move Card Effect Base"))
class PROJECTDMSGAME_API UDMSEffect_MoveCard : public UDMSEffectDefinition
{
	GENERATED_BODY()
	
public:
	UDMSEffect_MoveCard() { Keyword = TEXT("MoveCard"); };

	// Keyword for search
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect, meta = (DisplayName = "Name of Destination Container"))
	FName NameDestination;

	virtual void Work_Implementation(UDMSEffectInstance* iEI) override;
};
