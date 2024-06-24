// Copyright JeongWoo Lee


#include "Location/DMSLocationBase.h"
#include "Scenario/DMSScenarioData.h"
#include "Location/DMSLocationData.h"
#include "Card/DMSCardManagerComponent.h"
#include "Common/DMSGameTags.h"
#include "Effect/DMSEIManagerComponent.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Attribute/DMSAttributeValue_Boolean.h"

ADMSLocationBase::ADMSLocationBase(const FObjectInitializer& ObjectInitializer):ADMSEffectorActorBase(ObjectInitializer)
{
	ContainerManagerComponent = CreateDefaultSubobject<UDMSContainerManagerComponent>(TEXT("ContainerManagerComponent"));
	AttributeComponent = CreateDefaultSubobject<UDMSAttributeComponent>(TEXT("AttributesComponent"));
	AttributeComponent->SetIsReplicated(true);
}


bool ADMSLocationBase::CanPlayerLeave() const
{
	return (LocationStateFlag & EDMSLocationState::LS_BlockPlayerLeave) == EDMSLocationState::LS_Default;

}

TArray<TScriptInterface<IDMSLocatableInterface>> ADMSLocationBase::GetActorsOnLocation()
{
	TArray<TScriptInterface<IDMSLocatableInterface>> rv;
	auto Cont = ContainerManagerComponent->SearchContainer(FGameplayTag::RequestGameplayTag("Field.Arkham.Location"));
	if(Cont)
		for(auto& Loctb : Cont->GetObjectsRef())
			if(Loctb->Implements<UDMSLocatableInterface>()) rv.Add(Loctb);		
	return rv;
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
	// 컨테이너의 이벤트로 옮겨짐
	//auto CurrentContainer = Spawnable->GetOwningContainer();
	//ADMSLocationBase* CurrentLocation = CurrentContainer ? CurrentContainer->GetTypedOuter<ADMSLocationBase>() : nullptr;
	//if(CurrentLocation != Dest) 
	//{
	//	if(CurrentLocation) CurrentLocation->OnActorLeaved(Locatable);
	//	Dest->OnActorEntered(Locatable);
	//}
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
	if(LocContainerClass)
	ContainerManagerComponent->ConstructContainer(FGameplayTag::RequestGameplayTag("Field.Arkham.Location"),LocContainerClass);
}

void ADMSLocationBase::OnInitialized_Implementation()
{
	Super::OnInitialized_Implementation();

	if (!OriginalData->IsA<UDMSLocationData>()) return;

	auto LocData = Cast<UDMSLocationData>(OriginalData);
	
	EIManagerComponent->SetupOwnEffect(LocData->LocationEffect, TAG_DMS_EffectType_Effect);

	for(auto& Attribute : LocData->LocationAttributes)
		AttributeComponent->GenerateAndSetAttribute(Attribute.DefaultTag, Attribute.DefaultValue,true);

	auto VisitedAttribute = AttributeComponent->MakeAttribute(FGameplayTag::RequestGameplayTag("Attribute.Arkham.Location.Revealed").GetSingleTagContainer(),UDMSAttributeValue_Boolean::StaticClass(),true);
	//Cast<UDMSAttributeValue_Boolean>(VisitedAttribute->AttributeValue)->SetValue(false);
}

void ADMSLocationBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADMSLocationBase, ContainerManagerComponent);
}
