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

void ADMSCardBase::Initialize_Implementation(const UDMSSpawnableDataBase* iCardDefinition/*, Saved data...*/)
{
	// EffectManagerComponent->CleanupOwnEffect(); ?
	// 
	//Super::Initialize(iCardDefinition);
	OriginalData = iCardDefinition;

	// Caching casted one.
	SetCardDefinition(Cast<UDMSCardDefinition>(iCardDefinition));

	check(CardDefinition);

	TArray<FGameplayTag> Keys;
	CardDefinition->CardEffectSets.GetKeys(Keys);

	// TODO :: 액티베이트 BA를 플레이 BA랑 비슷하게 공시퀀스 생성으로 바꿀것.
	for(auto& Key : Keys)
		EffectManagerComponent->SetupOwnEffect(CardDefinition->CardEffectSets[Key], Key);

	// 우리꺼만?
	//if (CardDefinition->CardEffectSets.Contains(TAG_DMS_EffectType_Effect))
	//	EffectManagerComponent->SetupOwnEffect(CardDefinition->CardEffectSets[TAG_DMS_EffectType_Effect], TAG_DMS_EffectType_Effect);

	//if (CardDefinition->CardEffectSets.Contains(TAG_DMS_EffectType_Cost))
	//	EffectManagerComponent->SetupOwnEffect(CardDefinition->CardEffectSets[TAG_DMS_EffectType_Cost], TAG_DMS_EffectType_Cost);
}

UDMSEffectSet* ADMSCardBase::GetOwningEffectSet(const FGameplayTag& iSetName)
{
	//return CardDefinition->CardEffects;
	return CardDefinition->CardEffectSets.Contains(iSetName) ? CardDefinition->CardEffectSets[iSetName] : nullptr;
}


