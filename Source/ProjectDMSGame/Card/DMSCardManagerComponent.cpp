// Fill out your copyright notice in the Description page of Project Settings.


#include "Card/DMSCardManagerComponent.h"
#include "Card/DMSCardBase.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UDMSCardManagerComponent::UDMSCardManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Containers.Add(FGameplayTag::EmptyTag, CreateDefaultSubobject<UDMSCardContainerComponent>(TEXT("NoneContainer")));
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

void UDMSCardManagerComponent::AddCardtoContainer(TArray<ADMSCardBase*> Cards, const FGameplayTag& ContainerName)
{
	if(!Containers.Contains(ContainerName)) Containers[FGameplayTag::EmptyTag]->Insert(Cards,0);
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

UDMSCardContainerComponent* UDMSCardManagerComponent::SearchContainer(const FGameplayTag& ContainerName)
{
	return Containers.Contains(ContainerName) ? Containers[ContainerName] : nullptr;
}

void UDMSCardManagerComponent::ConstructContainer(const FGameplayTag& ContainerName, TSubclassOf<UDMSCardContainerComponent> ContainerClass)
{
	if (Containers.Contains(ContainerName)) return;

	//DMS_LOG_SCREEN(TEXT("%s"), *ContainerClass->GetName());

	UDMSCardContainerComponent* NewContainer = NewObject<UDMSCardContainerComponent>(this, ContainerClass, ContainerName.GetTagName());
	
	Containers.Add(ContainerName, NewContainer);
	NewContainer->ContainerName = ContainerName;
	NewContainer->SetIsReplicated(true);
	NewContainer->RegisterComponent();
}

