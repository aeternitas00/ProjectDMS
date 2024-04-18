// Fill out your copyright notice in the Description page of Project Settings.

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
	bool ContainAttribute(const FGameplayTagContainer& Tag) const;
	bool ContainAttributeByQuery(const FGameplayTagQuery & Query) const;
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
	UDMSAttribute* MakeAttribute(const FGameplayTagContainer& AttributeName, const TSubclassOf<UDMSAttributeValue>& AttributeValueClass);

	UFUNCTION(BlueprintCallable)
	UDMSAttribute* GenerateAndSetAttribute(const FGameplayTagContainer& AttributeName, UDMSAttributeValue* AttributeValue);

	UFUNCTION(BlueprintCallable)
	void RemoveAttribute(const FGameplayTagContainer& AttributeName);
	/**
	* Make named attribute if component doesn't contain param name.
	* @param	AttributeName						key of Attributes
	* @param	DefValue							Default value of creating attribute
	*/
	UFUNCTION(BlueprintCallable)
	UDMSAttribute* DuplicateAttribute(UDMSAttribute* Attribute);

	/**
	 * Bind param OnModified deletegate to named attribute instance.
	 * @param	AttributeName						Target attribute's tag
	 * @param	iDelegate							Binding delegate.
	 */
	UFUNCTION(BlueprintCallable)
	void BindOnModifiedToAttribute(const FGameplayTagContainer& AttributeName, const FOnAttributeModifiedSignature& iDelegate);

	/**
	 * Getter of attribute instance
	 * @param	AttributeName						Target attribute's tag
	 */
	UFUNCTION(BlueprintCallable)
	UDMSAttribute* GetAttribute(const FGameplayTagContainer& AttributeName) const;
	TArray<UDMSAttribute*> GetAttributesByQuery(const FGameplayTagQuery& TargetQuery) const;

	/**
	 * Getter of attribute value
	 * @param	AttributeName						Target attribute's tag
	 * @param	outValue							returning value
	 * @return	true if component contains AttributeName
	 */
	UFUNCTION(BlueprintCallable)
	UDMSAttributeValue* GetAttributeValue(const FGameplayTagContainer& AttributeName) const;
	template<typename T>
	T* GetTypedAttributeValue(const FGameplayTagContainer& AttributeName) const;	
	template<typename T>
	TArray<T*> GetTypedAttributeValuesByQuery(const FGameplayTagQuery& AttributeSearchQuery) const;

	//UFUNCTION(BlueprintCallable)
	//TArray<FDMSAttributeDefinition> ToDefinition();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

template<typename T>
inline T* UDMSAttributeComponent::GetTypedAttributeValue(const FGameplayTagContainer& AttributeName) const
{
	return Cast<T>(GetAttributeValue(AttributeName));
}

template<typename T>
inline TArray<T*> UDMSAttributeComponent::GetTypedAttributeValuesByQuery(const FGameplayTagQuery& AttributeSearchQuery) const
{
	TArray<T*> rv;
	for (auto& Att : GetAttributesByQuery(AttributeSearchQuery))
	{
		rv.Add(Cast<T>(Att));
	}
	return rv;
}
