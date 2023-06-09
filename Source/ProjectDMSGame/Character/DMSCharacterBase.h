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

	TObjectPtr<ADMSLocationBase> CurrentLocationRef;
public:	
	// Sets default values for this actor's properties
	ADMSCharacterBase(const FObjectInitializer& ObjectInitializer);

	virtual UDMSEffectSet* GetOwningEffectSet(const FGameplayTag& iSetName) override;
	virtual void SetCurrentLocation_Implementation(ADMSLocationBase* iLoc) { CurrentLocationRef = iLoc; }
	virtual ADMSLocationBase* GetCurrentLocation_Implementation() { return CurrentLocationRef; }

	/**
	 *
	 */
	virtual void OnInitialized_Implementation();
	
	/**
	 *
	 */
	UFUNCTION(BlueprintCallable)
	void SetupAttributes(const TArray<FDMSSerializedAttribute>& Attributes);


};
