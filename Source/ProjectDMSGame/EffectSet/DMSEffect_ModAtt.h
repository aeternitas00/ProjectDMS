// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Effect/DMSEffectDefinition.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Common/DMSValueSelectorDefinition.h"
#include "DMSEffect_ModAtt.generated.h"


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

	// Attribute Modifier generating rules

	bool GetTargetAttComp(ADMSActiveEffect* iEI, AActor*& OutTarget, UDMSAttributeComponent*& OutComp);
	
	UFUNCTION(BlueprintNativeEvent)
	bool GenerateModifier(ADMSActiveEffect* EI, UPARAM(Ref) FDMSAttributeModifier& OutModifier);
	virtual bool GenerateModifier_Implementation(ADMSActiveEffect* EI,FDMSAttributeModifier& OutModifier){return false;}
	
	virtual void Work_Implementation(UDMSSequence* SourceSequence, ADMSActiveEffect* iEI, const FOnExecuteCompleted& OnWorkCompleted) override; // temp
	virtual bool Predict_Implementation(UDMSSequence* SourceSequence, ADMSActiveEffect* iEI) override;
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
	FDMSAttributeModifier StaticModifier;


	virtual FGameplayTagContainer GetEffectTags_Implementation() override;
	virtual bool GenerateModifier_Implementation(ADMSActiveEffect* EI,FDMSAttributeModifier& OutModifier){ OutModifier=StaticModifier; return true;}

};

UCLASS(Blueprintable, ClassGroup = (Effect), meta = (DisplayName = "Mod Attribute Effect : From AE Data"))
class PROJECTDMSGAME_API UDMSEffect_ModAtt_Variable : public UDMSEffect_ModAtt
{
	GENERATED_BODY()

public:
	UDMSEffect_ModAtt_Variable();

	/**
	 * Effect's modifying value selector.
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect, meta = (EditCondition = "bIsUsingSelector", EditConditionHides))
	FDMSValueSelectionForm DataPicker;

	virtual bool GenerateModifier_Implementation(ADMSActiveEffect* EI,FDMSAttributeModifier& OutModifier);
};

UCLASS(Blueprintable, ClassGroup = (Effect), meta = (DisplayName = "Mod Attribute Effect : From Attribute"))
class PROJECTDMSGAME_API UDMSEffect_ModAtt_FromAttribute : public UDMSEffect_ModAtt
{
	GENERATED_BODY()

public:
	//UDMSEffect_ModAtt_FromAttribute();

	/**
	* Modifier attribute tags to be referenced in the Active Effect.
	*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced, Category = Effect)
	TObjectPtr<UDMSAttributeModifierOp> ModifierOp;

	/**
	* Modifier attribute tags to be referenced in the Active Effect.
	*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect)
	FGameplayTagContainer ActiveEffectValueTags;

	virtual bool GenerateModifier_Implementation(ADMSActiveEffect* EI,FDMSAttributeModifier& OutModifier);
};



// 추가적으로 데이터 여러개를 참조해서 모디파이어를 만드는 식의 형태는 사용자가 직접 구현 할 수 있도록...