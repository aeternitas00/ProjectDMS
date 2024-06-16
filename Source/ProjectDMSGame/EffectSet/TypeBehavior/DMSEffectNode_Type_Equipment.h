// Copyright JeongWoo Lee

#pragma once

#include "ProjectDMS.h"
#include "EffectSet/TypeBehavior/DMSEffectNode_TypeBehavior.h"
#include "DMSEffectNode_Type_Equipment.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Type Behavior : Equipment"))
class PROJECTDMSGAME_API UDMSEffectNode_Type_Equipment : public UDMSEffectNode_TypeBehavior
{
	GENERATED_BODY()
	
public:
	UDMSEffectNode_Type_Equipment() { bForced=true; TypeName = TEXT("Equipment"); }
};
