// Fill out your copyright notice in the Description page of Project Settings.


#include "DMSCoreFunctionLibrary.h"
#include "GameModes/DMSGameMode.h"
#include "Sequence/DMSSeqManager.h"
#include "Notify/DMSNotifyManager.h"
#include "Effect/DMSEffectHandler.h"

ADMSGameMode* UDMSCoreFunctionLibrary::GetDMSGameMode()
{
	UWorld* tWorld = GEngine->GameViewport->GetWorld();

	return (Cast<ADMSGameMode>(UGameplayStatics::GetGameMode(tWorld)));
}

UDMSSeqManager* UDMSCoreFunctionLibrary::GetDMSSequenceManager()
{
	auto GM = UDMSCoreFunctionLibrary::GetDMSGameMode();
	
	return GM != nullptr ? GM->GetSequenceManager() : nullptr ;
}

UDMSNotifyManager* UDMSCoreFunctionLibrary::GetDMSNotifyManager()
{
	auto GM = UDMSCoreFunctionLibrary::GetDMSGameMode();

	return GM != nullptr ? GM->GetNotifyManager() : nullptr;
}

UDMSEffectHandler* UDMSCoreFunctionLibrary::GetDMSEffectHandler()
{
	auto GM = UDMSCoreFunctionLibrary::GetDMSGameMode();

	return GM != nullptr ? GM->GetEffectHandler() : nullptr;
}
