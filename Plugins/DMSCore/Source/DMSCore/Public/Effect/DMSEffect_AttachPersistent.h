// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DMSCoreIncludes.h"
#include "Effect/DMSEffectDefinition.h"
#include "DMSEffect_AttachPersistent.generated.h"


class UDMSEffect_AttachPersistent;
class UDMSSelector_AttachPersistent;


UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Effect_AttachPersistent)


/**
 * 
 */
UCLASS()
class DMSCORE_API UDMSEffect_AttachPersistent : public UDMSEffectDefinition
{
	GENERATED_BODY()
	
public:
	UDMSEffect_AttachPersistent();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect)
	bool bActivateImmediately;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect)
	TArray<TObjectPtr<UDMSEffectNodeWrapper>> AttachingEffects;

	virtual void Work_Implementation(UDMSSequence* SourceSequence, ADMSActiveEffect* iEI, const FOnExecuteCompleted& OnWorkCompleted) override; // temp;
};

