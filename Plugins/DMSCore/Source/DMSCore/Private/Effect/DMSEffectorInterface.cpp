// Copyright JeongWoo Lee


#include "Effect/DMSEffectorInterface.h"
#include "Effect/DMSEffectorOwnableInterface.h"
#include "Sequence/DMSSequence.h"
#include "Common/DMSSpawnableDataBase.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "GameModes/DMSGameModeBase.h"
#include "GameModes/DMSGameStateBase.h"
#include "Effect/DMSEIManagerComponent.h"
#include "Effect/DMSEffectDefinition.h"
#include "GameFramework/PlayerState.h"

// Add default functionality here for any IDMSEIAttachable functions that are not pure virtual.



AActor* IDMSEffectorInterface::GetObject() 
{ 
	return Cast<AActor>(this);
}

AActor* IDMSEffectorInterface::GetOwningPlayer_Implementation() 
{ 
	auto Spawnable = Cast<ADMSSpawnableBase>(this);
	auto GS = UDMSCoreFunctionLibrary::GetDMSGameMode(Cast<UObject>(this))->GetDMSGameState();
	auto Player = Spawnable == nullptr ? nullptr : GS->FindPlayerFromId(Spawnable->GetOwnerID());

	// ADMSSpawnableBase 처럼 넷 오너 ID가 있을 경우 그것의 플레이어 리턴
	// 그외의 모든 경우에 게임 스테이트 ( 게임 시스템 / 서버 ) 가 소유한 것으로 취급.
	return Player == nullptr ? (AActor*)GS : (AActor*)Player;
}

int32 IDMSEffectorInterface::GetOwnerPlayerID()
{
	auto Owner = Cast<IDMSEffectorOwnableInterface>(Execute_GetOwningPlayer(Cast<UObject>(this)));
	if (Owner == nullptr) return -1;
	//
	return Owner->GetID();
}

ADMSPlayerControllerBase* IDMSEffectorInterface::GetOwningPlayerController()
{
	auto Owner = Cast<IDMSEffectorOwnableInterface>(Execute_GetOwningPlayer(Cast<UObject>(this)));
	if (Owner==nullptr) return nullptr;
	//
	return Owner->GetWidgetOwner();
}

UDMSEIManagerComponent* IDMSEffectorInterface::GetEffectorManagerComponent()
{
	AActor* thisActor = Cast<AActor>(this);
	if(thisActor->IsValidLowLevelFast())
		return thisActor->FindComponentByClass<UDMSEIManagerComponent>();
	return nullptr;
}

UDMSEffectSet* IDMSEffectorInterface::GetOwningEffectSet(const FGameplayTag& iSetName)
{ 
	return nullptr; 
}
