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
class DMSCORE_API UDMSEIManagerComponent : public UActorComponent/*, public IDMSEffectorInterface*/
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDMSEIManagerComponent();

	/**
	 * Owning Effects. 
	 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<ADMSActiveEffect*> OwnEffectInstances;

	// TODO :: MIGRATE EffectSet to here

protected:
	virtual void BeginPlay() override;

	//virtual UDMSEffectNode* ActivatorNodeGenerator(const FGameplayTag& EffectSetName, const uint8& idx);
public:	
	IDMSEffectorInterface* GetOwnerAsInterface() { return Cast<IDMSEffectorInterface>(GetOwner());}
	void AttachEffectInstance(ADMSActiveEffect* EI);
	bool OnNotifyReceived(TMultiMap<TScriptInterface<IDMSEffectorInterface>, ADMSActiveEffect*>& ResponsedObjects, bool iChainable, UDMSSequence* Seq, UObject* SourceTweak) ;

	// concepts????
	void SetupOwnEffect(UDMSEffectSet* EffectSet,const FGameplayTag& SetName);
};
