// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Effect/DMSEffectDefinition.h"
#include "DMSEffect_MoveCard.generated.h"

class ADMSCardBase;
class UDMSCardContainerComponent;

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Effect_MoveCard)

UCLASS(Blueprintable , ClassGroup = (Effect), meta = (DisplayName = "ED Move Spawnable to container"))
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

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced, Category = Effect, meta = (DisplayName = "Owner of Container ( Use first of found )"))
	TObjectPtr<UDMSTargetGenerator> ContainerOwner;

	virtual void Work_Implementation(ADMSSequence* SourceSequence, ADMSActiveEffect* iEI, const FOnExecuteCompleted& OnWorkCompleted) override;
	virtual bool Predict_Implementation(ADMSSequence* SourceSequence, ADMSActiveEffect* iEI) override;
};
