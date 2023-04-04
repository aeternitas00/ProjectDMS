// Fill out your copyright notice in the Description page of Project Settings.

#include "DMSPhase.h"
#include "DMSPhaseManager.h"
#include "Effect/DMSEffectDefinition.h"

UDMSPhase::UDMSPhase()
{
	PhaseNode = CreateDefaultSubobject<UDMSEffectNode>("PhaseNode");
	PhaseNode->NodeTag = PhaseTag;
}

void UDMSPhase::PhaseStart()
{
	OnPhaseStarted.Broadcast(this);
	OnPhaseStarted_Native.Broadcast(this);
}

void UDMSPhase::PhaseEnd()
{
	OnPhaseEnded.Broadcast(this);
	OnPhaseEnded_Native.Broadcast(this);
}
