// Fill out your copyright notice in the Description page of Project Settings.


#include "Location/DMSLocationBase.h"
#include "Scenario/DMSScenarioData.h"
#include "Location/DMSLocationData.h"
#include "Effect/DMSEIManagerComponent.h"

ADMSLocationBase::ADMSLocationBase(const FObjectInitializer& ObjectInitializer):ADMSEffectorActorBase(ObjectInitializer)
{
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
	UObject* Obj = Locatable.GetObject();
	
	auto CurrentLocation = Locatable->Execute_GetCurrentLocation(Obj);

	if (CurrentLocation != nullptr)
	{
		CurrentLocation->ActorsOnLocation.Remove(Locatable);
		CurrentLocation->OnActorLeaved(Locatable);
	}

	Locatable->Execute_SetCurrentLocation(Obj, Dest);

	Dest->OnActorEntered(Locatable);
	Dest->ActorsOnLocation.Add(Locatable);

	return true;
}

void ADMSLocationBase::ConnectLocations(ADMSLocationBase* Start, ADMSLocationBase* Dest, const bool& IsOneWay)
{
	Start->ConnectingLocations.Add(Dest);
	if (!IsOneWay)Dest->ConnectingLocations.Add(Start);

	
}

void ADMSLocationBase::Initialize_Implementation(const UDMSSpawnableDataBase* NewData) 
{
	Super::Initialize_Implementation(NewData);
	
	if (!OriginalData->IsA<UDMSLocationData>()) return;

	auto LocData = Cast<UDMSLocationData>(OriginalData);
	
	EffectManagerComponent->SetupOwnEffect(LocData->LocationEffect, TAG_DMS_EffectType_Effect);
}
