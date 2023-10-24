// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DMSPlayerStateBase.h"

#include "Effect/DMSEIManagerComponent.h"


ADMSPlayerStateBase::ADMSPlayerStateBase(const FObjectInitializer& Initializer) /*: Super(Initializer)*/
{
	EIManagerComponent = CreateDefaultSubobject<UDMSEIManagerComponent>("EIManagerComponent");
}
