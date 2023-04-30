// Fill out your copyright notice in the Description page of Project Settings.


#include "Location/DMSLocationBase.h"
#include "Scenario/DMSScenarioData.h"
#include "Location/DMSLocationData.h"

ADMSLocationBase::ADMSLocationBase():ADMSEffectorActorBase()
{
}

void ADMSLocationBase::InitializeLocation_Native(const FDMSScenarioLocatingData& iLocData)
{
	LocationData = iLocData.LocationAsset;
	SetActorTransform(iLocData.LocationOffset);
	InitializeLocation(iLocData);
}

void ADMSLocationBase::MoveActorToDMSLocation(ADMSLocationBase* Dest,const TScriptInterface<IDMSLocatableInterface>& Locatable)
{
	UObject* Obj = Locatable.GetObject();
	//IDMSLocatableInterface* ThisInterface = Cast<IDMSLocatableInterface>(Obj);
	
	auto CurrentLocation = Locatable->Execute_GetCurrentLocation(Obj);

	if (CurrentLocation != nullptr)
	{
		CurrentLocation->ActorsOnLocation.Remove(Locatable);
		CurrentLocation->OnActorLeaved(Locatable);
	}

	Locatable->Execute_SetCurrentLocation(Obj, Dest);

	Dest->OnActorEntered(Locatable);
	Dest->ActorsOnLocation.Add(Locatable);
}

void ADMSLocationBase::ConnectLocations(ADMSLocationBase* Start, ADMSLocationBase* Dest, const bool& IsOneWay)
{
	Start->ConnectingLocations.Add(Dest);
	if (!IsOneWay)Dest->ConnectingLocations.Add(Start);
}
