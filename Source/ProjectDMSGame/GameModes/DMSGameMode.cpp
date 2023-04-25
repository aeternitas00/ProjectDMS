// Copyright Epic Games, Inc. All Rights Reserved.


#include "DMSGameMode.h"

#include "Sequence/DMSSeqManager.h"
#include "Notify/DMSNotifyManager.h"
#include "Phase/DMSPhaseManager.h"
#include "Effect/DMSEffectHandler.h"

#include "Player/DMSPlayerController.h"
#include "Effect/DMSEffectorInterface.h"
#include "Card/DMSCardDefinition.h"

#include "Scenario/DMSScenarioData.h"
#include "Location/DMSLocationData.h"
#include "Location/DMSLocationBase.h"

ADMSGameMode::ADMSGameMode()
{
	EffectHandler = CreateDefaultSubobject<UDMSEffectHandler>(TEXT("CardEffectHandler"));
	NotifyManager = CreateDefaultSubobject<UDMSNotifyManager>(TEXT("NotifyManager"));
	PhaseManagerClass= UDMSPhaseManager::StaticClass();
	SequenceManagerClass = UDMSSeqManager::StaticClass();
}

void ADMSGameMode::PreInitializeComponents()
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

void ADMSGameMode::SpawnCardsFromDeck(class ADMSPlayerController* iPC)
{

}

void ADMSGameMode::RegisterNotifyObject(TScriptInterface<IDMSEffectorInterface> Object)
{
	NotifyManager->RegisterNotifyObject(Object);
}

void ADMSGameMode::LoadAndSpawnScenario(UDMSScenarioData* ScenarioData)
{
	for (auto& Location : ScenarioData->Locations)
	{
		Location.LocationAsset.LoadSynchronous();
		auto LocData = Location.LocationAsset.Get();
		GetWorld()->SpawnActor<ADMSLocationBase>(LocData->LocationClass,Location.LocationOffset)->InitializeLocation(Location);
	}
	
}

