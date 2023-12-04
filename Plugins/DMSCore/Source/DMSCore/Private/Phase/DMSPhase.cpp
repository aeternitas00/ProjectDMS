// Fill out your copyright notice in the Description page of Project Settings.

#include "Phase/DMSPhase.h"
#include "Phase/DMSPhaseManager.h"
#include "Effect/DMSEffectDefinition.h"
#include "Sequence/DMSSeqManager.h"
#include "Sequence/DMSSequence.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "GameModes/DMSGameStateBase.h"

UDMSPhase::UDMSPhase()
{
	PhaseNode = CreateDefaultSubobject<UDMSEffectNode>("PhaseNode");
	PhaseNode->NodeTag = PhaseTag;
}

void UDMSPhase::StartPhase()
{
	OnPreStartPhase();
}

void UDMSPhase::End_OnPreStartPhase()
{
	OnStartPhase();
}

void UDMSPhase::OnStartPhase_Implementation()
{
	auto SM = UDMSCoreFunctionLibrary::GetDMSSequenceManager(); check(SM);
	auto GS = UDMSCoreFunctionLibrary::GetDMSGameState(); check(GS);

	auto NewSeq = SM->RequestCreateSequence(GS,GS,PhaseNode,{});

	NewSeq->AddToOnSequenceFinished_Native([this](bool Successed){
		OnStartPhase_Dynamic.Broadcast(this);
		OnStartPhase_Native.Broadcast(this);
	});

	SM->RunSequence(NewSeq);
}

void UDMSPhase::EndPhase()
{
	OnPreEndPhase();
}

void UDMSPhase::End_OnPreEndPhase()
{
	OnEndPhase();
}

void UDMSPhase::OnEndPhase_Implementation()
{
	OnEndPhase_Dynamic.Broadcast(this);
	OnEndPhase_Native.Broadcast(this);
}
