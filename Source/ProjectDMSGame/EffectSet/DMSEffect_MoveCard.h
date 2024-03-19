// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Effect/DMSEffectDefinition.h"
#include "DMSEffect_MoveCard.generated.h"

class ADMSCardBase;
class UDMSCardContainerComponent;

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Effect_MoveCard)

UCLASS(Blueprintable , ClassGroup = (Effect), meta = (DisplayName = "Move Card Effect Base"))
class PROJECTDMSGAME_API UDMSEffect_MoveCard : public UDMSEffectDefinition
{
	GENERATED_BODY()
	
public:
	UDMSEffect_MoveCard();;

	/**
	 * Keyword for search
	 */ 
	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect, meta = (DisplayName = "Name of Destination Container"))
	//FName NameDestination;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect, meta = (DisplayName = "Tag of Destination Container"))
	FGameplayTag TagDestination;

	virtual void Work_Implementation(UDMSSequence* SourceSequence, ADMSActiveEffect* iEI, const FOnExecuteCompleted& OnWorkCompleted) override;
};
