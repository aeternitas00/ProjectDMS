// Copyright JeongWoo Lee

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
class ADMSActiveEffect;

// Modifier structure can be used in anywhere ( definitions, other runtime logics... )
USTRUCT(BlueprintType)
struct DMSCORE_API FDMSAttributeModifier
{
	GENERATED_BODY()

public:
	/**
	 * Instanced definition of operator 
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, Category = Attribute)
	TObjectPtr<UDMSAttributeModifierOp> ModifierOp;

	/**
	 * Operating value.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, Category = Attribute)
	TObjectPtr<UDMSAttributeValue> Value;
};


// Serializable definition of attribute
USTRUCT(BlueprintType)
struct DMSCORE_API FDMSAttributeDefinition
{
	GENERATED_BODY()

public:
	/**
	 * Defualt tag of attribute.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute)
	FGameplayTagContainer DefaultTag;

	/**
	 * Defualt value of attribute.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, Category = Attribute)
	TObjectPtr<UDMSAttributeValue> DefaultValue;

	DMSCORE_API friend FArchive& operator<<(FArchive& Ar, FDMSAttributeDefinition& AttDefinition);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeModified, UDMSAttribute*, Attribute);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAttributeModifiedSignature, UDMSAttribute*, Attribute);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeValueModified, UDMSAttributeValue*, AttributeValue);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAttributeValueModifiedSignature, UDMSAttributeValue*, AttributeValue);

// Abstract wrapper class for values of attribute.
UCLASS(Blueprintable,EditInlineNew,Abstract)
class DMSCORE_API UDMSAttributeValue : public UObject
{
	GENERATED_BODY()

public:
	//// Server-side delegate
	//FOnAttributeValueModified OnServerAttributeValueChanged;

	//// Client-side delegate
	//FOnAttributeValueModified OnClientAttributeValueChanged;

	//UFUNCTION(BlueprintCallable, Category = Attribute)
	//void ExecuteModifierDefinition(class UDMSAttributeModifierDefinition* Modifier);

	UFUNCTION(BlueprintNativeEvent, Category = Attribute)
	void ExecuteModifier(const FDMSAttributeModifier& Modifier);
	virtual void ExecuteModifier_Implementation(const FDMSAttributeModifier& Modifier);
	virtual void GetDeltasAfterModify(const FDMSAttributeModifier& OriginalModifier,ADMSActiveEffect* OriginalActiveEffect,TArray<FDMSAttributeModifier>& OutModifiers);
	virtual bool IsSupportedForNetworking() const override {return true;}
};


/**
 *	Attribute object.
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class DMSCORE_API UDMSAttribute : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Tag of attribute
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Attribute)
	FGameplayTagContainer AttributeTag;

	/**
	 * 해당 어트리뷰트가 변화되었을 때 호출 될 델리게이트.
	 */
	UPROPERTY()
	FOnAttributeModified OnAttributeModified;
	
	// Server-side delegate
	UPROPERTY()
	FOnAttributeModified OnClientAttributeModified;

	/**
	 * Value of attribute.
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

	//UFUNCTION(BlueprintCallable, Server, Category = Attribute)
	//void Server_BindOnModified(const FOnAttributeModifiedSignature& iDelegate);

	//UFUNCTION(BlueprintCallable, Client, Category = Attribute)
	//void Client_BindOnModified(const FOnAttributeModifiedSignature& iDelegate);

	// Implements Ops
	UFUNCTION(BlueprintCallable, Category = Attribute)
	void ApplyModifier(const FDMSAttributeModifier& Modifier);

	//UFUNCTION(BlueprintCallable, Category = Attribute)

	// "Revert" 기능을 위한 헬퍼 함수. ( 리버트 할때 사용할 모디파이어를 리턴 )
	void GetDeltaAfterModify(const FDMSAttributeModifier& Modifier,TObjectPtr<UDMSAttributeValue>& OutValue);

	void GetDeltasAfterModify(const FDMSAttributeModifier& OriginalModifier,ADMSActiveEffect* OriginalActiveEffect,TArray<FDMSAttributeModifier>& OutModifiers);

	void GenerateValue(const TSubclassOf<UDMSAttributeValue>& ValueClass);

	void CopyValueFromOther(UDMSAttributeValue* Value);

	virtual bool IsSupportedForNetworking() const override {return true;}
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

/**
 *	Attibute Modifier object. Various ways to use it.
 */
UCLASS(BlueprintType,Blueprintable,EditInlineNew,Abstract)
class DMSCORE_API UDMSAttributeModifierOp : public UObject
{
	GENERATED_BODY()

public:	
	/**
	 * DEPRECATED :: Moved to implemented class.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute)
	EDMSModifierType AttributeModifierType;

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
