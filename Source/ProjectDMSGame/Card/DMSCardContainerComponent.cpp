// Fill out your copyright notice in the Description page of Project Settings.


#include "Card/DMSCardContainerComponent.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Misc/DateTime.h"

// Called when the game starts
void UDMSCardContainerComponent::BeginPlay()
{
	Super::BeginPlay();

	//DMS_LOG_SCREEN(TEXT("%s"), *GetClass()->GetName());
}


TArray<ADMSCardBase*> UDMSCardContainerComponent::PopAt(uint16 Idx)
{
	ADMSCardBase* Card = CardList.Cards[Idx];

	if (Card) CardList.Cards.RemoveAt(Idx);

	OnContainerRemoved({Card});

	return {Card};
}

TArray<ADMSCardBase*> UDMSCardContainerComponent::PopAt(uint16 Idx, uint16 Num)
{
	TArray<ADMSCardBase*> Rv;

	//CardList.Cards.
	
	for ( int i = Idx; Num+Idx > i ;i++)
	{
		ADMSCardBase* Card = CardList.Cards[i];

		if (Card) { CardList.Cards.RemoveAt(i); Rv.Add(Card); }
		else break;
	}
	OnContainerRemoved(Rv);
	return Rv;
}

void UDMSCardContainerComponent::Insert(TArray<ADMSCardBase*> iContainer, uint16 Idx)
{
	uint16 Dest = Idx;
	if (CardList.Cards.Num() < Dest) Dest= CardList.Cards.Num();
	for (auto Card : iContainer) Card->SetOwningContainer(this);
	CardList.Cards.Insert(iContainer,Dest);
	OnContainerAdded(iContainer);
}

void UDMSCardContainerComponent::Remove(ADMSCardBase* iCard)
{
	CardList.Cards.Remove(iCard);
	OnContainerRemoved({iCard});
}

void UDMSCardContainerComponent::Remove(TArray<ADMSCardBase*> iCards) 
{
	for (auto iCard : iCards) CardList.Cards.Remove(iCard);
	OnContainerRemoved(iCards);
}

TArray<ADMSCardBase*> UDMSCardContainerComponent::GetTopNCards(int Num)
{
	if (CardList.Cards.Num() == 0 || Num <= 0) return TArray<ADMSCardBase*>();
	if (CardList.Cards.Num() <= Num) Num = CardList.Cards.Num();
	return TArray<ADMSCardBase*>(&CardList.Cards.GetData()[CardList.Cards.Num() - Num], Num);
}

void UDMSCardContainerComponent::ShuffleTopNCards(int Num)
{
	FRandomStream Stream(FDateTime::Now().GetTicks());
	UDMSCoreFunctionLibrary::ShuffleArray(Stream,CardList.Cards,Num);
}
