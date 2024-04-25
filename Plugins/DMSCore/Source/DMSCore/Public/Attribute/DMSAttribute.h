// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_GAME_MODULE
 *
 *	Attributes to use in this game (numeric values to be used by game actors).
 *
 *	========================================
 */

#include "DMSCoreIncludes.h"
#include "Common/DMSCommons.h"
#include "Attribute/DMSSerializedAttribute.h"
#include "DMSAttribute.generated.h"

class UDMSAttributeModifierOp;
class UDMSAttributeValue;

USTRUCT(BlueprintType)
struct FDMSAttributeModifier
{
	GENERATED_BODY()

public:
	/**
	* 
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, Category = Attribute)
	TObjectPtr<UDMSAttributeModifierOp> ModifierOp;

	/**
	* 
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, Category = Attribute)
	TObjectPtr<UDMSAttributeValue> Value;
};

USTRUCT(BlueprintType)
struct FDMSAttributeDefinition
{
	GENERATED_BODY()

public:
	/**
	* 
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute)
	FGameplayTagContainer DefaultTag;

	/**
	* 
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, Category = Attribute)
	TObjectPtr<UDMSAttributeValue> DefaultValue;

	DMSCORE_API friend FArchive& operator<<(FArchive& Ar, FDMSAttributeDefinition& AttDefinition);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeModified, UDMSAttribute*, Attribute);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAttributeModifiedSignature, UDMSAttribute*, Attribute);


UCLASS(Blueprintable,EditInlineNew,Abstract)
class DMSCORE_API UDMSAttributeValue : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = Attribute)
	void ExecuteModifier(const FDMSAttributeModifier& Modifier);
	virtual void ExecuteModifier_Implementation(const FDMSAttributeModifier& Modifier);

	//UFUNCTION(BlueprintNativeEvent, Category = Attribute)
	virtual void GetDeltaAfterModify(const FDMSAttributeModifier& Modifier,TObjectPtr<UDMSAttributeValue>& OutValue);
	//virtual void GetDeltaAfterModify_Implementation(const FDMSAttributeModifier& Modifier,TObjectPtr<UDMSAttributeValue>& OutValue);

	virtual bool IsSupportedForNetworking() const override {return true;}
};


/**
 *	Class of attribute base.
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class DMSCORE_API UDMSAttribute : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * 
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Attribute)
	FGameplayTagContainer AttributeTag;

	/**
	 * 해당 어트리뷰트가 변화되었을 때 호출 될 델리게이트.
	 */
	UPROPERTY()
	FOnAttributeModified OnAttributeModified;
	
	/**
	 * 
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Replicated, Category = Attribute)
	TObjectPtr<UDMSAttributeValue> AttributeValue;

public:
	UDMSAttribute();

	/**
	* Bind to OnAttributeModified
	*/
	UFUNCTION(BlueprintCallable, Category = Attribute)
	void BindOnModified(const FOnAttributeModifiedSignature& iDelegate);

	// Implements Ops
	UFUNCTION(BlueprintCallable, Category = Attribute)
	void ApplyModifier(const FDMSAttributeModifier& Modifier);

	//UFUNCTION(BlueprintCallable, Category = Attribute)
	void GetDeltaAfterModify(const FDMSAttributeModifier& Modifier,TObjectPtr<UDMSAttributeValue>& OutValue);

	void GenerateValue(const TSubclassOf<UDMSAttributeValue>& ValueClass);
	void DuplicateValue(UDMSAttributeValue* Value);

	virtual bool IsSupportedForNetworking() const override {return true;}
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

/**
 *	Attibute Modifier object. Using in ModAtt Effect. 
 */
UCLASS(BlueprintType,Blueprintable,EditInlineNew,Abstract)
class DMSCORE_API UDMSAttributeModifierOp : public UObject
{
	GENERATED_BODY()

public:	
	/**
	 * 
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute)
	EDMSModifierType AttributeModifierType;

	/**
	 * 
	 */
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute)
	//FGameplayTagContainer AttributeTag;

	UFUNCTION(BlueprintNativeEvent)
	void ExecuteOp(UDMSAttributeValue* AttValue, UDMSAttributeValue* ModifierValue);
	virtual void ExecuteOp_Implementation(UDMSAttributeValue* AttValue, UDMSAttributeValue* ModifierValue){}

	UFUNCTION(BlueprintNativeEvent, Category = Attribute)
	bool Predict(UDMSAttribute* Target, UDMSAttributeValue* ModifierValue);
	virtual bool Predict_Implementation(UDMSAttribute* Target, UDMSAttributeValue* ModifierValue) {return false;}
};

// Expose to Editor as instanced member of BP classes.
USTRUCT(BlueprintType)
struct DMSCORE_API FDMSInstancedModifierOp
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced)
	TObjectPtr<UDMSAttributeModifierOp> ModifierOp;
};
