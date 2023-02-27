// Fill out your copyright notice in the Description page of Project Settings.


#include "Card/DMSCardBase.h"
#include "Card/DMSCardDefinition.h"
#include "Card/DMSCardContainerComponent.h"
#include "GameModes/DMSGameMode.h"

#include "Effect/DMSEffectHandler.h"
#include "Effect/DMSEIManagerComponent.h"
#include "Effect/DMSEffectorInterface.h"


// Sets default values
ADMSCardBase::ADMSCardBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EffectManagerComponent=CreateDefaultSubobject<UDMSEIManagerComponent>("EffectManagerComponent");

}

// Called when the game starts or when spawned
void ADMSCardBase::BeginPlay()
{
	Super::BeginPlay();
	
	// Automatic Type Behavior EI Attachment with carddefintion->defaultCardType
}

// Called every frame
void ADMSCardBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UDMSCardDefinition* ADMSCardBase::GetCardDefinition()
{
	return CardDefinition;
}

void ADMSCardBase::SetCardDefinition(UDMSCardDefinition* iCardDefinition)
{
	CardDefinition = iCardDefinition;
}


void ADMSCardBase::InitializeCard(UDMSCardDefinition* iCardDefinition/*...*/)
{
	// EffectManagerComponent->CleanupOwnEffect(); ?
	SetCardDefinition(iCardDefinition);

	EffectManagerComponent->SetupOwnEffect(CardDefinition->CardEffects->EffectNodes);
}

void ADMSCardBase::AttachEffectInstance(UDMSEffectInstance* EI)
{
	EffectManagerComponent->AttachEffectInstance(EI);
}

void ADMSCardBase::OnNotifyReceived(UDMSSequence* Seq, UObject* SourceTweaker)
{
	//DMS_LOG_SCREEN(TEXT("%s : OnNotifyReceived"), *this->GetName());
	EffectManagerComponent->OnNotifyReceived(Seq,this);
}

UDMSEffectSet* ADMSCardBase::GetOwningEffectSet(const FName& iSetName)
{
	return CardDefinition->CardEffects;
	// return CardDefinition->CardEffectSets.Contain(iSetName) ? CardDefinition->CardEffectSets[iSetName] : nullptr;
}