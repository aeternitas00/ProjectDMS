// Copyright JeongWoo Lee

#pragma once

/**
 * 	========================================
 *
 *	DMS_GAME_MODULE
 *
 *	Components to managing DMSAttributes.
 *
 *	========================================
 */


#include "DMSCoreIncludes.h"
#include "Attribute/DMSAttribute.h"
#include "Attribute/DMSSerializedAttribute.h"
#include "Common/DMSSpawnableComponent.h"
#include "Components/ActorComponent.h"
#include "DMSAttributeComponent.generated.h"


//
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DMSCORE_API UDMSAttributeComponent : public UDMSSpawnableComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDMSAttributeComponent();

protected:
	UPROPERTY()
	FOnAttributeModified OnAttributeAdded;
public:	
	/**
	 * Storing attribute instances
	 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Replicated)
	TArray<TObjectPtr<UDMSAttribute>> Attributes;
	
	/**
	 * Check this component contians tagged attribute.
	 */
	UFUNCTION(BlueprintCallable)
	bool ContainAttribute(const FGameplayTagContainer& Tag, bool Exact = false) const;
	bool ContainAttributeByQuery(const FGameplayTagQuery & Query, bool Exact = false) const;
	/**
	 * Trying modify attribute with param Modifier
	 * @param	Modifier							In modifier
	 * @return	true if modifying was successful
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool PredictModifier(const FGameplayTagContainer& AttributeTag, const FDMSAttributeModifier& Modifier) const;
	
	UFUNCTION(BlueprintCallable)
	void ApplyModifier(const FGameplayTagContainer& AttributeTag, const FDMSAttributeModifier& Modifier);

	/**
	 * Make named attribute if component doesn't contain param name.
	 * @param	AttributeName						key of Attributes
	 * @param	DefValue							Default value of creating attribute
	 */
	UFUNCTION(BlueprintCallable)
	UDMSAttribute* MakeAttribute(const FGameplayTagContainer& AttributeName, const TSubclassOf<UDMSAttributeValue>& AttributeValueClass, bool Exact = false);

	UFUNCTION(BlueprintCallable)
	UDMSAttribute* GenerateAndSetAttribute(const FGameplayTagContainer& AttributeName, UDMSAttributeValue* AttributeValue, bool Exact = false);

	UFUNCTION(BlueprintCallable)
	void RemoveAttribute(const FGameplayTagContainer& AttributeName, bool Exact = false);
	/**
	* Make named attribute if component doesn't contain param name.
	* @param	AttributeName						key of Attributes
	* @param	DefValue							Default value of creating attribute
	*/
	UFUNCTION(BlueprintCallable)
	UDMSAttribute* DuplicateAttribute(UDMSAttribute* Attribute, bool Exact = false);

	void RegisterAttribute_Internal(UDMSAttribute* NewAttribute);
	/**
	 * Bind param OnModified deletegate to named attribute instance.
	 * @param	AttributeName						Target attribute's tag
	 * @param	iDelegate							Binding delegate.
	 */
	UFUNCTION(BlueprintCallable)
	void BindOnModifiedToAttribute(const FGameplayTagContainer& AttributeName, const FOnAttributeModifiedSignature& iDelegate);

	UFUNCTION(BlueprintCallable)
	void BindOnAttributeAdded(const FOnAttributeModifiedSignature& iDelegate);

	/**
	 * Getter of attribute instance
	 * @param	AttributeName						Target attribute's tag
	 */
	UFUNCTION(BlueprintCallable)
	UDMSAttribute* GetAttribute(const FGameplayTagContainer& AttributeName, bool Exact = false) const;
	TArray<UDMSAttribute*> GetAttributesByQuery(const FGameplayTagQuery& TargetQuery, bool Exact = false) const;

	/**
	 * Getter of attribute value
	 * @param	AttributeName						Target attribute's tag
	 * @param	outValue							returning value
	 * @return	true if component contains AttributeName
	 */
	UFUNCTION(BlueprintCallable)
	UDMSAttributeValue* GetAttributeValue(const FGameplayTagContainer& AttributeName, bool Exact = false) const;
	template<typename T>
	T* GetTypedAttributeValue(const FGameplayTagContainer& AttributeName, bool Exact = false) const;	
	template<typename T>
	TArray<T*> GetTypedAttributeValuesByQuery(const FGameplayTagQuery& AttributeSearchQuery, bool Exact = false) const;

	//UFUNCTION(BlueprintCallable)
	//TArray<FDMSAttributeDefinition> ToDefinition();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

template<typename T>
T* UDMSAttributeComponent::GetTypedAttributeValue(const FGameplayTagContainer& AttributeName, bool Exact) const
{
	auto AttVal = GetAttributeValue(AttributeName,Exact);
	return AttVal ? Cast<T>(AttVal) : nullptr;
}

template<typename T>
TArray<T*> UDMSAttributeComponent::GetTypedAttributeValuesByQuery(const FGameplayTagQuery& AttributeSearchQuery, bool Exact) const
{
	TArray<T*> rv;
	for (auto& Att : GetAttributesByQuery(AttributeSearchQuery,Exact))
	{
		auto CastVal = Cast<T>(Att->AttributeValue);
		if (CastVal)
			rv.Add(CastVal);
	}
	return rv;
}
