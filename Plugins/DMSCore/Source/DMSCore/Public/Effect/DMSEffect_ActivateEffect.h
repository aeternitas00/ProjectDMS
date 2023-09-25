// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Effect/DMSEffectDefinition.h"

#include "DMSEffect_ActivateEffect.generated.h"

//
// Usage : IDMSEffectorInterface 를 구현한 Outer가 자신이 가지고 있는 이펙트들 중 하나 혹은 저장된 특정한 이펙트를 발동 시키도록 함.
// AE로 발동되는 새로운 Sequence는 이 이펙트를 실행하는 EI의 Outer가 SourceObject이고 SourceObject의 오너가 SourcePlayer인 새로운 시퀀스로 진행됨.
// 

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Effect_ActivateEffect)

UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew, ClassGroup = (Effect), meta = (DisplayName = "Activate Effect Base"))
class DMSCORE_API UDMSEffect_ActivateEffect : public UDMSEffectDefinition
{
	GENERATED_BODY()

public:
	UDMSEffect_ActivateEffect();
	
	UPROPERTY(EditDefaultsOnly)
	bool UseEffectFromOuter;

	// EffectNode that will activate itself when it doesn't use Outer's one.
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "!UseEffectFromOuter", EditConditionHides))
	TObjectPtr<UDMSEffectNodeWrapper> StaticEffect;

	// == Sort of Hard coded Searching == //
	// == Use it when you know exactly what it'll be attached to. == //
	
	// Tag of the effect set to reference.
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "UseEffectFromOuter", EditConditionHides))
	FGameplayTag EffectSetName;

	// Index in EffectSet of EffectNode to be activated.
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "UseEffectFromOuter&&!bIsUsingSelector", EditConditionHides))
	uint8 EffectIdx;

	UDMSEffectSet* GetEffectSetFromOuter(UDMSEffectInstance* iEI);

	virtual void Work_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnExecuteCompleted& OnWorkCompleted) override;
	//virtual bool GetCandidates_Implementation(UDMSSequence* iSeq, TArray<UDMSDataObject*>& outDataObj);

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect, meta = (EditCondition = "bIsUsingSelector", EditConditionHides))
	TSubclassOf<UDMSSelector_ActivateEffect> PairedWidgetClass;

	virtual TSubclassOf<UDMSEffectElementSelectorWidget> GetPairedSelector_Implementation() { return PairedWidgetClass.Get(); }

	virtual void InitializePairedSelector(UDMSEffectElementSelectorWidget* WidgetInstance) override;

};

UCLASS(Blueprintable, Abstract)
class DMSCORE_API UDMSSelector_ActivateEffect : public UDMSEffectElementSelectorWidget
{
	GENERATED_BODY()

public:

	virtual UDMSDataObjectSet* MakeOutputData_Implementation();

};