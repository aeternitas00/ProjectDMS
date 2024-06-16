// Copyright JeongWoo Lee


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

ADMSGameModeBase* UDMSCoreFunctionLibrary::GetDMSGameMode(UObject* WorldContext)
{
	UWorld* tWorld = !WorldContext->IsValidLowLevel() ? GEngine->GameViewport->GetWorld() : WorldContext->GetWorld();

	return tWorld->GetAuthGameMode<ADMSGameModeBase>();
}

ADMSGameStateBase* UDMSCoreFunctionLibrary::GetDMSGameState(UObject* WorldContext)
{
	auto GM = UDMSCoreFunctionLibrary::GetDMSGameMode(WorldContext);

	return GM != nullptr ? GM->GetDMSGameState() : nullptr;
}

UDMSSeqManager* UDMSCoreFunctionLibrary::GetDMSSequenceManager(UObject* WorldContext)
{
	auto GS = UDMSCoreFunctionLibrary::GetDMSGameState(WorldContext);

	return GS != nullptr ? GS->GetSequenceManager() : nullptr;
}

UDMSNotifyManager* UDMSCoreFunctionLibrary::GetDMSNotifyManager(UObject* WorldContext)
{
	auto GS = UDMSCoreFunctionLibrary::GetDMSGameState(WorldContext);

	return GS != nullptr ? GS->GetNotifyManager() : nullptr;
}

UDMSEffectHandler* UDMSCoreFunctionLibrary::GetDMSEffectHandler(UObject* WorldContext)
{
	auto GS = UDMSCoreFunctionLibrary::GetDMSGameState(WorldContext);

	return GS != nullptr ? GS->GetEffectHandler() : nullptr;
}


UDMSPhaseManager* UDMSCoreFunctionLibrary::GetDMSPhaseManager(UObject* WorldContext)
{
	auto GS = UDMSCoreFunctionLibrary::GetDMSGameState(WorldContext);

	return GS != nullptr ? GS->GetPhaseManager() : nullptr;
}

UDMSSelectorManager* UDMSCoreFunctionLibrary::GetDMSSelectorManager(UObject* WorldContext)
{
	auto GS = UDMSCoreFunctionLibrary::GetDMSGameState(WorldContext);

	return GS != nullptr ? GS->GetSelectorManager() : nullptr;
}
