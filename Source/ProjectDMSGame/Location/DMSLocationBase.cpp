// Fill out your copyright notice in the Description page of Project Settings.


#include "Location/DMSLocationBase.h"
#include "Scenario/DMSScenarioData.h"
#include "Location/DMSLocationData.h"
#include "Card/DMSCardManagerComponent.h"
#include "Common/DMSGameTags.h"
#include "Effect/DMSEIManagerComponent.h"

ADMSLocationBase::ADMSLocationBase(const FObjectInitializer& ObjectInitializer):ADMSEffectorActorBase(ObjectInitializer)
{
	ContainerManagerComponent = CreateDefaultSubobject<UDMSContainerManagerComponent>(TEXT("ContainerManagerComponent"));

	//ChildSlot=CreateDefaultSubobject< USceneComponent>("ChildSlot");
}


bool ADMSLocationBase::CanPlayerLeave() const
{
	return (LocationStateFlag & EDMSLocationState::LS_BlockPlayerLeave) == EDMSLocationState::LS_Default;

}

bool ADMSLocationBase::CanPlayerEnter() const 
{
	return (LocationStateFlag & EDMSLocationState::LS_BlockPlayerEnter) == EDMSLocationState::LS_Default;
}

bool ADMSLocationBase::MoveActorToDMSLocation(ADMSLocationBase* Dest,const TScriptInterface<IDMSLocatableInterface>& Locatable)
{
	//UObject* Obj = Locatable.GetObject();
	//
	//auto CurrentLocation = Locatable->Execute_GetCurrentLocation(Obj);

	//if (CurrentLocation != nullptr)
	//{
	//	CurrentLocation->ActorsOnLocation.Remove(Locatable);
	//	CurrentLocation->OnActorLeaved(Locatable);
	//}

	//Locatable->Execute_SetCurrentLocation(Obj, Dest);

	//Dest->OnActorEntered(Locatable);
	//Dest->ActorsOnLocation.Add(Locatable);

	//return true;

	// New

	auto DestContainer = Dest->GetComponentByClass<UDMSContainerManagerComponent>()->SearchContainer(FGameplayTag::RequestGameplayTag("Field.Arkham.Location"));

	check(DestContainer);

	auto Spawnable = Cast<ADMSSpawnableBase>(Locatable.GetObject());

	if (!Spawnable) return false;

	UDMSContainerManagerComponent::MigrateObjects(Spawnable, DestContainer, 0);

	return true;
}

void ADMSLocationBase::ConnectLocations(ADMSLocationBase* Start, ADMSLocationBase* Dest, const bool& IsOneWay)
{
	Start->ConnectingLocations.Add(Dest);
	if (!IsOneWay)Dest->ConnectingLocations.Add(Start);	
}

void ADMSLocationBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ContainerManagerComponent->ConstructContainer(FGameplayTag::RequestGameplayTag("Field.Arkham.Location"),LocContainerClass);
}

void ADMSLocationBase::OnInitialized_Implementation()
{
	Super::OnInitialized_Implementation();

	if (!OriginalData->IsA<UDMSLocationData>()) return;

	auto LocData = Cast<UDMSLocationData>(OriginalData);
	
	EIManagerComponent->SetupOwnEffect(LocData->LocationEffect, TAG_DMS_EffectType_Effect);
}
