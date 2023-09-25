// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Effect/DMSEffectDefinition.h"

#include "DMSEffect_ActivateEffect.generated.h"

//
// Usage : IDMSEffectorInterface �� ������ Outer�� �ڽ��� ������ �ִ� ����Ʈ�� �� �ϳ� Ȥ�� ����� Ư���� ����Ʈ�� �ߵ� ��Ű���� ��.
// AE�� �ߵ��Ǵ� ���ο� Sequence�� �� ����Ʈ�� �����ϴ� EI�� Outer�� SourceObject�̰� SourceObject�� ���ʰ� SourcePlayer�� ���ο� �������� �����.
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