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

///**
// *	Attibute Modifier struct. Using in ModAtt Effect. 
// */
//USTRUCT(BlueprintType)
//struct FDMSAttributeModifier
//{
//	GENERATED_BODY()
//
//public:
//	/**
//	 * Tag of modifing attribute
//	 */
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute)
//	FGameplayTagQuery TargetQuery;
//
//	/**
//	 * 
//	 */
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute)
//	TObjectPtr<UDMSAttributeModifier> ModifierDefinition;
//};




DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeModified, UDMSAttribute*, Attribute);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAttributeModifiedSignature, UDMSAttribute*, Attribute);


UCLASS(BlueprintType,Blueprintable,DefaultToInstanced,Abstract)
class DMSCORE_API UDMSAttributeValue : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, Category = Attribute)
	void ExecuteOp(UDMSAttributeModifier* Modifier);
	virtual void ExecuteOp_Implementation(UDMSAttributeModifier* Modifier);

	virtual bool IsSupportedForNetworking() const override {return true;}
};

/**
 *	Class of attribute base.
 */
UCLASS(BlueprintType,NotBlueprintable,DefaultToInstanced)
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

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Category = Attribute)
	TObjectPtr<UDMSAttributeValue> AttributeValue;

public:
	UDMSAttributeBase();

	/**
	* Bind to OnAttributeModified
	*/
	UFUNCTION(BlueprintCallable)
	void BindOnModified(const FOnAttributeModifiedSignature& iDelegate);

	// Implements Ops
	UFUNCTION(BlueprintCallable, Category = Attribute)
	void ApplyModifier(UDMSAttributeModifier* Modifier);

	virtual bool IsSupportedForNetworking() const override {return true;}
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};


/**
*	Attibute Modifier struct. Using in ModAtt Effect. 
*/
UCLASS(BlueprintType,Blueprintable,DefaultToInstanced,Abstract)
class DMSCORE_API UDMSAttributeModifier : public UObject
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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute)
	FGameplayTagQuery TargetQuery;

	/**
	 * 
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, Category = Attribute)
	TObjectPtr<UDMSAttributeValue> AttributeValue;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = Attribute)
	bool Predict(UDMSAttribute* Target) const;
	virtual bool Predict_Implementation(UDMSAttribute* Target) const {return false; /*Target->GetClass()==AttributeValue->GetClass();*/}
};













/*
 *	Class of single numeric attribute.
 */
UCLASS()
class DMSCORE_API UDMSAttributeValue_Numeric : public UDMSAttributeValue
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = Attribute) 
	float Value;

public:
	UDMSAttributeValue_Numeric();

	UFUNCTION(BlueprintCallable, BlueprintPure , Category = Attribute)
	float GetValue() const { return Value; }

	UFUNCTION(BlueprintCallable, Category = Attribute)
	void SetValue(float i) { Value=i; }

	virtual void ExecuteOp_Implementation(UDMSAttributeModifier* Modifier);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};



/**
*	Attibute Modifier struct. Using in ModAtt Effect. 
*/
UCLASS(BlueprintType,Blueprintable,DefaultToInstanced)
class DMSCORE_API UDMSAttributeModifier_Numeric : public UObject
{
	GENERATED_BODY()

protected:	
	/**
	* It has failure condition.
	*/
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

public:
	virtual bool Predict_Implementation(UDMSAttribute* Target) const;
};
