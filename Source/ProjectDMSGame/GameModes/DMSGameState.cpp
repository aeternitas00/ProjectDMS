// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/DMSGameState.h"
#include "Player/DMSPlayerState.h"
#include "Effect/DMSEIManagerComponent.h"

ADMSGameState::ADMSGameState(const FObjectInitializer& Initializer) /*: Super(Initializer)*/
{
	//CardManagerComponent = CreateDefaultSubobject<UDMSCardManagerComponent>(TEXT("CardManagerComponent"));
	EffectManagerComponent = CreateDefaultSubobject<UDMSEIManagerComponent>("EffectManagerComponent");

}

bool ADMSGameState::SetLeaderPlayer(int32 inLeaderID)
{
	for (auto& Player : PlayerArray)
	{
		if (inLeaderID == Player->GetPlayerId())
		{
			LeaderPlayerID= inLeaderID;
			return true;
		}
	}
	return false;
}

APlayerState* ADMSGameState::FindPlayerFromId(int32 OwnerID)
{
	for (auto& Player : PlayerArray)
	{
		if (OwnerID == Player->GetPlayerId())
			return Player;
	}
	return nullptr;
}

APlayerController* ADMSGameState::FindPlayerControllerFromId(int32 OwnerID)
{
	return FindPlayerFromId(OwnerID) == nullptr ? FindPlayerFromId(LeaderPlayerID)->GetPlayerController() : FindPlayerFromId(OwnerID)->GetPlayerController();
}

//void ADMSGameState::AttachEffectInstance(UDMSEffectInstance* EI)
//{
//	EffectManagerComponent->AttachEffectInstance(EI);
//}
//
//bool ADMSGameState::OnNotifyReceived(TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*>& ResponsedObjects,
//	bool iChainable, UDMSSequence* Seq, UObject* SourceTweak)
//{
//	return EffectManagerComponent->OnNotifyReceived(ResponsedObjects, iChainable, Seq, this);
//}
