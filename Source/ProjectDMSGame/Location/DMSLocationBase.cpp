// Fill out your copyright notice in the Description page of Project Settings.


#include "Location/DMSLocationBase.h"
#include "Scenario/DMSScenarioData.h"
#include "Location/DMSLocationData.h"

ADMSLocationBase::ADMSLocationBase(const FObjectInitializer& ObjectInitializer):ADMSEffectorActorBase(ObjectInitializer)
{
	//ChildSlot=CreateDefaultSubobject< USceneComponent>("ChildSlot");
}

//void ADMSLocationBase::Initialize_Implementation(const UDMSSpawnableDataBase* inData)
//{
//	LocationData = Cast<UDMSLocationData>(inData);
//}

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
