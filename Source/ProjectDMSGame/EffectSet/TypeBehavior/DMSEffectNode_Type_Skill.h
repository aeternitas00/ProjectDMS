// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "EffectSet/TypeBehavior/DMSEffectNode_TypeBehavior.h"
#include "DMSEffectNode_Type_Skill.generated.h"

/**
 * 
 */
UCLASS(meta=(DisplayName = "Type Behavior : Skill"))
class PROJECTDMSGAME_API UDMSEffectNode_Type_Skill : public UDMSEffectNode_TypeBehavior
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	UDMSNotifyCheckerDefinition_ObjectCompareBase* CheckerDefinition;

public:
	UDMSEffectNode_Type_Skill();
};
