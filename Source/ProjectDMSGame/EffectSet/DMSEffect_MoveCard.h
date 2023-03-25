// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Effect/DMSEffectDefinition.h"
#include "DMSEffect_MoveCard.generated.h"

class ADMSCardBase;
class UDMSCardContainerComponent;
/**
 * 
 * 카드 이펙트처럼 인스턴스드 일수도 있고 필요할 때 생성해서 쓸수 있게도 할 예정 ( 플레이어 조작에 의한 행동 )
 * 
 */

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Effect_MoveCard)

UCLASS(Blueprintable , ClassGroup = (Effect), meta = (DisplayName = "Move Card Effect Base"))
class PROJECTDMSGAME_API UDMSEffect_MoveCard : public UDMSEffectDefinition
{
	GENERATED_BODY()
	
public:
	UDMSEffect_MoveCard();;

	// Keyword for search
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect, meta = (DisplayName = "Name of Destination Container"))
	FName NameDestination;

	virtual void Work_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnWorkCompleted& OnWorkCompleted) override;
};
