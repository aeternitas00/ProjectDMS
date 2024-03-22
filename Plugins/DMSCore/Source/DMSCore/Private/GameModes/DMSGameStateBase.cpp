// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/DMSGameStateBase.h"
#include "Player/DMSPlayerControllerBase.h"
#include "Player/DMSPlayerStateBase.h"
#include "Selector/DMSSelectorManager.h"
#include "Sequence/DMSSeqManager.h"
#include "Notify/DMSNotifyManager.h"
#include "Phase/DMSPhaseManager.h"
#include "Effect/DMSEffectHandler.h"
#include "Effect/DMSEIManagerComponent.h"

ADMSGameStateBase::ADMSGameStateBase(const FObjectInitializer& Initializer) /*: Super(Initializer)*/
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	PrimaryActorTick.TickGroup = TG_PostUpdateWork;

	//CardManagerComponent = CreateDefaultSubobject<UDMSCardManagerComponent>(TEXT("CardManagerComponent"));
	EIManagerComponent = CreateDefaultSubobject<UDMSEIManagerComponent>("EIManagerComponent");
	EffectHandler = CreateDefaultSubobject<UDMSEffectHandler>(TEXT("EffectHandler"));	
	NotifyManager = CreateDefaultSubobject<UDMSNotifyManager>(TEXT("NotifyManager"));
	SelectorManager = CreateDefaultSubobject<UDMSSelectorManager>(TEXT("SelectorManager"));
	PhaseManagerClass = UDMSPhaseManager::StaticClass();
	SequenceManagerClass = UDMSSeqManager::StaticClass();

	EIManagerComponent->SetIsReplicated(true);
	EffectHandler->SetIsReplicated(true);
	NotifyManager->SetIsReplicated(true);
	SelectorManager->SetIsReplicated(true);
}

void ADMSGameStateBase::Tick(float Deltatime)
{
	if(bNeedToCleanup){
		bNeedToCleanup = false;
		EffectHandler->CleanupNonPersistent();
		NotifyManager->CleanupPrevSeqTree();
	}

	Super::Tick(Deltatime);
}

void ADMSGameStateBase::PreInitializeComponents()
{
	if (PhaseManagerClass->IsValidLowLevelFast()) {
		PhaseManager = NewObject<UDMSPhaseManager>(this, PhaseManagerClass, TEXT("PhaseManager"));
		PhaseManager->SetIsReplicated(true);
		PhaseManager->RegisterComponent();
	}
	if (SequenceManagerClass->IsValidLowLevelFast()) {
		SequenceManager = NewObject<UDMSSeqManager>(this, SequenceManagerClass, TEXT("SequenceManager"));
		SequenceManager->SetIsReplicated(true);
		SequenceManager->RegisterComponent();
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

