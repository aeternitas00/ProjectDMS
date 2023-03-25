// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Effect/DMSEffectDefinition.h"
#include "Attribute/DMSAttributeComponent.h"
#include "DMSEffect_ModAtt.generated.h"


UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Effect_ModAttribute)

// BP���� �⺻���� �͵� ���� Ŀ���� ����Ʈ ���� ���� �����ϰ�
// Ex) BP_EffectInstance_DealDamage ?
USTRUCT(BlueprintType)
struct FDMSSelectorData_ModAtt
{
	GENERATED_BODY()

		/**
		 *	True = Value is ranged. (ex. {"Add", "HP", [3 ~ 6]} )
		 *	False = Give Modifier list to player (ex. Choose one of [ {"Add", "HP", 3 } , {"Add", "HP", 6 } ]
		 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = ModAttribute)
	bool bIsRanged;

	/*
	 *	Modifier's Value will be ignored. ( For attribute's name and operator )
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect, meta = (EditCondition = "bIsRanged"))
	FDMSAttributeModifier Modifier;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = ModAttribute, meta = (EditCondition = "bIsRanged"))
	float MaxValue;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = ModAttribute, meta = (EditCondition = "bIsRanged"))
	float MinValue;

	/**
	 *	For list based selector.
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = ModAttribute, meta = (EditCondition = "!bIsRanged"))
	TArray<FDMSAttributeModifier> ModifierArray;
};

UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew, ClassGroup = (Effect), meta = (DisplayName = "Mod Attribute Effect Base"))
class UDMSEffect_ModAtt: public UDMSEffectDefinition
{
	GENERATED_BODY()

public:
	// �ڼ����� ǥ���ϱ� ���� �Ļ� Ű������� + ".~~" �ϴ� ����? ex) ModifyAttribute.Deal 
	// ( ������ ���� ���迡 ���ϴ� ����Ʈ���� ���� ����. --> HP�� ��ȭ���� �� > { HP ���ظ� �Ծ��� �� , HP ȸ���� ���� �� } )
	UDMSEffect_ModAtt();

	/**
	 *	Effect's modifying value.
	 */
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, Category = Effect, meta = (DisplayName = "Modifying Value", EditCondition = "!bIsUsingSelector", EditConditionHides))
	FDMSAttributeModifier Value;
	
	/**
	 *	Create attribute if there isn't matching one.
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect, meta = (DisplayName = "Create If Null"))
	bool bCreateIfNull;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect, meta = (EditCondition = "bIsUsingSelector", EditConditionHides))
	FDMSSelectorData_ModAtt SelectorData;

	virtual void Work_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnWorkCompleted& OnWorkCompleted) override; // temp

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect, meta = (EditCondition = "bIsUsingSelector", EditConditionHides))
	TSubclassOf<UDMSSelector_ModAtt> PairedWidgetClass;

	virtual TSubclassOf<UDMSEffectElementSelectorWidget> GetPairedSelector_Implementation() { return PairedWidgetClass.Get(); }

	virtual void InitializePairedSelector(UDMSEffectElementSelectorWidget* WidgetInstance) override;
};

UCLASS(Blueprintable, Abstract)
class UDMSSelector_ModAtt : public UDMSEffectElementSelectorWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FDMSSelectorData_ModAtt SelectorData;
};