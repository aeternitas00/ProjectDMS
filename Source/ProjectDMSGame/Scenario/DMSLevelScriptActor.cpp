// Fill out your copyright notice in the Description page of Project Settings.


#include "Scenario/DMSLevelScriptActor.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "GameModes/DMSGameMode.h"
#include "GameModes/DMSGameState.h"
#include "Location/DMSLocationBase.h"
#include "Notify/DMSNotifyManager.h"
#include "Effect/DMSEffectorInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Effect/DMSEIManagerComponent.h"

ADMSLevelScriptActor::ADMSLevelScriptActor(/*const FObjectInitializer& ObjectInitializer*/)//:Super(ObjectInitializer)
{
	EffectManagerComponent = CreateDefaultSubobject<UDMSEIManagerComponent>("EffectManagerComponent");

}


void ADMSLevelScriptActor::InitializeDMSGame(/*UPARAM(ref)TArray<ADMSLocationBase*>& Locations*/)
{
	auto GM = UDMSCoreFunctionLibrary::GetDMSGameMode();
	check(GM);

	GM->RegisterNotifyObject(this);

	auto GS = GM->GetDMSGameState();
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
		GM->RegisterNotifyObject(Actor);
	
	TArray<FGameplayTag> Keys;
	ScenarioRules.GetKeys(Keys);
	for (auto& Key : Keys)
	EffectManagerComponent->SetupOwnEffect(ScenarioRules[Key],Key);

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
//	EffectManagerComponent->AttachEffectInstance(EI);
//}
//
//bool ADMSLevelScriptActor::OnNotifyReceived(TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*>& ResponsedObjects, bool iChainable, UDMSSequence* Seq, UObject* SourceTweak)
//{
//	return EffectManagerComponent->OnNotifyReceived(ResponsedObjects, iChainable, Seq, this);
//}

UDMSEffectSet* ADMSLevelScriptActor::GetOwningEffectSet(const FGameplayTag& iSetName)
{ 
	return ScenarioRules.Contains(iSetName) ? ScenarioRules[iSetName] : nullptr;
}
