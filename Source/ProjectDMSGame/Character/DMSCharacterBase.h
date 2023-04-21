// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Effect/DMSEffectorActorBase.h"
#include "Location/DMSLocatableInterface.h"
#include "Attribute/DMSSerializedAttribute.h"
#include "DMSCharacterBase.generated.h"

class UDMSCharacterDefinition;
class UDMSAttributeComponent;

UCLASS()
class PROJECTDMSGAME_API ADMSCharacterBase : public ADMSEffectorActorBase , public IDMSLocatableInterface
{
	GENERATED_BODY()
	
protected:

	/**
	 * Character's data.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<const UDMSCharacterDefinition> CharacterDefinition;

	/**
	 * Attribute component.
	 * Manage mana, resource, etc... of character.
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDMSAttributeComponent> AttributeComponent;

public:	
	// Sets default values for this actor's properties
	ADMSCharacterBase();

	virtual UDMSEffectSet* GetOwningEffectSet(const FName& iSetName) override;

	/**
	 *
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InitializeCharacter(const UDMSCharacterDefinition* NewDefinition);
	virtual void InitializeCharacter_Implementation(const UDMSCharacterDefinition* NewDefinition);
	
	/**
	 *
	 */
	UFUNCTION(BlueprintCallable)
	void SetupAttributes(const TArray<FDMSSerializedAttribute>& Attributes);
};
