// Copyright Epic Games, Inc. All Rights Reserved.


#include "DMSGameMode.h"
#include "Sequence/DMSSeqManager.h"
#include "Player/DMSPlayerController.h"
#include "Effect/DMSEffectHandler.h"
#include "Effect/DMSEffectorInterface.h"
#include "Notify/DMSNotifyManager.h"
#include "Card/DMSCardDefinition.h"

ADMSGameMode::ADMSGameMode()
{
	EffectHandler = CreateDefaultSubobject<UDMSEffectHandler>(TEXT("CardEffectHandler"));
	SequenceManager = CreateDefaultSubobject<UDMSSeqManager>(TEXT("SequenceManager"));
	NotifyManager = CreateDefaultSubobject<UDMSNotifyManager>(TEXT("NotifyManager"));
}

void ADMSGameMode::SpawnCardsFromDeck(class ADMSPlayerController* iPC)
{

}

void ADMSGameMode::RegisterNotifyObject(TScriptInterface<IDMSEffectorInterface> Object)
{
}

void ADMSGameMode::PushSequence()
{
}
