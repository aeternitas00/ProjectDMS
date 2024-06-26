// Copyright JeongWoo Lee

#pragma once

#include "DMSCoreIncludes.h"
#include "Attribute/DMSAttribute.h"
#include "DMSAttributeValue_Tag.generated.h"

UENUM(BlueprintType)
enum class EDMSModifierType_Tag : uint8
{
	MT_Add UMETA(DisplayName = "Add"),
	MT_Remove UMETA(DisplayName = "Remove"),
	MT_Override UMETA(DisplayName = "Set (Override)"),
	MT_MAX
};

/**
 * 
 */
UCLASS()
class DMSCORE_API UDMSAttributeValue_Tag : public UDMSAttributeValue
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = Attribute) 
	FGameplayTagContainer Value;


public:
	UDMSAttributeValue_Tag(){}

	UFUNCTION(BlueprintCallable, BlueprintPure , Category = Attribute)
	FGameplayTagContainer GetValue() const { return Value; }

	UFUNCTION(BlueprintCallable, Category = Attribute)
	void SetValue(const FGameplayTagContainer& i) { Value=i; }

	UFUNCTION(BlueprintCallable, Category = Attribute)
	void AddSingleValue(const FGameplayTag& Item) { Value.AddTag(Item); }

	UFUNCTION(BlueprintCallable, Category = Attribute)
	void AddValue(const FGameplayTagContainer& Item) { Value.AppendTags(Item); }

	UFUNCTION(BlueprintCallable, Category = Attribute)
	void RemoveValue(const FGameplayTagContainer& Item) { Value.RemoveTags(Item); }

	UFUNCTION(BlueprintCallable, Category = Attribute)
	void ClearValue() { Value.Reset(); }

	//virtual void GetDeltaAfterModify(const FDMSAttributeModifier& Modifier,TObjectPtr<UDMSAttributeValue>& OutValue);
	virtual void GetDeltasAfterModify(const FDMSAttributeModifier& OriginalModifier,ADMSActiveEffect* OriginalActiveEffect,TArray<FDMSAttributeModifier>& OutModifiers);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


};

UCLASS()
class DMSCORE_API UDMSAttributeModifierOp_Tag : public UDMSAttributeModifierOp
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect, meta = (DisplayName = "Modifier Type"))
	EDMSModifierType_Tag ModifierType;
public:
	virtual void ExecuteOp_Implementation(UDMSAttributeValue* AttValue, UDMSAttributeValue* ModifierValue) override;
	virtual bool Predict_Implementation(UDMSAttribute* Target, UDMSAttributeValue* ModifierValue) override;
};
