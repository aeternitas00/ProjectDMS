// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DMSPlayerState.h"

#include "Net/OnlineEngineInterface.h"

#include "System/DMSSaveGame.h"

#include "Effect/DMSEffectDefinition.h"
#include "Effect/DMSEIManagerComponent.h"

#include "Card/DMSCardContainerComponent.h"
#include "Card/DMSCardManagerComponent.h"

#include "Attribute/DMSAttributeComponent.h"
#include "Character/DMSCharacterBase.h"

#include "Library/DMSGameFunctionLibrary.h"

#include "Common/DMSGameTags.h"
#include "DMSPlayerState.h"

ADMSPlayerState::ADMSPlayerState(const FObjectInitializer& Initializer) : ADMSPlayerStateBase(Initializer)
{
	CardManagerComponent = CreateDefaultSubobject<UDMSCardManagerComponent>(TEXT("CardManagerComponent"));
	//AttributesComponent=nullptr;
	AttributeComponent = CreateDefaultSubobject<UDMSAttributeComponent>(TEXT("AttributesComponent"));

	CardManagerComponent->SetIsReplicated(true);
	AttributeComponent->SetIsReplicated(true);


	//DefaultStats.Add(TAG_DMS_Attribute_Resource, 10);
	//DefaultStats.Add(TAG_DMS_Attribute_ActionPoint, 10);
	//DefaultStats.Add(TAG_DMS_Attribute_HP, 10);
	//DefaultStats.Add(TAG_DMS_Attribute_STR, 5);
	//DefaultStats.Add(TAG_DMS_Attribute_INT, 5);
	//DefaultStats.Add(TAG_DMS_Attribute_DEX, 5);
	//DefaultStats.Add(TAG_DMS_Attribute_SavedSkillBonus, 0);
}

//void ADMSPlayerState::SetupCardContainers()
//{
	//for (auto ContainerDef : CardContainerTypes) {
	//	CardManagerComponent->ConstructContainer(ContainerDef.Key, ContainerDef.Value);
	//}
//}

void ADMSPlayerState::SetupAttributes()
{
	for (auto Stat : DefaultAttributes) {
		AttributeComponent->GenerateAndSetAttribute(Stat.DefaultTag,Stat.DefaultValue);
	}

	for (auto CharacterStat : PlayerCharacterData.Attributes){
		AttributeComponent->GenerateAndSetAttribute(CharacterStat.DefaultTag,CharacterStat.DefaultValue);
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
		UDMSGameFunctionLibrary::SetCardDataOwner(Data,this);
	}
}

void ADMSPlayerState::SetupDefaults()
{
	//SetupCardContainers();
	LoadSaveGame("TestSlot",0);
	SetupAttributes();
}

void ADMSPlayerState::OnLoadSaveGame_Implementation(UDMSSaveGame* LoadedItem)
{
	DMS_LOG_SIMPLE(TEXT("%s : OnLoadSaveGame"),*this->GetName());

	LoadDatasFromSave(LoadedItem);
}

void ADMSPlayerState::LoadSaveGame(const FString& SlotName, const int32 UserIndex)
{
	auto DMSSaveGame=Cast<UDMSSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));

	OnLoadSaveGame(DMSSaveGame);
}


UDMSCardContainerComponent* ADMSPlayerState::SearchContainer(const FGameplayTag& ContainerName)
{
	return CardManagerComponent ? CardManagerComponent->SearchContainer(ContainerName) : nullptr;
}

//void ADMSPlayerState::RegisterPlayerWithSession(bool bWasFromInvite)
//{
//	if(GetNetMode() != NM_Standalone)
//	{
//		if(GetUniqueId().IsValid())
//		{
//			// Register the player as part of the session
//			if(UOnlineEngineInterface::Get()->DoesSessionExist(GetWorld(), SessionName))
//			{
//				UOnlineEngineInterface::Get()->RegisterPlayer(GetWorld(), SessionName, GetUniqueId(), bWasFromInvite);
//			}
//		}
//	}
//}
//
//void ADMSPlayerState::UnregisterPlayerWithSession()
//{
//	if (GetNetMode() == NM_Client && GetUniqueId().IsValid())
//	{
//		if (SessionName != NAME_None)
//		{
//			if (UOnlineEngineInterface::Get()->DoesSessionExist(GetWorld(), SessionName))
//			{
//				UOnlineEngineInterface::Get()->UnregisterPlayer(GetWorld(), SessionName, GetUniqueId());
//			}
//		}
//	}
//}


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

void ADMSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADMSPlayerState, CardManagerComponent);	
	DOREPLIFETIME(ADMSPlayerState, AttributeComponent);	
}

