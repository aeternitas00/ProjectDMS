// Fill out your copyright notice in the Description page of Project Settings.


#include "DMSCoreFunctionLibrary.h"
#include "GameModes/DMSGameMode.h"
#include "Sequence/DMSSeqManager.h"
#include "Notify/DMSNotifyManager.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Effect/DMSEffectHandler.h"
#include "Card/DMSCardBase.h"
#include "Card/DMSCardDefinition.h"

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

bool UDMSCoreFunctionLibrary::GetAttributeFromActor(AActor* iActor, const FName& Name, float& outValue)
{
	if (iActor == nullptr) goto INVSOURCE;

	UActorComponent* Comp;
	Comp = iActor->GetComponentByClass(UDMSAttributeComponent::StaticClass());

	UDMSAttributeComponent* AttComp = Cast<UDMSAttributeComponent>(Comp);
	if (AttComp == nullptr) goto INVSOURCE;

	float SourceValue;
	if (!AttComp->GetAttributeValue(Name, SourceValue)) {
	INVSOURCE:
		outValue = 0.0f; return false;
	}

	outValue = SourceValue;
	return true;
}

bool UDMSCoreFunctionLibrary::CheckCardIsCommitable(ADMSCardBase* iCard)
{
	return iCard->GetCardDefinition()->CardEffectSets.Contains("Commit");
}
