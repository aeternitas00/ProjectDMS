// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Effect/DMSEffectDefinition.h"
#include "Selector/DMSDecisionDefinition.h"
#include "DMSEffect_ActivateEffect.generated.h"

//
// Usage : IDMSEffectorInterface 를 구현한 Outer가 자신이 가지고 있는 이펙트들 중 하나 혹은 저장된 특정한 이펙트를 발동 시키도록 함.
// AE로 발동되는 새로운 Sequence는 이 이펙트를 실행하는 EI의 Outer가 SourceObject이고 SourceObject의 오너가 SourcePlayer인 새로운 시퀀스로 진행됨.
// 
UCLASS(NotBlueprintable, DefaultToInstanced, EditInlineNew, ClassGroup = (Effect), meta = (DisplayName = "Activate Effect Base"))
class DMSCORE_API UDMSEffect_ActivateEffect : public UDMSEffectDefinition
{
	GENERATED_BODY()

public:
	UDMSEffect_ActivateEffect();

	virtual bool GetEffectNodeWrapper(UDMSEffectInstance* iEI, UDMSEffectNodeWrapper*& OutWrapper){return false;}
	virtual bool GetEffectNodeWrappers(UDMSEffectInstance* iEI, TArray<UDMSEffectNodeWrapper*>& OutWrapperArr){return false;}

	virtual void Work_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnExecuteCompleted& OnWorkCompleted) override;

};


UCLASS(Blueprintable, ClassGroup = (Effect), meta = (DisplayName = "Activate Effect : Static"))
class DMSCORE_API UDMSEffect_ActivateEffect_Static : public UDMSEffect_ActivateEffect
{
	GENERATED_BODY()

public:
	UDMSEffect_ActivateEffect_Static();

	// == Sort of Hard coded Searching == //
	// == Use it when you know exactly what it'll be attached to. == //
	UPROPERTY(EditDefaultsOnly)
	bool UseEffectFromOuter;

	// Tag of the effect set to reference.
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "UseEffectFromOuter", EditConditionHides))
	FGameplayTag EffectSetName;

	// Index in EffectSet of EffectNode to be activated.
	//UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "UseEffectFromOuter", EditConditionHides))
	//uint8 EffectIdx;

	// Index in EffectSet of EffectNode to be activated.
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "UseEffectFromOuter", EditConditionHides))
	TArray<uint8> EffectIdxArr;

	// EffectNode that will activate itself when it doesn't use Outer's one.
	//UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "!UseEffectFromOuter", EditConditionHides))
	//TObjectPtr<UDMSEffectNodeWrapper> StaticEffect;


	// EffectNode that will activate itself when it doesn't use Outer's one.
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "!UseEffectFromOuter", EditConditionHides))
	TArray<TObjectPtr<UDMSEffectNodeWrapper>> StaticEffectArr;

	UDMSEffectSet* GetEffectSetFromOuter(UDMSEffectInstance* iEI);

	//virtual bool GetEffectNodeWrapper(UDMSEffectInstance* iEI, UDMSEffectNodeWrapper*& OutWrapper);
	virtual bool GetEffectNodeWrappers(UDMSEffectInstance* iEI, TArray<UDMSEffectNodeWrapper*>& OutWrapperArr);
};


UCLASS(Blueprintable, ClassGroup = (Effect), meta = (DisplayName = "Activate Effect : Variable"))
class DMSCORE_API UDMSEffect_ActivateEffect_Variable : public UDMSEffect_ActivateEffect
{
	GENERATED_BODY()

public:
	UDMSEffect_ActivateEffect_Variable();

	/**
	 *
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect, meta = (EditCondition = "!UseEffectFromOuter", EditConditionHides))
	FDMSValueSelectionForm SelectorData;

	//virtual bool GetEffectNodeWrapper(UDMSEffectInstance* iEI, UDMSEffectNodeWrapper*& OutWrapper);
	virtual bool GetEffectNodeWrappers(UDMSEffectInstance* iEI, TArray<UDMSEffectNodeWrapper*>& OutWrapperArr);


};

UCLASS()
class DMSCORE_API UDMSSelectorRequestGenerator_AE : public UDMSSelectorRequestGenerator
{
	GENERATED_BODY()

public:
	// == Use it when you know exactly what it'll be attached to. == //
	UPROPERTY(EditDefaultsOnly)
	bool UseEffectFromOuter;

	// Tag of the effect set to reference.
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "UseEffectFromOuter", EditConditionHides))
	FGameplayTag EffectSetTag;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "!UseEffectFromOuter", EditConditionHides))
	TArray<TObjectPtr<UDMSEffectNodeWrapper>> StaticEffects;

	virtual TArray<UDMSDataObject*> GenerateCandidates(UDMSSequence* Sequence, UDMSEffectInstance* TargetEI);
	TArray<UDMSDataObject*> MakeDataArray(UDMSEffectInstance* TargetEI);
};