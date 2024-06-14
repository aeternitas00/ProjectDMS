// Copyright Epic Games, Inc. All Rights Reserved.


#include "DMSGameMode.h"
#include "System/DMSSession.h"
#include "Player/DMSPlayerState.h"
#include "GameModes/DMSGameState.h"

#include "Attribute/DMSAttributeComponent.h"
#include "Attribute/DMSAttributeValue_Boolean.h"
#include "Attribute/DMSAttributeValue_Object.h"

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

#include "Effect/DMSEIManagerComponent.h"

#include "GameFramework/GameStateBase.h"
#include "Common/DMSSpawnableDataBase.h"

ADMSGameMode::ADMSGameMode() : ADMSGameModeBase()
{

}

void ADMSGameMode::BeginPlay()
{
	Super::BeginPlay();

}


void ADMSGameMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}


ADMSCardBase* ADMSGameMode::SpawnCard_Implementation(const FDMSCardData& CardData, int32 OwnerID, const FGameplayTag& DefaultContainerName)
{
	ADMSCardBase* SpawnedCard = SpawnDMSGameActor<ADMSCardBase>(CardData.CardDefinition, OwnerID);
	SpawnedCard->SetActorHiddenInGame(true);

	for (auto EI : CardData.AttachedEffect)
	{
		SpawnedCard->GetEffectorManagerComponent()->AttachEffectInstance(EI);
	}
	auto CM = GetGameState<ADMSGameState>()->FindPlayerFromId(OwnerID)->FindComponentByClass<UDMSContainerManagerComponent>();
	if (CM != nullptr)
	{
		CM->AddObjectsToContainer({ SpawnedCard }, DefaultContainerName );
	}
	return SpawnedCard;
}



void ADMSGameMode::SetupDMSGame_Implementation()
{
	ADMSGameState* GS = Cast<ADMSGameState>(GetDMSGameState());
	check(GS);
	
	//NumReadyPlayer=0;

	if (GS->PlayerArray.Num() != 0)
	{
		GS->SetLeaderPlayer(GetDMSGameState()->PlayerArray[0]->GetPlayerId());
	}

	// Setup game system thingys.
	GS->RegisterNotifyObject(GS);
	GS->SetupDefaults();

	auto CurrentLSA = Cast<ADMSLevelScriptActor>(GetWorld()->GetLevelScriptActor());
	if (CurrentLSA != nullptr)
		CurrentLSA->InitializeDMSGame();

	// ...
}

void ADMSGameMode::PlayerReady()
{	
	ADMSGameState* GS = Cast<ADMSGameState>(GetDMSGameState());
	check(GS);
	NumReadyPlayer++;
	if ( GS->TestPlayerNum == NumReadyPlayer )
	{	
		auto CurrentLSA = Cast<ADMSLevelScriptActor>(GetWorld()->GetLevelScriptActor());
		auto StartingLocation = CurrentLSA->GetStartingLocations()[0];

		auto StartingLocationAtt = GS->GetComponentByClass<UDMSAttributeComponent>()->MakeAttribute(FGameplayTag::RequestGameplayTag("Attribute.Arkham.Scenario.StartingLocation").GetSingleTagContainer(),UDMSAttributeValue_Object::StaticClass(),true);
		
		Cast<UDMSAttributeValue_Object>(StartingLocationAtt->AttributeValue)->SetValue({StartingLocation});

		for (auto PS : GS->GetDMSPlayers()) {

			auto PlayerID = PS->GetPlayerId();
			auto CharacterAsset_Soft = UKismetSystemLibrary::GetSoftObjectReferenceFromPrimaryAssetId(PS->PlayerCharacterData.AssetID);
			auto CharacterAsset = Cast<UDMSCharacterDefinition>(UKismetSystemLibrary::LoadAsset_Blocking(CharacterAsset_Soft));
			if (CharacterAsset)
				PS->CharacterRef = SpawnDMSGameActor<ADMSCharacterBase>(CharacterAsset, PlayerID, StartingLocation, FTransform(FVector(- 10, 0, 0)));

			// Spawn cards
			// TODO :: INITIAL SPAWN TO NONE CONTAINER AND DO ADDITIONAL MOVEMENT WITH OTHER METHOD FOLLOWS GAME RULE.
			for (auto CardData : PS->OriginalCardDatas)
				SpawnCard(CardData, PlayerID, DefaultCardSpawnContainer);

			if (PS->SearchContainer(DefaultCardSpawnContainer))
				PS->SearchContainer(DefaultCardSpawnContainer)->ShuffleTopNObjects();
		}

		OnAllPlayerReady();
	}
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

	return SpawnedActor;
}

ADMSSpawnableBase* ADMSGameMode::SpawnDMSGameActor(const UDMSSpawnableDataBase* ActorData, int32 OwnerID, ADMSLocationBase* DefaultLocation, const FTransform& inRelativeTransform)
{
	FActorSpawnParameters param;
	UClass* SpawningClass = ActorData->SpawningClass == nullptr && DefaultSpawningClasses.Contains(ActorData->GetPrimaryAssetId().PrimaryAssetType) ? DefaultSpawningClasses[ActorData->GetPrimaryAssetId().PrimaryAssetType] : nullptr;

	if (SpawningClass == nullptr) { DMS_LOG_SIMPLE(TEXT("No Default Spawning Class")); return nullptr; }

	return SpawnDMSGameActor(SpawningClass, ActorData, OwnerID, DefaultLocation, inRelativeTransform);

}

