// Fill out your copyright notice in the Description page of Project Settings.


#include "Card/DMSCardBase.h"
#include "Card/DMSCardDefinition.h"
#include "Card/DMSCardContainerComponent.h"
#include "GameModes/DMSGameMode.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Effect/DMSEffectHandler.h"
#include "Effect/DMSEIManagerComponent.h"
#include "Effect/DMSEffectorInterface.h"
#include "EffectSet/DMSEffect_ActivateEffect.h"
#include "EffectSet/DMSEffectNode_PlayCard.h"

// Sets default values
ADMSCardBase::ADMSCardBase() : ADMSEffectorActorBase()
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


void ADMSCardBase::InitializeCard_Implementation(const UDMSCardDefinition* iCardDefinition/*, Saved data...*/)
{
	// EffectManagerComponent->CleanupOwnEffect(); ?
	SetCardDefinition(iCardDefinition);

	// ¿ì¸®²¨¸¸?
	if (CardDefinition->CardEffectSets.Contains("Effect"))
		EffectManagerComponent->SetupOwnEffect(CardDefinition->CardEffectSets["Effect"], "Effect");

	if (CardDefinition->CardEffectSets.Contains("Cost"))
		EffectManagerComponent->SetupOwnEffect(CardDefinition->CardEffectSets["Cost"], "Cost");
}

UDMSEffectSet* ADMSCardBase::GetOwningEffectSet(const FName& iSetName)
{
	//return CardDefinition->CardEffects;
	return CardDefinition->CardEffectSets.Contains(iSetName) ? CardDefinition->CardEffectSets[iSetName] : nullptr;
}


