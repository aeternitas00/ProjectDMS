// Copyright Epic Games, Inc. All Rights Reserved.


#include "GameModes/DMSGameModeBase.h"
//#include "Player/DMSPlayerStateBase.h"
#include "GameModes/DMSGameStateBase.h"

#include "Sequence/DMSSeqManager.h"
#include "Notify/DMSNotifyManager.h"
#include "Phase/DMSPhaseManager.h"
#include "Effect/DMSEffectHandler.h"

//#include "Player/DMSPlayerController.h"
#include "Effect/DMSEffectorInterface.h"
//
//#include "Character/DMSCharacterDefinition.h"
//#include "Card/DMSCardManagerComponent.h"
//#include "Card/DMSCardBase.h"
//#include "Card/DMSCardDefinition.h"
//#include "Card/DMSCardData.h"
//#include "Scenario/DMSScenarioData.h"
//#include "Location/DMSLocationData.h"
//#include "Location/DMSLocationBase.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
////#include "Common/DMSSpawnableDataBase.h"

ADMSGameModeBase::ADMSGameModeBase()
{
	EffectHandler = CreateDefaultSubobject<UDMSEffectHandler>(TEXT("CardEffectHandler"));
	NotifyManager = CreateDefaultSubobject<UDMSNotifyManager>(TEXT("NotifyManager"));
	PhaseManagerClass= UDMSPhaseManager::StaticClass();
	SequenceManagerClass = UDMSSeqManager::StaticClass();
}

void ADMSGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	bool rv = GetDMSGameState()->IsValidLowLevelFast();
	if (rv && GetDMSGameState()->PlayerArray.Num() != 0)
	{
		GetDMSGameState()->SetLeaderPlayer(GetDMSGameState()->PlayerArray[0]->GetPlayerId());
	}
	//DMSGameState=GetGameState<ADMSGameState>();
}

ADMSGameStateBase* ADMSGameModeBase::GetDMSGameState() { return GetGameState<ADMSGameStateBase>(); }

void ADMSGameModeBase::PreInitializeComponents()
{
	if(PhaseManagerClass->IsValidLowLevelFast()){
		PhaseManager = NewObject<UDMSPhaseManager>(this, PhaseManagerClass,TEXT("PhaseManager"));
		PhaseManager->RegisterComponent();
	}
	if (SequenceManagerClass->IsValidLowLevelFast()) {
		SequenceManager = NewObject<UDMSSeqManager>(this, SequenceManagerClass, TEXT("SequenceManager"));
		SequenceManager->RegisterComponent();
	}
	Super::PreInitializeComponents();
}

void ADMSGameModeBase::RegisterNotifyObject(TScriptInterface<IDMSEffectorInterface> Object)
{
	NotifyManager->RegisterNotifyObject(Object);
}


