// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DMSCoreIncludes.h"
#include "Attribute/DMSAttribute.h"
#include "DMSAttributeValue_Object.generated.h"

class UDMSAttributeModifierOp;
class UDMSAttributeValue;

UCLASS(BlueprintType)
class DMSCORE_API UDMSAttributeValue_Object : public UDMSAttributeValue
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = Attribute) 
	TArray<TObjectPtr<UObject>> Value;

public:
	//UDMSAttributeValue_Object();

	UFUNCTION(BlueprintCallable, BlueprintPure , Category = Attribute)
	TArray<UObject*> GetValue() const { return Value; }

	UFUNCTION(BlueprintCallable, Category = Attribute)
	void AppendValue(const TArray<UObject*>& Items) { for(auto& Item : Items) Value.AddUnique(Item); }

	UFUNCTION(BlueprintCallable, Category = Attribute)
	void AddValue(UObject* Item) { Value.AddUnique(Item); }

	UFUNCTION(BlueprintCallable, Category = Attribute)
	void RemoveValue(UObject* Item) { Value.Remove(Item); }

	UFUNCTION(BlueprintCallable, Category = Attribute)
	void RemoveValues(const TArray<UObject*>& Items) { for(auto& Item : Items) Value.Remove(Item); }

	UFUNCTION(BlueprintCallable, Category = Attribute)
	void SetValue(const TArray<UObject*>& i) { Value=i; }

	UFUNCTION(BlueprintCallable, Category = Attribute)
	void ClearValue() { Value.Empty(); }

	//virtual void GetDeltaAfterModify(const FDMSAttributeModifier& Modifier,TObjectPtr<UDMSAttributeValue>& OutValue);
	virtual void GetDeltasAfterModify(const FDMSAttributeModifier& OriginalModifier,ADMSActiveEffect* OriginalActiveEffect,TArray<FDMSAttributeModifier>& OutModifiers);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

//UCLASS(BlueprintType)
//class DMSCORE_API UDMSAttributeValue_MultiObject : public UDMSAttributeValue


/**
*	
*/
UCLASS()
class DMSCORE_API UDMSAttributeModifierOp_Object : public UDMSAttributeModifierOp
{
	GENERATED_BODY()
protected:

public:
	virtual void ExecuteOp_Implementation(UDMSAttributeValue* AttValue, UDMSAttributeValue* ModifierValue) override;
	virtual bool Predict_Implementation(UDMSAttribute* Target, UDMSAttributeValue* ModifierValue) override;
};
