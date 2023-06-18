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


#include "ProjectDMS.h"
#include "Attribute/DMSAttribute.h"
#include "Attribute/DMSSerializedAttribute.h"
#include "Components/ActorComponent.h"
#include "DMSAttributeComponent.generated.h"


//
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTDMSGAME_API UDMSAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDMSAttributeComponent();

public:	

	/**
	 * Storing attribute instances
	 */
	UPROPERTY()
	TMap<FGameplayTag, UDMSAttribute*> Attributes; // Resources
	
	/**
	 * Trying modify attribute with param Modifier
	 * @param	Modifier							In modifier
	 * @return	true if modifying was successful
	 */
	UFUNCTION(BlueprintCallable)
	bool TryModAttribute(const FDMSAttributeModifier& Modifier, float& OutValue, bool Apply = true);
	

	/**
	 * Make named attribute if component doesn't contain param name.
	 * @param	AttributeName						key of Attributes
	 * @param	DefValue							Default value of creating attribute
	 */
	UFUNCTION(BlueprintCallable)
	void MakeAttribute(const FGameplayTag& AttributeName, const float& DefValue = 0.0f);

	/**
	 * Bind param OnModified deletegate to named attribute instance.
	 * @param	AttributeName						Target attribute's tag
	 * @param	iDelegate							Binding delegate.
	 */
	UFUNCTION(BlueprintCallable)
	void BindOnModifiedToAttribute(const FGameplayTag& AttributeName, const FOnAttributeModifiedSignature& iDelegate);

	/**
	 * Getter of attribute instance
	 * @param	AttributeName						Target attribute's tag
	 */
	UFUNCTION(BlueprintCallable)
	UDMSAttribute* GetAttribute(const FGameplayTag& AttributeName);

	/**
	 * Getter of attribute value
	 * @param	AttributeName						Target attribute's tag
	 * @param	outValue							returning value
	 * @return	true if component contains AttributeName
	 */
	UFUNCTION(BlueprintCallable)
	bool GetAttributeValue(const FGameplayTag& AttributeName, float& outValue) const;

	UFUNCTION(BlueprintCallable)
	TArray<FDMSSerializedAttribute> ToSerialized();
};
