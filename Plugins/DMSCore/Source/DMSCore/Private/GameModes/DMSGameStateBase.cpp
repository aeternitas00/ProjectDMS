// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/DMSGameStateBase.h"
#include "Player/DMSPlayerControllerBase.h"
#include "Player/DMSPlayerStateBase.h"

#include "Sequence/DMSSeqManager.h"
#include "Notify/DMSNotifyManager.h"
#include "Phase/DMSPhaseManager.h"
#include "Effect/DMSEffectHandler.h"
#include "Effect/DMSEIManagerComponent.h"

ADMSGameStateBase::ADMSGameStateBase(const FObjectInitializer& Initializer) /*: Super(Initializer)*/
{
	//CardManagerComponent = CreateDefaultSubobject<UDMSCardManagerComponent>(TEXT("CardManagerComponent"));
	EIManagerComponent = CreateDefaultSubobject<UDMSEIManagerComponent>("EIManagerComponent");
	EffectHandler = CreateDefaultSubobject<UDMSEffectHandler>(TEXT("EffectHandler"));	
	NotifyManager = CreateDefaultSubobject<UDMSNotifyManager>(TEXT("NotifyManager"));

	PhaseManagerClass = UDMSPhaseManager::StaticClass();
	SequenceManagerClass = UDMSSeqManager::StaticClass();

	EIManagerComponent->SetIsReplicated(true);
	EffectHandler->SetIsReplicated(true);
	NotifyManager->SetIsReplicated(true);
}

void ADMSGameStateBase::PreInitializeComponents()
{
	if (PhaseManagerClass->IsValidLowLevelFast()) {
		PhaseManager = NewObject<UDMSPhaseManager>(this, PhaseManagerClass, TEXT("PhaseManager"));
		PhaseManager->RegisterComponent();
		PhaseManager->SetIsReplicated(true);
	}
	if (SequenceManagerClass->IsValidLowLevelFast()) {
		SequenceManager = NewObject<UDMSSeqManager>(this, SequenceManagerClass, TEXT("SequenceManager"));
		SequenceManager->RegisterComponent();
		SequenceManager->SetIsReplicated(true);
	}

	Super::PreInitializeComponents();
}

void ADMSGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADMSGameStateBase, EIManagerComponent);
	DOREPLIFETIME(ADMSGameStateBase, EffectHandler);
	DOREPLIFETIME(ADMSGameStateBase, NotifyManager);
	DOREPLIFETIME(ADMSGameStateBase, PhaseManager);
	DOREPLIFETIME(ADMSGameStateBase, SequenceManager);
}

void ADMSGameStateBase::RegisterNotifyObject(TScriptInterface<IDMSEffectorInterface> Object)
{
	NotifyManager->RegisterNotifyObject(Object);
}

bool ADMSGameStateBase::SetLeaderPlayer(int32 inLeaderID)
{
	for (auto& Player : PlayerArray)
	{
		if (inLeaderID == Player->GetPlayerId())
		{
			LeaderPlayerID= inLeaderID;
			return true;
		}
	}
	return false;
}
\
\
ADMSPlayerStateBase* ADMSGameStateBase::FindPlayerFromId(int32 OwnerID)
{
	for (auto& Player : PlayerArray)
	{
		if (OwnerID == Player->GetPlayerId())
			return Cast<ADMSPlayerStateBase>(Player);
	}
	return nullptr;
}

ADMSPlayerControllerBase* ADMSGameStateBase::FindPlayerControllerFromId(int32 OwnerID)
{
	return FindPlayerFromId(OwnerID) == nullptr ? 
			Cast<ADMSPlayerControllerBase>(FindPlayerFromId(LeaderPlayerID)->GetPlayerController()) :
			Cast<ADMSPlayerControllerBase>(FindPlayerFromId(OwnerID)->GetPlayerController());
}

