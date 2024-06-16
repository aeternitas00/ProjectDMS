// Copyright JeongWoo Lee

#pragma once

#include "ProjectDMS.h"
#include "Effect/DMSEffectorActorBase.h"
#include "DMSChaosTokenBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDMSGAME_API ADMSChaosTokenBase : public ADMSEffectorActorBase
{
	GENERATED_BODY()
public:
	ADMSChaosTokenBase(const FObjectInitializer& ObjectInitializer);
protected:

	/**
	* Attribute component.
	* Manage mana, resource, etc... of character.
	*/
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDMSAttributeComponent> AttributeComponent;

public:
	virtual UDMSEffectSet* GetOwningEffectSet(const FGameplayTag& iSetName) override;
	virtual void OnInitialized_Implementation() override;
};
