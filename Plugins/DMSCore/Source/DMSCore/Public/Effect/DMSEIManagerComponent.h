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

#include "DMSCoreIncludes.h"
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
class DMSCORE_API UDMSEIManagerComponent : public UActorComponent, public IDMSEffectorInterface
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

	// TODO :: MIGRATE EffectSet to here

protected:
	virtual void BeginPlay() override;

	virtual UDMSEffectNode* ActivatorNodeGenerator(const FGameplayTag& EffectSetName, const uint8& idx);
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual UObject* GetObject() { return Cast<UObject>(GetOwner()); }
	virtual AActor* GetOwningPlayer();
	virtual void AttachEffectInstance(UDMSEffectInstance* EI) override;
	virtual bool OnNotifyReceived(TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*>& ResponsedObjects, bool iChainable, UDMSSequence* Seq, UObject* SourceTweak) override;
	virtual UDMSEffectSet* GetOwningEffectSet(const FGameplayTag& iSetName) override;

	
	// concepts????
	void SetupOwnEffect(UDMSEffectSet* EffectSet,const FGameplayTag& SetName);
};
