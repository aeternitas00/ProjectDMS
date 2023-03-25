// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Effect/DMSEffectDefinition.h"

#include "DMSEffect_ActivateEffect.generated.h"

// 카드뿐만 아니고 이펙트 사용한 클래스 전반이 사용 가능하게? ( enemy 등등 )
// Usage : IDMSEffectorInterface 를 구현한 Outer가 자신이 가지고 있는 이펙트들 중 하나를 발동 시키도록 함.
// 

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Effect_ActivateEffect)

UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew, ClassGroup = (Effect), meta = (DisplayName = "Activate Effect Base"))
class UDMSEffect_ActivateEffect : public UDMSEffectDefinition
{
	GENERATED_BODY()

public:
	UDMSEffect_ActivateEffect();
	
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "!bIsUsingSelector", EditConditionHides))
	uint8 EffectIdx;

	UPROPERTY(EditDefaultsOnly)
	FName EffectSetName;

	UDMSEffectSet* GetEffectSetFromOuter(UDMSEffectInstance* iEI);

	virtual void Work_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnWorkCompleted& OnWorkCompleted) override;
	virtual bool GetCandidates_Implementation(UDMSSequence* iSeq, TArray<UDMSDataObject*>& outDataObj);

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect, meta = (EditCondition = "bIsUsingSelector", EditConditionHides))
	TSubclassOf<UDMSSelector_ActivateEffect> PairedWidgetClass;

	virtual TSubclassOf<UDMSEffectElementSelectorWidget> GetPairedSelector_Implementation() { return PairedWidgetClass.Get(); }

	virtual void InitializePairedSelector(UDMSEffectElementSelectorWidget* WidgetInstance) override;

};

UCLASS(Blueprintable, Abstract)
class UDMSSelector_ActivateEffect : public UDMSEffectElementSelectorWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag OutDataName;

	virtual	bool SetupWidget_Implementation() { return GetCandidatesFromED(); }
};