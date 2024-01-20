// Fill out your copyright notice in the Description page of Project Settings.


#include "Card/DMSCardManagerComponent.h"
#include "Card/DMSCardBase.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UDMSCardManagerComponent::UDMSCardManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Containers.Add(NAME_None, CreateDefaultSubobject<UDMSCardContainerComponent>(TEXT("NoneContainer")));

	// Now supported ( had to use OnComponentCreated before )
	//Deck=CreateDefaultSubobject<UDMSCardContainerComponent>(TEXT("Deck"));
	//Hand = CreateDefaultSubobject<UDMSCardContainerComponent>(TEXT("Hand"));
	//PlayArea = CreateDefaultSubobject<UDMSCardContainerComponent>(TEXT("PlayArea"));
	//DiscardPile = CreateDefaultSubobject<UDMSCardContainerComponent>(TEXT("DiscardPile"));
	// ...
}


void UDMSCardManagerComponent::MigrateCard(UDMSCardContainerComponent* Origin, uint16 OrgIdx, uint16 Num, UDMSCardContainerComponent* Dest, uint16 DestIdx)
{
	Dest->Insert(Origin->PopAt(OrgIdx,Num),DestIdx);
}

void UDMSCardManagerComponent::MigrateCard(ADMSCardBase* Card, UDMSCardContainerComponent* Dest, uint16 DestIdx)
{
	Card->GetOwningContainer()->Remove(Card);
	Dest->Insert({Card}, DestIdx);
}

void UDMSCardManagerComponent::MigrateCard(TArray<ADMSCardBase*> Cards, UDMSCardContainerComponent* Dest, uint16 DestIdx)
{
	for (auto Card:Cards) Card->GetOwningContainer()->Remove(Card);
	Dest->Insert(Cards, DestIdx);
}

void UDMSCardManagerComponent::AddCardtoContainer(TArray<ADMSCardBase*> Cards, const FName& ContainerName)
{
	if(!Containers.Contains(ContainerName)) Containers[NAME_None]->Insert(Cards,0);
	else Containers[ContainerName]->Insert(Cards,0);
}

TArray<ADMSCardBase*> UDMSCardManagerComponent::GetAllCards()
{
	TArray<ADMSCardBase*> rv;

	for (auto& Container : Containers)
	{
		rv.Append(Container.Value->GetCards());
	}
	return rv;
}


void UDMSCardManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

UDMSCardContainerComponent* UDMSCardManagerComponent::SearchContainer(const FName& ContainerName)
{
	return Containers.Contains(ContainerName) ? Containers[ContainerName] : nullptr;
}

void UDMSCardManagerComponent::ConstructContainer(const FName& ContainerName, TSubclassOf<UDMSCardContainerComponent> ContainerClass)
{
	if (Containers.Contains(ContainerName)) return;

	//DMS_LOG_SCREEN(TEXT("%s"), *ContainerClass->GetName());

	UDMSCardContainerComponent* NewContainer = NewObject<UDMSCardContainerComponent>(this, ContainerClass, ContainerName);
	
	Containers.Add(ContainerName, NewContainer);
	NewContainer->ContainerName = ContainerName;
	NewContainer->SetIsReplicated(true);
	NewContainer->RegisterComponent();
}

