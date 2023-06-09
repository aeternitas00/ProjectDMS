// Fill out your copyright notice in the Description page of Project Settings.


#include "Card/DMSCardBase.h"
#include "Card/DMSCardDefinition.h"
#include "Card/DMSCardContainerComponent.h"
#include "Location/DMSLocationBase.h"
#include "GameModes/DMSGameMode.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Effect/DMSEffectHandler.h"
#include "Effect/DMSEIManagerComponent.h"
#include "Effect/DMSEffectorInterface.h"
#include "EffectSet/DMSEffect_ActivateEffect.h"
#include "EffectSet/DMSEffectNode_PlayCard.h"
#include "Player/DMSPlayerController.h"

// Sets default values
ADMSCardBase::ADMSCardBase(const FObjectInitializer& ObjectInitializer) : ADMSEffectorActorBase(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates=true;
	SetReplicateMovement(true);

}

// Called when the game starts or when spawned
void ADMSCardBase::BeginPlay()
{
	Super::BeginPlay();
	
	// Automatic Type Behavior EI Attachment with carddefintion->defaultCardType
}

const UDMSCardDefinition* ADMSCardBase::GetCardDefinition()
{
	return CardDefinition;
}

void ADMSCardBase::SetCardDefinition(const UDMSCardDefinition* iCardDefinition)
{
	CardDefinition = iCardDefinition;
}


void ADMSCardBase::SetCurrentLocation_Implementation(ADMSLocationBase* iLoc)
{
	//LocationRef = iLoc;
}

ADMSLocationBase* ADMSCardBase::GetCurrentLocation_Implementation()
{
	//return LocationRef;
	return IDMSLocatableInterface::Execute_GetCurrentLocation(GetOwningPlayer());
}

int ADMSCardBase::GetDistanceWith_Implementation(const TScriptInterface<IDMSLocatableInterface>& OtherObject)
{
	return IDMSLocatableInterface::Execute_GetDistanceWith(GetOwningPlayer(), OtherObject);
}

bool ADMSCardBase::LocatingTo_Implementation(ADMSLocationBase* TargetLocation)
{
	// Add card container component to location?
	//AttachCardToLocation(iLoc,this);

	return false;
}

void ADMSCardBase::OnInitialized_Implementation()
{
	// Caching casted one.
	SetCardDefinition(Cast<UDMSCardDefinition>(OriginalData));

	Super::OnInitialized_Implementation();
	// EffectManagerComponent->CleanupOwnEffect(); ?
	// 


	check(CardDefinition);

	TArray<FGameplayTag> Keys;
	CardDefinition->CardEffectSets.GetKeys(Keys);

	for(auto& Key : Keys)
		EffectManagerComponent->SetupOwnEffect(CardDefinition->CardEffectSets[Key], Key);
}

//void ADMSCardBase::PostInitialize_Implementation()
//{
//	Super::PostInitialize_Implementation();
//}

UDMSEffectSet* ADMSCardBase::GetOwningEffectSet(const FGameplayTag& iSetName)
{
	//return CardDefinition->CardEffects;
	return CardDefinition->CardEffectSets.Contains(iSetName) ? CardDefinition->CardEffectSets[iSetName] : nullptr;
}


