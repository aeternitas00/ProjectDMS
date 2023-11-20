// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *
 *	========================================
 */

#include "DMSCoreIncludes.h"
#include "DMSSpawnableComponent.generated.h"


UCLASS(Abstract,Blueprintable, ClassGroup=(Custom) )
class DMSCORE_API UDMSSpawnableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDMSSpawnableComponent();

public:	

	UPROPERTY()
	TObjectPtr<UDMSSpawnableComponent> ParentComponent;

	void UpdateParentComponent();
};
