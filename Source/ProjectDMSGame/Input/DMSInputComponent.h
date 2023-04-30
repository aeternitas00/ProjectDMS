// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "EnhancedInputComponent.h"
#include "DMSInputComponent.generated.h"

/**
 * Unused
 */
UCLASS()
class PROJECTDMSGAME_API UDMSInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	UDMSInputComponent(const FObjectInitializer& ObjectInitializer);

};
