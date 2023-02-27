// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "UObject/Interface.h"
#include "Attribute/DMSAttribute.h"
#include "DMSAttributeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDMSAttributeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTDMSGAME_API IDMSAttributeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual UDMSAttribute* GetAttribute(const FName& AttributeName)=0;
	virtual bool TryModAttribute(const FDMSAttributeModifier& Modifier)=0;
};
