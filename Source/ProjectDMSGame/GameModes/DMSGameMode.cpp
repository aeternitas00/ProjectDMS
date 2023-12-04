// Copyright Epic Games, Inc. All Rights Reserved.


#include "DMSGameMode.h"
#include "Player/DMSPlayerState.h"
#include "GameModes/DMSGameState.h"

#include "Scenario/DMSLevelScriptActor.h"
#include "Sequence/DMSSeqManager.h"
#include "Notify/DMSNotifyManager.h"
#include "Phase/DMSPhaseManager.h"
#include "Effect/DMSEffectHandler.h"

#include "Player/DMSPlayerController.h"
#include "Effect/DMSEffectorInterface.h"

#include "Character/DMSCharacterDefinition.h"
#include "Card/DMSCardManagerComponent.h"
#include "Card/DMSCardBase.h"
#include "Card/DMSCardDefinition.h"
#include "Card/DMSCardData.h"
#include "Scenario/DMSScenarioData.h"
#include "Location/DMSLocationData.h"
#include "Location/DMSLocationBase.h"

#include "GameFramework/GameStateBase.h"
#include "Common/DMSSpawnableDataBase.h"

ADMSGameMode::ADMSGameMode() : ADMSGameModeBase()
{
	//EffectHandler = CreateDefaultSubobject<UDMSEffectHandler>(TEXT("CardEffectHandler"));
	//NotifyManager = CreateDefaultSubobject<UDMSNotifyManager>(TEXT("NotifyManager"));
	//PhaseManagerClass= UDMSPhaseManager::StaticClass();
	//SequenceManagerClass = UDMSSeqManager::StaticClass();
}

void ADMSGameMode::BeginPlay()
{
	Super::BeginPlay();
	bool rv = GetDMSGameState()->IsValidLowLevelFast();
	if (rv && GetDMSGameState()->PlayerArray.Num() != 0)
	{
		GetDMSGameState()->SetLeaderPlayer(GetDMSGameState()->PlayerArray[0]->GetPlayerId());
	}
	//DMSGameState=GetGameState<ADMSGameState>();
}

//ADMSGameState* ADMSGameMode::GetDMSGameState() { return GetGameState<ADMSGameState>(); }

void ADMSGameMode::PreInitializeComponents()
{
	//if(PhaseManagerClass->IsValidLowLevelFast()){
	//	PhaseManager = NewObject<UDMSPhaseManager>(this, PhaseManagerClass,TEXT("PhaseManager"));
	//	PhaseManager->RegisterComponent();
	//}
	//if (SequenceManagerClass->IsValidLowLevelFast()) {
	//	SequenceManager = NewObject<UDMSSeqManager>(this, SequenceManagerClass, TEXT("SequenceManager"));
	//	SequenceManager->RegisterComponent();
	//}
	Super::PreInitializeComponents();
}


ADMSCardBase* ADMSGameMode::SpawnCard_Implementation(const FDMSCardData& CardData, int32 OwnerID, const FName& DefaultContainerName)
{
	ADMSCardBase* SpawnedCard = SpawnDMSGameActor<ADMSCardBase>(CardData.CardDefinition, OwnerID);
	SpawnedCard->SetActorHiddenInGame(true);

	for (auto EI : CardData.AttachedEffect)
	{
		SpawnedCard->AttachEffectInstance(EI);
	}
	auto CM = GetGameState<ADMSGameState>()->FindPlayerFromId(OwnerID)->FindComponentByClass<UDMSCardManagerComponent>();
	if (CM != nullptr)
	{
		CM->AddCardtoContainer({ SpawnedCard }, DefaultContainerName );
	}
	return SpawnedCard;
}

//void ADMSGameMode::RegisterNotifyObject(TScriptInterface<IDMSEffectorInterface> Object)
//{
//	NotifyManager->RegisterNotifyObject(Object);
//}

