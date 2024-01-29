// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_GAME_MODULE
 *
 *	Base of Card actor in DMS GAME
 *
 *	========================================
 */

#include "ProjectDMS.h"
#include "Effect/DMSEffectorActorBase.h"
#include "Location/DMSLocatableInterface.h"
#include "DMSCardBase.generated.h"

class UDMSCardDefinition;
class UDMSCardContainerComponent;
class UDMSAttributeComponent;

/**
 * 	========================================
 *
 *	ADMSCardBase : Base of Card actor. Wiil be implemented in BP.
 *
 *	========================================
 */
UCLASS(Blueprintable, Abstract)
class PROJECTDMSGAME_API ADMSCardBase : public ADMSEffectorActorBase , public IDMSLocatableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADMSCardBase(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	/**
	* Attribute component.
	* Manage mana, resource, etc... of character.
	*/
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDMSAttributeComponent> AttributeComponent;
	/**
	 * Card's data.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<const UDMSCardDefinition> CardDefinition;

	/**
	 * Card container that owning this card.
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDMSCardContainerComponent> OwningContainer;

public:	

	/**
	 * Native Get,Setter
	 */
	const UDMSCardDefinition* GetCardDefinition();
	void SetCardDefinition(const UDMSCardDefinition* iCardDefinition); // Init

	UDMSCardContainerComponent* GetOwningContainer() {return OwningContainer;}
	void SetOwningContainer(UDMSCardContainerComponent* Container) { OwningContainer = Container;}


	// == Virtual functions == //

	// IDMSEffectorInterface Implements.
	virtual UDMSEffectSet* GetOwningEffectSet(const FGameplayTag& iSetName) override;

	virtual void OnInitialized_Implementation() override;
	//virtual void PostInitialize_Implementation() override;

	virtual void SetCurrentLocation_Implementation(ADMSLocationBase* iLoc);
	virtual ADMSLocationBase* GetCurrentLocation_Implementation();
	virtual int GetDistanceWith_Implementation(const TScriptInterface<IDMSLocatableInterface>& OtherObject);
	virtual bool LocatingTo_Implementation(ADMSLocationBase* TargetLocation);
	//virtual void ResetPreviewObject() override;
};
