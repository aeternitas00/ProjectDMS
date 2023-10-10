// Fill out your copyright notice in the Description page of Project Settings.


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



UObject* IDMSEffectorInterface::GetObject() 
{ 
	return Cast<UObject>(this);
}

AActor* IDMSEffectorInterface::GetOwningPlayer() 
{ 
	auto Spawnable = Cast<ADMSSpawnableBase>(this);
	auto GS = UDMSCoreFunctionLibrary::GetDMSGameMode()->GetDMSGameState();
	auto Player = Spawnable == nullptr ? nullptr : GS->FindPlayerFromId(Spawnable->GetOwnerID());
	return Player == nullptr ? (AActor*)GS : (AActor*)Player;
}

APlayerController* IDMSEffectorInterface::GetOwningPlayerController()
{
	auto Owner = Cast<IDMSEffectorOwnableInterface>(GetOwningPlayer());
	if (Owner==nullptr) return nullptr;
	//
	return Owner->GetWidgetOwner();
}

void IDMSEffectorInterface::AttachEffectInstance(UDMSEffectInstance* EI)
{
	AActor* thisActor = Cast<AActor>(this);
	if(thisActor->IsValidLowLevelFast())
		thisActor->FindComponentByClass<UDMSEIManagerComponent>()->AttachEffectInstance(EI);
}

bool IDMSEffectorInterface::OnNotifyReceived(TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*>& ResponsedObjects, bool iChainable, UDMSSequence* Seq, UObject* SourceTweak)
{
	AActor* thisActor = Cast<AActor>(this);

	// �⺻ �������� ������Ʈ ������ SourceTweak ����.
	if (thisActor->IsValidLowLevelFast())
	{
		return thisActor->FindComponentByClass<UDMSEIManagerComponent>()->OnNotifyReceived(ResponsedObjects, iChainable, Seq, thisActor);
	}

	// ������Ʈ �̻��� ���� �����ؾ���.
	return false;
}

UDMSEffectSet* IDMSEffectorInterface::GetOwningEffectSet(const FGameplayTag& iSetName)
{ 
	return nullptr; 
}
