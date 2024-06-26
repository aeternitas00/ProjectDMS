// Copyright JeongWoo Lee

#include "Common/DMSSpawnableComponent.h"
#include "Common/DMSSpawnableDataBase.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "GameModes/DMSGameStateBase.h"
#include "Player/DMSPlayerStateBase.h"

UDMSSpawnableComponent::UDMSSpawnableComponent():ParentComponent(nullptr)
{
}

// DO we actually need this????
void UDMSSpawnableComponent::UpdateParentComponent()
{
	ParentComponent = nullptr;
	if (GetOwner()->IsA<ADMSSpawnableBase>()){
		auto GS = UDMSCoreFunctionLibrary::GetDMSGameState(this);
		AActor* PlayerActor = GS->FindPlayerFromId(Cast<ADMSSpawnableBase>(GetOwner())->GetOwnerID());
		AActor* NetOwnerActor = PlayerActor==nullptr ? GS : PlayerActor;

		// 같은 오너의 같은 컴포넌트를 패런츠로.
		ParentComponent = (UDMSSpawnableComponent*)NetOwnerActor->GetComponentByClass(GetClass());
	}
}
