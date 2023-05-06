// Copyright Epic Games, Inc. All Rights Reserved.


#include "DMSGameMode.h"
#include "GameModes/DMSGameState.h"

#include "Sequence/DMSSeqManager.h"
#include "Notify/DMSNotifyManager.h"
#include "Phase/DMSPhaseManager.h"
#include "Effect/DMSEffectHandler.h"

#include "Player/DMSPlayerController.h"
#include "Effect/DMSEffectorInterface.h"

#include "Card/DMSCardManagerComponent.h"
#include "Card/DMSCardBase.h"
#include "Card/DMSCardDefinition.h"
#include "Card/DMSCardData.h"
#include "Scenario/DMSScenarioData.h"
#include "Location/DMSLocationData.h"
#include "Location/DMSLocationBase.h"

#include "GameFramework/GameStateBase.h"
#include "Common/DMSSpawnableDataBase.h"

ADMSGameMode::ADMSGameMode()
{
	EffectHandler = CreateDefaultSubobject<UDMSEffectHandler>(TEXT("CardEffectHandler"));
	NotifyManager = CreateDefaultSubobject<UDMSNotifyManager>(TEXT("NotifyManager"));
	PhaseManagerClass= UDMSPhaseManager::StaticClass();
	SequenceManagerClass = UDMSSeqManager::StaticClass();
}

void ADMSGameMode::BeginPlay()
{
	Super::BeginPlay();

	//DMSGameState=GetGameState<ADMSGameState>();
}

ADMSGameState* ADMSGameMode::GetDMSGameState() { return GetGameState<ADMSGameState>(); }

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

ADMSCardBase* ADMSGameMode::SpawnCard_Implementation(const FDMSCardData& CardData, AActor* iOwner, const FName& DefaultContainerName)
{
	ADMSCardBase* SpawnedCard = SpawnDMSGameActor<ADMSCardBase>(CardData.CardDefinition, iOwner);
	SpawnedCard->SetActorHiddenInGame(true);

	for (auto EI : CardData.AttachedEffect)
	{
		SpawnedCard->AttachEffectInstance(EI);
	}
	auto CM = iOwner->FindComponentByClass<UDMSCardManagerComponent>();
	if (CM != nullptr)
	{
		CM->AddCardtoContainer({ SpawnedCard }, DefaultContainerName);
	}
	return SpawnedCard;
}

void ADMSGameMode::RegisterNotifyObject(TScriptInterface<IDMSEffectorInterface> Object)
{
	NotifyManager->RegisterNotifyObject(Object);
}

//void ADMSGameMode::LoadAndSpawnScenario(UDMSScenarioData* ScenarioData)
//{
//	auto& Locs = GetGameState<ADMSGameState>()->Locations;
//	for (auto& Location : ScenarioData->Locations)
//	{
//		//Location.LocationAsset.LoadSynchronous();
//		auto LocData = Location.LocationAsset.LoadSynchronous();
//		auto SpawnedLocation = SpawnDMSGameActor<ADMSLocationBase>(LocData, this, nullptr, Location.LocationOffset);
//
//		Locs.Add(SpawnedLocation);
//	}
//	
//	for (auto& LocConnection : ScenarioData->Connections)
//	{
//		if(LocConnection.StartLocationIndex < Locs.Num() && LocConnection.DestLocationIndex < Locs.Num())
//			ADMSLocationBase::ConnectLocations(Locs[LocConnection.StartLocationIndex], Locs[LocConnection.DestLocationIndex], LocConnection.bIsOneWay);
//	}
//
//}

ADMSSpawnableBase* ADMSGameMode::SpawnDMSGameActor_BP(const UDMSSpawnableDataBase* ActorData, AActor* NewOwner, ADMSLocationBase* DefaultLocation, const FTransform& inRelativeTransform)
{
	return SpawnDMSGameActor(ActorData, NewOwner, DefaultLocation, inRelativeTransform);
}

template<typename ReturnType>
ReturnType* ADMSGameMode::SpawnDMSGameActor(const UDMSSpawnableDataBase* ActorData, AActor* NewOwner, ADMSLocationBase* DefaultLocation, const FTransform& inRelativeTransform)
{
	TSubclassOf<ADMSSpawnableBase> SpawningClass = ActorData->SpawningClass == nullptr && DefaultSpawningClasses.Contains(ActorData->GetPrimaryAssetId().PrimaryAssetType) ? DefaultSpawningClasses[ActorData->GetPrimaryAssetId().PrimaryAssetType] : nullptr;

	if ( SpawningClass == nullptr || !SpawningClass->IsChildOf<ReturnType>() ) return nullptr;
	
	return Cast<ReturnType>(SpawnDMSGameActor(SpawningClass, ActorData, NewOwner, DefaultLocation, inRelativeTransform));
}

ADMSSpawnableBase* ADMSGameMode::SpawnDMSGameActor(const TSubclassOf<ADMSSpawnableBase>& SpawningClass, const UDMSSpawnableDataBase* ActorData, AActor* NewOwner, ADMSLocationBase* DefaultLocation, const FTransform& inRelativeTransform)
{
	FActorSpawnParameters Params;
	Params.Owner = NewOwner == nullptr ? this : NewOwner;
	ADMSSpawnableBase* SpawnedActor = GetWorld()->SpawnActor<ADMSSpawnableBase>(SpawningClass, Params);
	//SpawnedActor->Rename(nullptr, NewOwner == nullptr ? this : NewOwner);
	SpawnedActor->Initialize(ActorData);

	// Support DMS Common interface
	if (SpawnedActor->Implements<UDMSLocatableInterface>() && DefaultLocation != nullptr)
	{
		TScriptInterface<IDMSLocatableInterface> Locatable = SpawnedActor;

		ADMSLocationBase::MoveActorToDMSLocation(DefaultLocation, SpawnedActor);
	}

	if (SpawnedActor->Implements<UDMSEffectorInterface>())
	{
		TScriptInterface<IDMSEffectorInterface> Effector = SpawnedActor;
		NotifyManager->RegisterNotifyObject(Effector);
	}
	SpawnedActor->AddActorLocalTransform(inRelativeTransform);
	//SpawnedActor->SetActorRelativeRotation(inRelativeTransform.GetRotation());
	//SpawnedActor->SetActorRelativeScale3D(inRelativeTransform.GetScale3D());
	//SpawnedActor->AddActorLocalOffset(inRelativeTransform.GetLocation());

	return SpawnedActor;
}

ADMSSpawnableBase* ADMSGameMode::SpawnDMSGameActor(const UDMSSpawnableDataBase* ActorData, AActor* NewOwner, ADMSLocationBase* DefaultLocation, const FTransform& inRelativeTransform)
{
	FActorSpawnParameters param;
	UClass* SpawningClass = ActorData->SpawningClass == nullptr && DefaultSpawningClasses.Contains(ActorData->GetPrimaryAssetId().PrimaryAssetType) ? DefaultSpawningClasses[ActorData->GetPrimaryAssetId().PrimaryAssetType] : nullptr;

	if (SpawningClass==nullptr) {DMS_LOG_SIMPLE(TEXT("No Default Spawning Class")); return nullptr; }

	return SpawnDMSGameActor(SpawningClass, ActorData, NewOwner, DefaultLocation, inRelativeTransform);
}

