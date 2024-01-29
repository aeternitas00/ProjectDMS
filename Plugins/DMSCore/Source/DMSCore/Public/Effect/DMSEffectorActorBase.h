// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DMSCoreIncludes.h"
#include "Common/DMSSpawnableDataBase.h"
#include "Effect/DMSEffectorInterface.h"
#include "DMSEffectorActorBase.generated.h"

class UDMSEIManagerComponent;

UCLASS(Abstract)
class DMSCORE_API ADMSEffectorActorBase : public ADMSSpawnableBase, public IDMSEffectorInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADMSEffectorActorBase(const FObjectInitializer& ObjectInitializer);

protected:
	/**
	 * Actor's effector component.
	 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UDMSEIManagerComponent> EIManagerComponent;
	
public:	

	virtual void OnInitialized_Implementation() override;
};
