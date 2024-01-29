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
	UPROPERTY(BlueprintReadOnly, Replicated)
	TArray<UDMSAttribute*> Attributes; // Resources
	
	/**
	 * Check this component contians tagged attribute.
	 */
	UFUNCTION(BlueprintCallable)
	bool ContainAttribute(const FGameplayTagContainer& Tag) const;

	/**
	 * Trying modify attribute with param Modifier
	 * @param	Modifier							In modifier
	 * @return	true if modifying was successful
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool PredictModifier(UDMSAttributeModifier* Modifier) const;
	
	UFUNCTION(BlueprintCallable)
	void ApplyModifier(UDMSAttributeModifier* Modifier);

	/**
	 * Make named attribute if component doesn't contain param name.
	 * @param	AttributeName						key of Attributes
	 * @param	DefValue							Default value of creating attribute
	 */
	UFUNCTION(BlueprintCallable)
	UDMSAttribute* MakeAttribute(const FGameplayTagContainer& AttributeName, const TSubclassOf<UDMSAttribute>& AttributeClass);

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

	/**
	 * Getter of attribute value
	 * @param	AttributeName						Target attribute's tag
	 * @param	outValue							returning value
	 * @return	true if component contains AttributeName
	 */
	//UFUNCTION(BlueprintCallable)
	//bool GetAttributeValue(const FGameplayTag& AttributeName, float& outValue) const;

	//UFUNCTION(BlueprintCallable)
	//TArray<FDMSSerializedAttribute> ToSerialized();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
