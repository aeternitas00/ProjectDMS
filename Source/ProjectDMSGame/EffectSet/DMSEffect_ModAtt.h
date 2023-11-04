// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Effect/DMSEffectDefinition.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Common/DMSValueSelectorDefinition.h"
#include "Selector/DMSConfirmWidgetBase.h"
#include "DMSEffect_ModAtt.generated.h"

class UDMSConfirmWidgetBase;

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Effect_ModAttribute)

UCLASS(NotBlueprintable, DefaultToInstanced, EditInlineNew, ClassGroup = (Effect), meta = (DisplayName = "Mod Attribute Effect Base"))
class PROJECTDMSGAME_API UDMSEffect_ModAtt: public UDMSEffectDefinition
{
	GENERATED_BODY()

public:
	// 자손임을 표현하기 위해 파생 키워드들은 + ".~~" 하는 형태? ex) ModifyAttribute.Deal 
	// ( 일종의 포함 관계에 속하는 이펙트들의 구분 위함. --> HP가 변화했을 때 > { HP 피해를 입었을 때 , HP 회복을 했을 때 } )
	UDMSEffect_ModAtt();

	/**
	 *	Create attribute if there isn't matching one.
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect, meta = (DisplayName = "Create If Null"))
	bool bCreateIfNull;


	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect, meta = (DisplayName = "Exist failure condition"))
	bool bExistFailureCondition;

	/**
	 * Checking operator.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Effect, meta = (EditCondition = "bExistFailureCondition", EditConditionHides))
	EDMSComparisonOperator FailureConditionOperator;

	/**
	 * Operating value.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Effect, meta = (EditCondition = "bExistFailureCondition", EditConditionHides))
	float FailureConditionValue; // float? 

	bool GetTargetAttComp(UDMSEffectInstance* iEI, AActor*& OutTarget, UDMSAttributeComponent*& OutComp);
	virtual bool GenerateModifier(UDMSEffectInstance* EI, FDMSAttributeModifier& OutValue){return false;}
	virtual void Work_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnExecuteCompleted& OnWorkCompleted) override; // temp
	virtual bool Predict_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI) override;
	// ====== Selectors ====== //

	virtual FGameplayTagContainer GetEffectTags_Implementation() override;
};

UCLASS(Blueprintable, ClassGroup = (Effect), meta = (DisplayName = "Mod Attribute Effect : Static"))
class PROJECTDMSGAME_API UDMSEffect_ModAtt_Static : public UDMSEffect_ModAtt
{
	GENERATED_BODY()

public:
	//UDMSEffect_ModAtt_Static();

	/**
	 *	Effect's modifying value.
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect, meta = (DisplayName = "Static Value", EditCondition = "!bIsUsingSelector", EditConditionHides))
	FDMSAttributeModifier Value;

	virtual FGameplayTagContainer GetEffectTags_Implementation() override;
	virtual bool GenerateModifier(UDMSEffectInstance* EI, FDMSAttributeModifier& OutValue){OutValue = Value; return true;}

};

UCLASS(Blueprintable, ClassGroup = (Effect), meta = (DisplayName = "Mod Attribute Effect : Variable"))
class PROJECTDMSGAME_API UDMSEffect_ModAtt_Variable : public UDMSEffect_ModAtt
{
	GENERATED_BODY()

public:
	UDMSEffect_ModAtt_Variable();

	/**
	 * Effect's modifying value selector.
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect, meta = (EditCondition = "bIsUsingSelector", EditConditionHides))
	FDMSValueSelectionForm SelectorData;

	
	virtual bool GenerateModifier(UDMSEffectInstance* EI, FDMSAttributeModifier& OutValue);

};
//
//UCLASS()
//class PROJECTDMSGAME_API UDMSValueSelector_Attribute : public UDMSValueSelectorDefinition
//{
//	GENERATED_BODY()
//
//public:
//	//UDMSValueSelector_Attribute(){};
//
//	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "CandidatesFlag != 0", EditConditionHides))
//	bool isRanged;
//
//	/*
//	 *	Modifier's Value will be ignored. ( For attribute's name and operator )
//	 */
//	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = ModAttribute, meta = (EditCondition = "bIsRanged"))
//	FDMSAttributeModifier Modifier;
//
//	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = ModAttribute, meta = (EditCondition = "bIsRanged"))
//	float MaxValue;
//
//	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = ModAttribute, meta = (EditCondition = "bIsRanged"))
//	float MinValue;
//
//	/**
//	 *	For list based selector.
//	 */
//	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = ModAttribute, meta = (EditCondition = "!bIsRanged"))
//	TArray<FDMSAttributeModifier> ModifierArray;
//
//
//	UPROPERTY(EditDefaultsOnly)
//	TSubclassOf<UDMSConfirmWidgetBase_AttModifier> WidgetClass;
//};
//
//UCLASS()
//class PROJECTDMSGAME_API UDMSConfirmWidgetBase_AttModifier : public UDMSConfirmWidgetBase
//{
//	GENERATED_BODY()
//
//public:
//	UFUNCTION(BlueprintCallable)
//	void UpdateData(UDMSDataObjectSet* UpdatingData, FDMSAttributeModifier Value) { UpdatingData->SetData(SelectionForm.OutDataKey, Value); }
//};