// Copyright JeongWoo Lee


#include "Player/DMSPlayerStateBase.h"

#include "Effect/DMSEIManagerComponent.h"


ADMSPlayerStateBase::ADMSPlayerStateBase(const FObjectInitializer& Initializer) /*: Super(Initializer)*/
{
	EIManagerComponent = CreateDefaultSubobject<UDMSEIManagerComponent>("EIManagerComponent");
}
