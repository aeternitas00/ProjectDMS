// Fill out your copyright notice in the Description page of Project Settings.


#include "Scenario/DMSLevelScriptActor.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Player/DMSPlayerState.h"
#include "Player/DMSPlayerController.h"
#include "GameModes/DMSGameModeBase.h"
#include "GameModes/DMSGameStateBase.h"
#include "Location/DMSLocationBase.h"
#include "Notify/DMSNotifyManager.h"
#include "Effect/DMSEffectorInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Effect/DMSEIManagerComponent.h"

ADMSLevelScriptActor::ADMSLevelScriptActor(/*const FObjectInitializer& ObjectInitializer*/)//:Super(ObjectInitializer)
{
	EIManagerComponent = CreateDefaultSubobject<UDMSEIManagerComponent>("EIManagerComponent");
}


void ADMSLevelScriptActor::InitializeDMSGame(/*UPARAM(ref)TArray<ADMSLocationBase*>& Locations*/)
{
	auto GM = UDMSCoreFunctionLibrary::GetDMSGameMode();
	check(GM);

	ADMSGameState* GS = Cast<ADMSGameState>(GM->GetDMSGameState());
	check(GS);

	GS->Locations = SpawnedLocations;
	for (auto& Pair : SpawnedDMSActors)
	{
		for(auto& Actor : Pair.Value.Actors)
		{
			if (Actor->Implements<UDMSLocatableInterface>())
				IDMSLocatableInterface::Execute_LocatingTo(Actor,Pair.Key);
		}
	}
	TArray<AActor*> LevelPlacedActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADMSSpawnableBase::StaticClass(), LevelPlacedActors);
	for (auto Actor : LevelPlacedActors)
	{	
		ADMSSpawnableBase* Spawnable = Cast<ADMSSpawnableBase>(Actor);
		Spawnable->Initialize(Spawnable->GetOriginalData());
	}

	UGameplayStatics::GetAllActorsWithInterface(GetWorld(),UDMSEffectorInterface::StaticClass(), LevelPlacedActors);

	for (auto Actor : LevelPlacedActors)
		GS->RegisterNotifyObject(Actor);
	
	// Duplicating LSA in same outer makes crash... 
	//PreviewDummy = DuplicateObject(this, this, FName(GetName() + TEXT("_Preview")));

	TArray<FGameplayTag> Keys;
	ScenarioRules.GetKeys(Keys);
	for (auto& Key : Keys)
		EIManagerComponent->SetupOwnEffect(ScenarioRules[Key],Key);
}

TArray<ADMSLocationBase*> ADMSLevelScriptActor::GetStartingLocations()
{
	if(bIsStartingSelectable)
		return StartingLocations;
	return { StartingLocations[rand() % StartingLocations.Num()]};
}

//
//void ADMSLevelScriptActor::AttachEffectInstance(UDMSEffectInstance* EI)
//{
//	EIManagerComponent->AttachEffectInstance(EI);
//}
//
//bool ADMSLevelScriptActor::OnNotifyReceived(TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*>& ResponsedObjects, bool iChainable, UDMSSequence* Seq, UObject* SourceTweak)
//{
//	return EIManagerComponent->OnNotifyReceived(ResponsedObjects, iChainable, Seq, this);
//}


/**
* Interfaces
*/
//virtual UObject* GetObject() override {return this;} // RENAME?

AActor* ADMSLevelScriptActor::GetOwningPlayer() { return UDMSCoreFunctionLibrary::GetDMSGameMode()->GameState; }

UDMSEffectSet* ADMSLevelScriptActor::GetOwningEffectSet(const FGameplayTag& iSetName)
{ 
	return ScenarioRules.Contains(iSetName) ? ScenarioRules[iSetName] : nullptr;
}
