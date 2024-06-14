// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DMSCoreIncludes.h"
#include "Attribute/DMSAttribute.h"
#include "DMSAttributeValue_Numeric.generated.h"

UENUM(BlueprintType)
enum class EDMSModifierType_Numeric : uint8
{
	MT_Additive UMETA(DisplayName = "Additive"),
	MT_Multiplicative UMETA(DisplayName = "Multiplicative"),
	MT_Override UMETA(DisplayName = "Set (Override)"),
	MT_Subtractive UMETA(DisplayName = "Subtractive"),
	MT_MAX
};

/*
 *	Class of single numeric attribute.
 */
UCLASS(BlueprintType)
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

	//virtual void GetDeltaAfterModify(const FDMSAttributeModifier& Modifier,TObjectPtr<UDMSAttributeValue>& OutValue);
	virtual void GetDeltasAfterModify(const FDMSAttributeModifier& OriginalModifier,ADMSActiveEffect* OriginalActiveEffect,TArray<FDMSAttributeModifier>& OutModifiers);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

/**
*	
*/
UCLASS()
class DMSCORE_API UDMSAttributeModifierOp_Numeric : public UDMSAttributeModifierOp
{
	GENERATED_BODY()

public:	
	/**
	* It has failure condition.
	*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect, meta = (DisplayName = "Exist failure condition"))
	bool bExistFailureCondition = false;

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
	virtual void ExecuteOp_Implementation(UDMSAttributeValue* AttValue, UDMSAttributeValue* ModifierValue) override;
	virtual bool Predict_Implementation(UDMSAttribute* Target, UDMSAttributeValue* ModifierValue) override;
};

