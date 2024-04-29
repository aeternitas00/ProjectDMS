#include "Common/DMSSpawnableDataBase.h"
#include "Common/DMSSpawnableComponent.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "GameModes/DMSGameStateBase.h"
#include "Player/DMSPlayerStateBase.h"

FPrimaryAssetId UDMSSpawnableDataBase::GetPrimaryAssetId() const
{
	return FPrimaryAssetId();
}

void ADMSSpawnableBase::Initialize(const UDMSSpawnableDataBase* inData)
{
	if (bInitialized) return;

	OriginalData = inData; 

	ForEachComponent<UDMSSpawnableComponent>(false,[](UDMSSpawnableComponent* Comp){
		Comp->UpdateParentComponent();
	});

	bInitialized=true;

	OnInitialized();
	
	PostInitialize();
}

UDMSSpawnableContainerComponent::UDMSSpawnableContainerComponent()
{
	SetIsReplicatedByDefault(true);
}

// Called when the game starts
void UDMSSpawnableContainerComponent::BeginPlay()
{
	Super::BeginPlay();

	//DMS_LOG_SCREEN(TEXT("%s"), *GetClass()->GetName());
}


TArray<ADMSSpawnableBase*> UDMSSpawnableContainerComponent::PopAt(uint16 Idx)
{
	ADMSSpawnableBase* Card = SpawnableList.Actors[Idx];

	if (Card) SpawnableList.Actors.RemoveAt(Idx);

	OnContainerRemoved({Card});

	return {Card};
}

TArray<ADMSSpawnableBase*> UDMSSpawnableContainerComponent::PopAt(uint16 Idx, uint16 Num)
{
	TArray<ADMSSpawnableBase*> Rv;

	//CardList.Cards.

	for ( int i = Idx; Num+Idx > i ;i++)
	{
		ADMSSpawnableBase* Object = SpawnableList.Actors[i];

		if (Object) { SpawnableList.Actors.RemoveAt(i); Rv.Add(Object); }
		else break;
	}
	OnContainerRemoved(Rv);
	return Rv;
}

void UDMSSpawnableContainerComponent::Insert(TArray<ADMSSpawnableBase*> iContainer, uint16 Idx)
{
	uint16 Dest = Idx;
	if (SpawnableList.Actors.Num() < Dest) Dest= SpawnableList.Actors.Num();
	for (auto Card : iContainer) Card->SetOwningContainer(this);
	SpawnableList.Actors.Insert(iContainer,Dest);
	OnContainerAdded(iContainer);
}

void UDMSSpawnableContainerComponent::Insert(ADMSSpawnableBase* iObject, uint16 Idx)
{	
	uint16 Dest = Idx;
	if (SpawnableList.Actors.Num() < Dest) Dest= SpawnableList.Actors.Num();
	iObject->SetOwningContainer(this);
	SpawnableList.Actors.Insert(iObject,Dest);
	OnContainerAdded({iObject});
}

uint16 UDMSSpawnableContainerComponent::Find(ADMSSpawnableBase* iObject)
{
	return SpawnableList.Actors.Find(iObject);
}

void UDMSSpawnableContainerComponent::Remove(ADMSSpawnableBase* iObject)
{
	SpawnableList.Actors.Remove(iObject);
	OnContainerRemoved({iObject});
}

void UDMSSpawnableContainerComponent::Remove(TArray<ADMSSpawnableBase*> iObjects) 
{
	for (auto iObject : iObjects) SpawnableList.Actors.Remove(iObject);
	OnContainerRemoved(iObjects);
}

TArray<ADMSSpawnableBase*> UDMSSpawnableContainerComponent::GetTopNObjects(int Num)
{
	if (SpawnableList.Actors.Num() == 0 || Num <= 0) return TArray<ADMSSpawnableBase*>();
	if (SpawnableList.Actors.Num() <= Num) Num = SpawnableList.Actors.Num();
	return TArray<ADMSSpawnableBase*>(&SpawnableList.Actors.GetData()[SpawnableList.Actors.Num() - Num], Num);
}

void UDMSSpawnableContainerComponent::ShuffleTopNObjects(int Num)
{
	FRandomStream Stream(FDateTime::Now().GetTicks());
	UDMSCoreFunctionLibrary::ShuffleArray(Stream,SpawnableList.Actors,Num);
}

