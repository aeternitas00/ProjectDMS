// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/DMSCoreFunctionLibrary.h"
#include "Selector/DMSSelectorManager.h"
#include "GameModes/DMSGameModeBase.h"
#include "GameModes/DMSGameStateBase.h"
#include "Sequence/DMSSeqManager.h"
#include "Effect/DMSEffectHandler.h"
#include "Notify/DMSNotifyManager.h"
#include "Phase/DMSPhaseManager.h"
#include "Common/DMSCommons.h"

FString UDMSCoreFunctionLibrary::GetTimingString(const EDMSTimingFlag& Flag)
{
	switch (Flag)	{
		case EDMSTimingFlag::T_Before:	return "BEFORE";
		case EDMSTimingFlag::T_During:	return "DURING"; ;
		case EDMSTimingFlag::T_After:	return "AFTER"; 
		default: return "";
	}
}

ADMSGameModeBase* UDMSCoreFunctionLibrary::GetDMSGameMode()
{
	UWorld* tWorld = GEngine->GameViewport->GetWorld();

	return (Cast<ADMSGameModeBase>(UGameplayStatics::GetGameMode(tWorld)));
}

ADMSGameStateBase* UDMSCoreFunctionLibrary::GetDMSGameState()
{
	auto GM = UDMSCoreFunctionLibrary::GetDMSGameMode();

	return GM != nullptr ? GM->GetDMSGameState() : nullptr;
}

UDMSSeqManager* UDMSCoreFunctionLibrary::GetDMSSequenceManager()
{
	auto GS = UDMSCoreFunctionLibrary::GetDMSGameState();

	return GS != nullptr ? GS->GetSequenceManager() : nullptr;
}

UDMSNotifyManager* UDMSCoreFunctionLibrary::GetDMSNotifyManager()
{
	auto GS = UDMSCoreFunctionLibrary::GetDMSGameState();

	return GS != nullptr ? GS->GetNotifyManager() : nullptr;
}

UDMSEffectHandler* UDMSCoreFunctionLibrary::GetDMSEffectHandler()
{
	auto GS = UDMSCoreFunctionLibrary::GetDMSGameState();

	return GS != nullptr ? GS->GetEffectHandler() : nullptr;
}


UDMSPhaseManager* UDMSCoreFunctionLibrary::GetDMSPhaseManager()
{
	auto GS = UDMSCoreFunctionLibrary::GetDMSGameState();

	return GS != nullptr ? GS->GetPhaseManager() : nullptr;
}

UDMSSelectorManager* UDMSCoreFunctionLibrary::GetDMSSelectorManager()
{
	auto GS = UDMSCoreFunctionLibrary::GetDMSGameState();

	return GS != nullptr ? GS->GetSelectorManager() : nullptr;
}
