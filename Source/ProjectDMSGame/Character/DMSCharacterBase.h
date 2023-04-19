// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Effect/DMSEffectorActorBase.h"
#include "Attribute/DMSSerializedAttribute.h"
#include "DMSCharacterBase.generated.h"

UCLASS()
class PROJECTDMSGAME_API ADMSCharacterBase : public ADMSEffectorActorBase
{
	GENERATED_BODY()
	
protected:

	/**
	 * Character's data.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UDMSCharacterDefinition* CharacterDefinition;

	/**
	 * Attribute component.
	 * Manage mana, resource, etc... of character.
	 */
	UPROPERTY(BlueprintReadOnly)
	class UDMSAttributeComponent* AttributeComponent;

public:	
	// Sets default values for this actor's properties
	ADMSCharacterBase();

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	/**
	 *
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InitializeCharacter(UDMSCharacterDefinition* NewDefinition);
	virtual void InitializeCharacter_Implementation(UDMSCharacterDefinition* NewDefinition);
	
	/**
	 *
	 */
	UFUNCTION(BlueprintCallable)
	void SetupAttributes(const TArray<FDMSSerializedAttribute>& Attributes);
};
