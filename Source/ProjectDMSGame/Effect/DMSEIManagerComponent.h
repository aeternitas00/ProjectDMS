// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_GAME_MODULE
 *
 *	Effect instance management component can be attached to various actors such as cards, enemies, and more.
 *
 * =========================================
 */

#include "ProjectDMS.h"
#include "Effect/DMSEffectInstance.h"
#include "Components/ActorComponent.h"
#include "DMSEIManagerComponent.generated.h"

class UDMSEffectSet;

/**
 * This component is used by attaching it to effector actors that require EI interactions.
 * 
 * TODO :: SelfLogging System?
 */ 
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTDMSGAME_API UDMSEIManagerComponent : public UActorComponent, public IDMSEffectorInterface//, public IDMSAttributeInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDMSEIManagerComponent();

	/** 
	 * Attached Effects
	 */
	UPROPERTY()
	TArray<UDMSEffectInstance*> EffectInstances;

	/**
	 * Owning Effects. 
	 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<UDMSEffectInstance*> OwnEffectInstances;

protected:
	virtual void BeginPlay() override;

	virtual UDMSEffectNode* ActivatorNodeGenerator(const FName& EffectSetName, const uint8& idx);
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual UObject* GetObject() { return Cast<UObject>(GetOwner()); }
	virtual void AttachEffectInstance(UDMSEffectInstance* EI) override;
	virtual bool OnNotifyReceived(TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*>& ResponsedObjects, bool iChainable, UDMSSequence* Seq, UObject* SourceTweak) override;
	virtual UDMSEffectSet* GetOwningEffectSet(const FName& iSetName) override;
	virtual AActor* GetOwningPlayer() { return GetOwner()->GetOwner(); }

	
	// concepts????
	void SetupOwnEffect(UDMSEffectSet* EffectSet,const FName& SetName);
};
