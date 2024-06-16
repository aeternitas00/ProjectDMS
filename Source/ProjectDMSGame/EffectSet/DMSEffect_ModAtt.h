// Copyright JeongWoo Lee

#pragma once

#include "Effect/DMSEffectDefinition.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Common/DMSValueSelectorDefinition.h"
#include "DMSEffect_ModAtt.generated.h"


UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Effect_ModAttribute)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Effect_ModAttribute_Revert);
/**
 *	Base for effects that change value of attributes.
 *	Implementing the GenerateModifier function allows implementing 'how to adjust attributes' in various ways."
 */
UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew, ClassGroup = (Effect), meta = (DisplayName = "ED Mod Attribute Base"))
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

	/**
	 *	Tags of modifing target attribute.
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect)
	FGameplayTagContainer TargetAttributeTags;

	/**
	 *	Create attribute if there isn't matching one.
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect, meta = (DisplayName = "Revert when AE was detached from target"))
	bool bTemporal;

	// Attribute Modifier generating rules

	/**
	 *	Get outer and outer's attribute component with current active effect.
	 */
	bool GetTargetAttComp(ADMSActiveEffect* iEI, AActor*& OutTarget, UDMSAttributeComponent*& OutComp);
	
	/**
	 * Generate an attribute modifier to be applied by this effect.
	 */
	UFUNCTION(BlueprintNativeEvent)
	bool GenerateModifier(ADMSActiveEffect* EI, ADMSSequence* SourceSequence, UPARAM(Ref) FDMSAttributeModifier& OutModifier);
	virtual bool GenerateModifier_Implementation(ADMSActiveEffect* EI, ADMSSequence* SourceSequence, FDMSAttributeModifier& OutModifier){return false;}

	void Revert(ADMSActiveEffect* iEI);
	// vf implements

	virtual void Work_Implementation(ADMSSequence* SourceSequence, ADMSActiveEffect* iEI, const FOnExecuteCompleted& OnWorkCompleted) override; // temp
	virtual bool Predict_Implementation(ADMSSequence* SourceSequence, ADMSActiveEffect* iEI) override;

	virtual FGameplayTagContainer GetEffectTags_Implementation() override;
};

/**
 *  Generate modifier with static value. ( it will never change )
 */
UCLASS(ClassGroup = (Effect), meta = (DisplayName = "ED Mod Attribute : Static"))
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

	virtual bool GenerateModifier_Implementation(ADMSActiveEffect* EI, ADMSSequence* SourceSequence, FDMSAttributeModifier& OutModifier){ OutModifier=StaticModifier; return StaticModifier.Value!=nullptr && StaticModifier.ModifierOp!=nullptr;}

};

/**
 * Processor targeting attribute values.
 */
UCLASS()
class PROJECTDMSGAME_API UDMSAttributeValueProcesser : public UDMSDataProcesser
{
	GENERATED_BODY()

public:
	//UDMSAttributeValueProcesser(){}
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Effect)
	FDMSAttributeModifier ProcessorModifier;

	virtual void Process_Implementation(UObject* iObject);
};

/**
 *	Flag for source actor of modifier value using in ModAtt_FromAttribute
 */
UENUM(BlueprintType)
enum class EDMSAttributeSourceFlag : uint8
{
	TG UMETA(DisplayName = "From first result of target generator"),
	AE UMETA(DisplayName = "From current active effect"),
	SourcePlayer UMETA(DisplayName = "From source player of sequence"),
	SourceObject UMETA(DisplayName = "From source object of sequence")
};


/** 
 * Creating modifiers with some other attribute as value ( most common method )
 */
UCLASS(ClassGroup = (Effect), meta = (DisplayName = "ED Mod Attribute : Value From Attribute"))
class PROJECTDMSGAME_API UDMSEffect_ModAtt_FromAttribute : public UDMSEffect_ModAtt
{
	GENERATED_BODY()

public:
	UDMSEffect_ModAtt_FromAttribute():UDMSEffect_ModAtt(),ValueAttributeSource(EDMSAttributeSourceFlag::AE){}

	/**
	 * ModifierOp to be used on generating modifier.
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced, Category = Effect)
	TObjectPtr<UDMSAttributeModifierOp> ModifierOp;

	/**
	 * Modifier attribute tags to be referenced in the source actor.
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect)
	FGameplayTagContainer ValueAttributeTags;

	/**
	 *	Flag of modifier value's source actor 
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect)
	EDMSAttributeSourceFlag ValueAttributeSource;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced, Category = Effect, meta=(EditCondition = "ValueAttributeSource == EDMSAttributeSourceFlag::TG", EditConditionHides))
	TObjectPtr<UDMSTargetGenerator> ValueAttributeOwner;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Category = Effect)
	TArray<TObjectPtr<UDMSAttributeValueProcesser>> ValueProcessers;

	virtual bool GenerateModifier_Implementation(ADMSActiveEffect* EI, ADMSSequence* SourceSequence, FDMSAttributeModifier& OutModifier);
};


// 추가적으로 데이터 여러개를 참조해서 모디파이어를 만드는 식의 형태는 사용자가 직접 구현 할 수 있도록...