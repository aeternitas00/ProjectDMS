// Copyright JeongWoo Lee

#include "GameModes/DMSGameModeBase.h"
//#include "Player/DMSPlayerStateBase.h"
#include "GameModes/DMSGameStateBase.h"
#include "GameFramework/PlayerState.h"

ADMSGameModeBase::ADMSGameModeBase()
{
	GameStateClass = ADMSGameStateBase::StaticClass();
}

void ADMSGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	bool rv = GetDMSGameState()->IsValidLowLevelFast();
	if (rv && GetDMSGameState()->PlayerArray.Num() != 0)
	{
		GetDMSGameState()->SetLeaderPlayer(GetDMSGameState()->PlayerArray[0]->GetPlayerId());
	}
	//DMSGameState=GetGameState<ADMSGameState>();
}



ADMSGameStateBase* ADMSGameModeBase::GetDMSGameState() { return GetGameState<ADMSGameStateBase>(); }



