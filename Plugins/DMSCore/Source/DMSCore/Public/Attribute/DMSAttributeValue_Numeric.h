// Copyright JeongWoo Lee

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
	// Rep Testing
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, /*ReplicatedUsing = OnRep_Value,*/ Category = Attribute) 
	float Value;

public:
	UDMSAttributeValue_Numeric();

	UFUNCTION(BlueprintCallable, BlueprintPure , Category = Attribute)
	float GetValue() const { return Value; }

	UFUNCTION(BlueprintCallable, Category = Attribute)
	void SetValue(float i);

	// OnRep Testing
	//UFUNCTION()
	//void OnRep_Value();

	virtual UDMSAttributeValue* GetDeltaAfterModify(const FDMSAttributeModifier& OriginalModifier, ADMSActiveEffect* OriginalActiveEffect);
	virtual void GetDeltasAfterModify(const FDMSAttributeModifier& OriginalModifier,ADMSActiveEffect* OriginalActiveEffect,TArray<FDMSAttributeModifier>& OutModifiers);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};


UENUM(BlueprintType)
enum class EDMSNumericOperatorFlag : uint8
{
	OF_Default = 0 UMETA(DisplayName = "Execute normally"),
	OF_Clamp UMETA(DisplayName = "Execute with clamp"),
	OF_FailCondition UMETA(DisplayName = "Block if predict result is specific value"),
	OF_MAX
};
/**
*	
*/
UCLASS()
class DMSCORE_API UDMSAttributeModifierOp_Numeric : public UDMSAttributeModifierOp
{
	GENERATED_BODY()

public:	

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect, meta = (DisplayName = "Modifier Type"))
	EDMSModifierType_Numeric ModifierType;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect)
	EDMSNumericOperatorFlag OperatorFlag;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Effect, meta = (EditCondition = "OperatorFlag != EDMSNumericOperatorFlag::OF_Default", EditConditionHides))
	float FlagMinValue;

	/**
	 * Clamping value.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Effect, meta = (EditCondition = "OperatorFlag != EDMSNumericOperatorFlag::OF_Default", EditConditionHides))
	float FlagMaxValue;

	/**
	* Fail checking operator.
	*/
	//UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Effect, meta = (EditCondition = "OperatorFlag != EDMSNumericOperatorFlag::OF_Default", EditConditionHides))
	//EDMSComparisonOperator FlagOperator;

	/**
	 * It has failure condition.
	 */
	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Effect, meta = (DisplayName = "Exist failure condition"))
	//bool bExistFailureCondition = false;


	/**
	 * Fail condition value
	 */
	//UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Effect, meta = (EditCondition = "bExistFailureCondition", EditConditionHides))
	//float FailureConditionValue; // float? 


public:
	virtual void ExecuteOp_Implementation(UDMSAttributeValue* AttValue, UDMSAttributeValue* ModifierValue) override;
	virtual bool Predict_Implementation(UDMSAttribute* Target, UDMSAttributeValue* ModifierValue) override;
};

