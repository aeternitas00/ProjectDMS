// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Effect/DMSEffectDefinition.h"
#include "DMSEffect_AttachPersistent.generated.h"


class UDMSEffect_AttachPersistent;
class UDMSSelector_AttachPersistent;


UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Effect_AttachPersistent)


/**
 * 
 */
UCLASS()
class PROJECTDMSGAME_API UDMSEffect_AttachPersistent : public UDMSEffectDefinition
{
	GENERATED_BODY()
	
public:
	UDMSEffect_AttachPersistent();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect, meta = (DisplayName = "EI's Effect & Contidition"))
	TArray<UDMSEffectNodeWrapper*> EIEffects;

	virtual void Work_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnWorkCompleted& OnWorkCompleted) override; // temp




	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect, meta = (EditCondition = "bIsUsingSelector", EditConditionHides))
	TSubclassOf<UDMSSelector_AttachPersistent> PairedWidgetClass;

	virtual TSubclassOf<UDMSEffectElementSelectorWidget> GetPairedSelector_Implementation() { return PairedWidgetClass.Get(); }

	// virtual FGameplayTagContainer GetEffectTags_Implementation() override;
	
	virtual void InitializePairedSelector(UDMSEffectElementSelectorWidget* WidgetInstance) override;
};



UCLASS(Blueprintable, Abstract)
class UDMSSelector_AttachPersistent : public UDMSEffectElementSelectorWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	uint8 Num;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<uint8> SelectedIndex;

public:

	virtual UDMSDataObjectSet* MakeOutputData_Implementation();

};