// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DMSPlayerState.h"

#include "System/DMSSaveGame.h"

#include "Effect/DMSEffectDefinition.h"
#include "Effect/DMSEIManagerComponent.h"

#include "Card/DMSCardContainerComponent.h"
#include "Card/DMSCardManagerComponent.h"

#include "Attribute/DMSAttributeComponent.h"
#include "Character/DMSCharacterBase.h"

#include "Library/DMSCoreFunctionLibrary.h"

ADMSPlayerState::ADMSPlayerState(const FObjectInitializer& Initializer) /*: Super(Initializer)*/
{
	CardManagerComponent = CreateDefaultSubobject<UDMSCardManagerComponent>(TEXT("CardManagerComponent"));
	EffectManagerComponent = CreateDefaultSubobject<UDMSEIManagerComponent>("EffectManagerComponent");
	AttributeComponent = CreateDefaultSubobject<UDMSAttributeComponent>(TEXT("AttributeComponent"));

	DefaultStats.Add(TAG_DMS_Attribute_Resource, 10);
	DefaultStats.Add(TAG_DMS_Attribute_ActionPoint, 10);
	DefaultStats.Add(TAG_DMS_Attribute_HP, 10);
	DefaultStats.Add(TAG_DMS_Attribute_STR, 5);
	DefaultStats.Add(TAG_DMS_Attribute_INT, 5);
	DefaultStats.Add(TAG_DMS_Attribute_DEX, 5);
	DefaultStats.Add(TAG_DMS_Attribute_SavedSkillBonus, 0);
}

void ADMSPlayerState::SetupCardContainers()
{
	for (auto ContainerDef : CardContainerTypes) {
		CardManagerComponent->ConstructContainer(ContainerDef.Key, ContainerDef.Value);
	}
}

void ADMSPlayerState::SetupAttributes()
{
	for (auto Stat : DefaultStats) {
		AttributeComponent->MakeAttribute(Stat.Key, Stat.Value);
	}
}


void ADMSPlayerState::LoadDatasFromSave(UDMSSaveGame* SaveGame)
{
	PlayerCharacterData = SaveGame->SavedCharacterData;
	SetCardDatas(SaveGame->SavedCardDatas);
}

void ADMSPlayerState::SetCardDatas(const TArray<FDMSCardData>& InDatas)
{
	OriginalCardDatas = InDatas;

	for (auto& Data : OriginalCardDatas) 
	{
		Data.LoadCardDefinition(); 
		UDMSCoreFunctionLibrary::SetDataOwner(Data,this);
	}
}

void ADMSPlayerState::SetupDefaults()
{
	SetupCardContainers();
	SetupAttributes();

	// Load Player datas
	// -> 
	// Spawn Players Character

	//PreviewDummy = DuplicateObject(this,this,FName(GetName()+TEXT("_Preview")));
	// Referencer initalize
	//PreviewDummy->CharacterRef = Cast<ADMSCharacterBase>(CharacterRef->GetPreviewObject());
}

UDMSCardContainerComponent* ADMSPlayerState::SearchContainer(const FName& ContainerName)
{
	return CardManagerComponent ? CardManagerComponent->SearchContainer(ContainerName) : nullptr;
}


void ADMSPlayerState::SetCurrentLocation_Implementation(ADMSLocationBase* iLoc)
{
	IDMSLocatableInterface::Execute_SetCurrentLocation(CharacterRef, iLoc);
}

ADMSLocationBase* ADMSPlayerState::GetCurrentLocation_Implementation()
{
	return IDMSLocatableInterface::Execute_GetCurrentLocation(CharacterRef);
}

int ADMSPlayerState::GetDistanceWith_Implementation(const TScriptInterface<IDMSLocatableInterface>& OtherObject)
{
	return IDMSLocatableInterface::Execute_GetDistanceWith(CharacterRef, OtherObject);
}

bool ADMSPlayerState::LocatingTo_Implementation(ADMSLocationBase* TargetLocation)
{
	return IDMSLocatableInterface::Execute_LocatingTo(CharacterRef, TargetLocation);
}

