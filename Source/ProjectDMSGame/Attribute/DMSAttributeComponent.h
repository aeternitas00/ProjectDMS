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
#include "Attribute/DMSAttributeInterface.h"
#include "Components/ActorComponent.h"
#include "DMSAttributeComponent.generated.h"


//
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTDMSGAME_API UDMSAttributeComponent : public UActorComponent//, public IDMSAttributeInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDMSAttributeComponent();

public:	

	/**
	 *
	 */
	UPROPERTY()
	TMap<FGameplayTag, UDMSAttribute*> Attributes; // Resources
	
	/**
	 *
	 */
	UFUNCTION(BlueprintCallable)
	bool TryModAttribute(const FDMSAttributeModifier& Modifier);
	
	/**
	 *
	 */
	UFUNCTION(BlueprintCallable)
	void MakeAttribute(const FGameplayTag& AttributeName, const float& DefValue = 0.0f);

	/**
	 *
	 */
	UFUNCTION(BlueprintCallable)
	void BindOnModifiedToAttribute(const FGameplayTag& AttributeName, const FOnAttributeModifiedSignature& iDelegate);

	/**
	 *
	 */
	UFUNCTION(BlueprintCallable)
	UDMSAttribute* GetAttribute(const FGameplayTag& AttributeName) const;

	/**
	 *
	 */
	UFUNCTION(BlueprintCallable)
	bool GetAttributeValue(const FGameplayTag& AttributeName, float& outValue) const;
};
