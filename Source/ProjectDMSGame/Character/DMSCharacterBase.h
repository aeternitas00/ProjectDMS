// Copyright JeongWoo Lee

#pragma once

#include "ProjectDMS.h"
#include "Effect/DMSEffectorActorBase.h"
#include "Location/DMSLocatableInterface.h"
#include "Attribute/DMSSerializedAttribute.h"
#include "DMSCharacterBase.generated.h"

class UDMSCharacterDefinition;
class UDMSAttributeComponent;

/**
 *	DEPRECATED
 */
UCLASS()
class PROJECTDMSGAME_API ADMSCharacterBase : public ADMSEffectorActorBase , public IDMSLocatableInterface
{
	GENERATED_BODY()
	
protected:
	/**
	 * Casted character's data.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<const UDMSCharacterDefinition> CharacterDefinition;

	/**
	 * Attribute component.
	 * Manage mana, resource, etc... of character.
	 */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TObjectPtr<UDMSAttributeComponent> AttributesComponent;

	/**
	 * Reference of current location.
	 */
	TObjectPtr<ADMSLocationBase> CurrentLocationRef;

public:	
	ADMSCharacterBase(const FObjectInitializer& ObjectInitializer);
	/**
	 *
	 */
	UFUNCTION(BlueprintCallable)
	void SetupAttributes(const TArray<FDMSAttributeDefinition>& Attributes);

	virtual UDMSEffectSet* GetOwningEffectSet(const FGameplayTag& iSetName) override;
	//virtual void SetCurrentLocation_Implementation(ADMSLocationBase* iLoc) { CurrentLocationRef = iLoc; }
	//virtual ADMSLocationBase* GetCurrentLocation_Implementation() { return CurrentLocationRef; }
	virtual void OnInitialized_Implementation();
};

