// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Effect/DMSEffectDefinition.h"
#include "DMSEffectNode_TypeBehavior.generated.h"

/**
 * 
 */
UCLASS(Abstract, Const, NotEditInlineNew)
class PROJECTDMSGAME_API UDMSEffectNode_TypeBehavior : public UDMSEffectNode
{
	GENERATED_BODY()
	
public:
	FName TypeName;
};
