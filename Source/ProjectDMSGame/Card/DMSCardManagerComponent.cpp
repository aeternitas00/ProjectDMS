// Fill out your copyright notice in the Description page of Project Settings.


#include "Card/DMSCardManagerComponent.h"
#include "Card/DMSCardBase.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UDMSContainerManagerComponent::UDMSContainerManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	Containers.Add(FGameplayTag::EmptyTag, CreateDefaultSubobject<UDMSSpawnableContainerComponent>(TEXT("NoneContainer")));
}


void UDMSContainerManagerComponent::MigrateObjects(UDMSSpawnableContainerComponent* Origin, uint16 OrgIdx, uint16 Num, UDMSSpawnableContainerComponent* Dest, uint16 DestIdx)
{
	Dest->Insert(Origin->PopAt(OrgIdx,Num),DestIdx);
}

void UDMSContainerManagerComponent::MigrateObjects(ADMSSpawnableBase* Card, UDMSSpawnableContainerComponent* Dest, uint16 DestIdx)
{
	auto SourceCont = Card->GetOwningContainer();
	if(SourceCont)SourceCont->Remove(Card);
	Dest->Insert({Card}, DestIdx);
}

void UDMSContainerManagerComponent::MigrateObjects(TArray<ADMSSpawnableBase*> Cards, UDMSSpawnableContainerComponent* Dest, uint16 DestIdx)
{
	for (auto Card:Cards) Card->GetOwningContainer()->Remove(Card);
	Dest->Insert(Cards, DestIdx);
}

void UDMSContainerManagerComponent::AddObjectsToContainer(TArray<ADMSSpawnableBase*> Objects, const FGameplayTag& ContainerName)
{
	if(!Containers.Contains(ContainerName)) Containers[FGameplayTag::EmptyTag]->Insert(Objects,0);
	else Containers[ContainerName]->Insert(Objects,0);
}

TArray<ADMSSpawnableBase*> UDMSContainerManagerComponent::GetAllObjects()
{
	TArray<ADMSSpawnableBase*> rv;

	for (auto& Container : Containers)
	{
		rv.Append(Container.Value->GetObjects());
	}
	return rv;
}

void UDMSContainerManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	for (auto& ContainerDef : ContainerTypes) {
		ConstructContainer(ContainerDef.Key, ContainerDef.Value);
	}
}


void UDMSContainerManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

UDMSSpawnableContainerComponent* UDMSContainerManagerComponent::SearchContainer(const FGameplayTag& ContainerName)
{
	return Containers.Contains(ContainerName) ? Containers[ContainerName] : nullptr;
}

void UDMSContainerManagerComponent::ConstructContainer(const FGameplayTag& ContainerName, TSubclassOf<UDMSSpawnableContainerComponent> ContainerClass)
{
	if (Containers.Contains(ContainerName)) return;

	//DMS_LOG_SCREEN(TEXT("%s"), *ContainerClass->GetName());

	UDMSSpawnableContainerComponent* NewContainer = NewObject<UDMSSpawnableContainerComponent>(this, ContainerClass, ContainerName.GetTagName());
	
	Containers.Add(ContainerName, NewContainer);
	NewContainer->ContainerName = ContainerName;
	NewContainer->SetIsReplicated(true);
	NewContainer->RegisterComponent();
}