void ADMSGameMode::SetupDMSGame_Implementation()
{
	ADMSGameState* GS = Cast<ADMSGameState>(GetDMSGameState());
	check(GS);

	// Setup game system thingys.
	GS->RegisterNotifyObject(GS);
	GS->SetupDefaults();

	auto CurrentLSA = Cast<ADMSLevelScriptActor>(GetWorld()->GetLevelScriptActor());
	if (CurrentLSA != nullptr)
		CurrentLSA->InitializeDMSGame();

	auto StartingLocation = CurrentLSA->GetStartingLocations()[0];

	// Setup Player thingys.
	for (auto PC : GS->GetDMSPlayerControllers()) {

		auto PS = PC->GetPlayerState<ADMSPlayerState>();
		if (PS==nullptr) continue;
		auto PlayerID = PS->GetPlayerId();

		PC->CreateHUDWidgets();
		PS->SetupDefaults();
		PC->SetupHUDWidgets();
		
		// TODO :: USE RPC WITH SYNC
		PC->LoadClientSaveGame("TestSlot",0);

		// IF SERVER RECEIVED DATA [ MIGRATE TO OTHER FUNCTION FOR SYNC ]

		// Spawn character
		auto CharacterAsset_Soft = UKismetSystemLibrary::GetSoftObjectReferenceFromPrimaryAssetId(PS->PlayerCharacterData.AssetID);
		auto CharacterAsset = Cast<UDMSCharacterDefinition>(UKismetSystemLibrary::LoadAsset_Blocking(CharacterAsset_Soft));
		PS->CharacterRef = SpawnDMSGameActor<ADMSCharacterBase>(CharacterAsset, PlayerID,StartingLocation,FTransform(FVector(- 10, 0, 0)));

		// Spawn cards

		for (auto CardData : PS->OriginalCardDatas)
			SpawnCard(CardData, PlayerID,TEXT("Deck"));
		
		PS->SearchContainer(TEXT("Deck"))->ShuffleTopNCards();
	}

	// ...
}

ADMSSpawnableBase* ADMSGameMode::SpawnDMSGameActor_ID(const UDMSSpawnableDataBase* ActorData, int32 OwnerID, ADMSLocationBase* DefaultLocation, const FTransform& inRelativeTransform)
{
	return SpawnDMSGameActor(ActorData, OwnerID, DefaultLocation, inRelativeTransform);
}


ADMSSpawnableBase* ADMSGameMode::SpawnDMSGameActor(const TSubclassOf<ADMSSpawnableBase>& SpawningClass, const UDMSSpawnableDataBase* ActorData, int32 OwnerID, ADMSLocationBase* DefaultLocation, const FTransform& inRelativeTransform)
{
	FActorSpawnParameters Params;
	Params.Owner = this;
	ADMSSpawnableBase* SpawnedActor = GetWorld()->SpawnActor<ADMSSpawnableBase>(SpawningClass, Params);
	//SpawnedActor->Rename(nullptr, NewOwner == nullptr ? this : NewOwner);
	SpawnedActor->SetOwnerID(OwnerID);
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
		GetDMSGameState()->GetNotifyManager()->RegisterNotifyObject(Effector);
	}
	SpawnedActor->AddActorLocalTransform(inRelativeTransform);
	//SpawnedActor->SetActorRelativeRotation(inRelativeTransform.GetRotation());
	//SpawnedActor->SetActorRelativeScale3D(inRelativeTransform.GetScale3D());
	//SpawnedActor->AddActorLocalOffset(inRelativeTransform.GetLocation());

	return SpawnedActor;
}

ADMSSpawnableBase* ADMSGameMode::SpawnDMSGameActor(const UDMSSpawnableDataBase* ActorData, int32 OwnerID, ADMSLocationBase* DefaultLocation, const FTransform& inRelativeTransform)
{
	FActorSpawnParameters param;
	UClass* SpawningClass = ActorData->SpawningClass == nullptr && DefaultSpawningClasses.Contains(ActorData->GetPrimaryAssetId().PrimaryAssetType) ? DefaultSpawningClasses[ActorData->GetPrimaryAssetId().PrimaryAssetType] : nullptr;

	if (SpawningClass == nullptr) { DMS_LOG_SIMPLE(TEXT("No Default Spawning Class")); return nullptr; }

	return SpawnDMSGameActor(SpawningClass, ActorData, OwnerID, DefaultLocation, inRelativeTransform);

}

