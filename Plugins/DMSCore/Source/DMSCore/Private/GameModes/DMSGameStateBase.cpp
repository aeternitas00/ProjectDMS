// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/DMSGameStateBase.h"
#include "GameFramework/PlayerState.h"
//#include "Player/DMSPlayerController.h"
//#include "Camera/DMSCameraPawn.h"
#include "Effect/DMSEIManagerComponent.h"

ADMSGameStateBase::ADMSGameStateBase(const FObjectInitializer& Initializer) /*: Super(Initializer)*/
{
	//CardManagerComponent = CreateDefaultSubobject<UDMSCardManagerComponent>(TEXT("CardManagerComponent"));
	EffectManagerComponent = CreateDefaultSubobject<UDMSEIManagerComponent>("EffectManagerComponent");

}


bool ADMSGameStateBase::SetLeaderPlayer(int32 inLeaderID)
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
\
APlayerState* ADMSGameStateBase::FindPlayerFromId(int32 OwnerID)
{
	for (auto& Player : PlayerArray)
	{
		if (OwnerID == Player->GetPlayerId())
			return Player;
	}
	return nullptr;
}

APlayerController* ADMSGameStateBase::FindPlayerControllerFromId(int32 OwnerID)
{
	return FindPlayerFromId(OwnerID) == nullptr ? FindPlayerFromId(LeaderPlayerID)->GetPlayerController() : FindPlayerFromId(OwnerID)->GetPlayerController();
}

