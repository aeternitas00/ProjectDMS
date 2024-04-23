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
	void AppendValue(const TArray<UObject*>& Items) { Value.Append(Items); }

	UFUNCTION(BlueprintCallable, Category = Attribute)
	void AddValue(UObject* Item) { Value.Add(Item); }

	UFUNCTION(BlueprintCallable, Category = Attribute)
	void RemoveValue(UObject* Item) { Value.Remove(Item); }

	UFUNCTION(BlueprintCallable, Category = Attribute)
	void RemoveValues(const TArray<UObject*>& Items) { for(auto It:Items)Value.Remove(It); }

	UFUNCTION(BlueprintCallable, Category = Attribute)
	void SetValue(const TArray<UObject*>& i) { Value=i; }

	virtual void GetDeltaAfterModify(const FDMSAttributeModifier& Modifier,TObjectPtr<UDMSAttributeValue>& OutValue);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

/**
*	
*/
UCLASS()
class DMSCORE_API UDMSAttributeModifierOp_Object : public UDMSAttributeModifierOp
{
	GENERATED_BODY()

public:
	virtual void ExecuteOp_Implementation(UDMSAttributeValue* AttValue, UDMSAttributeValue* ModifierValue) override;
	virtual bool Predict_Implementation(UDMSAttribute* Target, UDMSAttributeValue* ModifierValue) override;
};
