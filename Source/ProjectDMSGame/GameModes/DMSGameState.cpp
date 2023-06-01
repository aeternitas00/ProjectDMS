// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/DMSGameState.h"
#include "Player/DMSPlayerState.h"
#include "Player/DMSPlayerController.h"
#include "Camera/DMSCameraPawn.h"
#include "Effect/DMSEIManagerComponent.h"

void ADMSGameState::SetPlayersFocusTarget(AActor* Target)
{
	if (Target == nullptr) return;

	auto PlayerControllers = GetDMSPlayerControllers();

	for (auto PC : PlayerControllers)
	{
		auto Loc = Target->GetActorLocation();
		auto OrLoc = PC->GetCameraPawn()->GetActorLocation();
		PC->GetCameraPawn()->SetActorLocation(FVector(OrLoc.X, Loc.Y, Loc.Z));
	}
}

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

TArray<ADMSPlayerState*> ADMSGameState::GetDMSPlayers()
{
	TArray<ADMSPlayerState*> rv;
	
	for (auto& Player : PlayerArray)
	{
		if (Player->IsA<ADMSPlayerState>())
			rv.Add(Cast<ADMSPlayerState>(Player));
	}

	return rv;
}

TArray<ADMSPlayerController*> ADMSGameState::GetDMSPlayerControllers()
{
	TArray<ADMSPlayerController*> rv;

	for (auto& Player : PlayerArray)
	{
		if (Player->GetPlayerController()->IsA<ADMSPlayerController>())
			rv.Add(Cast<ADMSPlayerController>(Player->GetPlayerController()));
	}

	return rv;
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

