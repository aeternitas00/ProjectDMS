// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DMSCoreIncludes.h"
#include "Attribute/DMSAttribute.h"
#include "DMSAttributeValue_Boolean.generated.h"

UENUM(BlueprintType)
enum class EDMSModifierType_Boolean : uint8
{
	MT_Set UMETA(DisplayName = "Set"),
	MT_AndEqual UMETA(DisplayName = "And equal (&=)"),
	MT_OrEqual UMETA(DisplayName = "Or equal (|=)"),
	MT_MAX
};

/*
 *	Class of single numeric attribute.
 */
UCLASS(BlueprintType)
class DMSCORE_API UDMSAttributeValue_Boolean : public UDMSAttributeValue
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = Attribute) 
	bool Value;

public:
	UDMSAttributeValue_Boolean();

	UFUNCTION(BlueprintCallable, BlueprintPure , Category = Attribute)
	bool GetValue() const { return Value; }

	UFUNCTION(BlueprintCallable, Category = Attribute)
	void SetValue(bool i) { Value=i; }

	//virtual void GetDeltaAfterModify(const FDMSAttributeModifier& Modifier,TObjectPtr<UDMSAttributeValue>& OutValue);
	virtual void GetDeltasAfterModify(const FDMSAttributeModifier& OriginalModifier,ADMSActiveEffect* OriginalActiveEffect,TArray<FDMSAttributeModifier>& OutModifiers);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

/**
*	
*/
UCLASS()
class DMSCORE_API UDMSAttributeModifierOp_Boolean : public UDMSAttributeModifierOp
{
	GENERATED_BODY()

public:	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect, meta = (DisplayName = "Exist failure condition"))
	EDMSModifierType_Boolean _ModifierType;

public:
	virtual void ExecuteOp_Implementation(UDMSAttributeValue* AttValue, UDMSAttributeValue* ModifierValue) override;
	virtual bool Predict_Implementation(UDMSAttribute* Target, UDMSAttributeValue* ModifierValue) override;
};

