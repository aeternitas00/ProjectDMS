// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/DMSEffectHandler.h"
#include "Effect/DMSEffectDefinition.h"
#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEffectorInterface.h"

//#include "Player/DMSPlayerController.h"
//#include "Camera/DMSCameraPawn.h"

#include "Notify/DMSNotifyManager.h"
#include "GameModes/DMSGameModeBase.h"
#include "GameModes/DMSGameStateBase.h"
#include "Sequence/DMSSequence.h"

#include "Effect/DMSEIManagerComponent.h"

#include "GameFramework/PlayerController.h"

#include "Library/DMSCoreFunctionLibrary.h"


// Owned effect creating helper
TArray<ADMSActiveEffect*> UDMSEffectHandler::CreateEffectInstance(AActor* SourceObject,AActor* SourcePlayer, AActor* Target, UDMSEffectNode* EffectNode, UDMSDataObjectSet* iSet)
{
	TArray<ADMSActiveEffect*> rv;
	FActorSpawnParameters Param;
	Param.Owner=Target;

	ADMSActiveEffect* EffectInstance = GetWorld()->SpawnActor<ADMSActiveEffect>(Param);
	EffectInstance->AttachToActor(Target,FAttachmentTransformRules::SnapToTargetIncludingScale);
	EffectInstance->SourceObject = SourceObject;
	EffectInstance->SourcePlayer = SourcePlayer;	
	EffectInstance->Initialize(EffectNode, iSet);

	EIList.Add(EffectInstance);

	rv.Add(EffectInstance);
	return rv;
}


TArray<ADMSActiveEffect*> UDMSEffectHandler::CreateEffectInstance(UDMSSequence* Sequence, UDMSEffectNode* EffectNode)
{
	// No Selected Target ( Passed or No selector for this effect node )
	// If PARAM_TARGET is exist, it will override preset generating.

	TArray<FDMSSequenceEIStorage>& Storages = Sequence->GetEIStorage();

	if (Sequence->GetTargets().Num() == 0)
		Sequence->SetTarget(UDMSEffectNode::GeneratePresetTarget(EffectNode,Sequence));

	UDMSEffectNode::GenerateApplyTarget(EffectNode,Sequence);

	if (Storages.Num() == 0)
	{
		DMS_LOG_SIMPLE(TEXT("%s : ApplyTargets is emtpy "), *EffectNode->GetName());
	}

	TArray<ADMSActiveEffect*> rv;
	for (auto& ApplyStorage : Storages)
	{
		for (auto& Target : ApplyStorage.ApplyTargets)
		{
			auto Comp = Target->GetEffectorManagerComponent();
			if (!Comp) continue;

			FActorSpawnParameters Param;
			Param.Owner=Target->GetObject();
			ADMSActiveEffect* EffectInstance =  GetWorld()->SpawnActor<ADMSActiveEffect>(Param);
			EffectInstance->AttachToActor(Target->GetObject(),FAttachmentTransformRules::SnapToTargetIncludingScale);
			EffectInstance->Initialize(EffectNode, Sequence);
			
			Comp->AttachEffectInstance(EffectInstance);

			EIList.Add(EffectInstance);
			ApplyStorage.EIs.Add(EffectInstance);
			rv.Add(EffectInstance);
		}
	}
	
	Sequence->SetTargetted(true);

	return rv;
}


void UDMSEffectHandler::ApplyNextEffectInstance(UDMSSequence* SourceSequence, bool PrevSucceeded)
{
	if (!PrevSucceeded)
	{
		// DISCUSSION :: Stopping immediately when failed is FINE?
		OnResolveCompletedMap[SourceSequence].Delegate.ExecuteIfBound(false);
		return;
	}

	if (OnResolveCompletedMap[SourceSequence].Count == SourceSequence->GetAllEIs().Num())
		OnResolveCompletedMap[SourceSequence].Delegate.ExecuteIfBound(true);
	else
		OnResolveCompletedMap[SourceSequence].Getter.Execute(SourceSequence)->Apply(SourceSequence, OnResolveCompletedMap[SourceSequence].IteratingDelegate);
}

void UDMSEffectHandler::CleanupNonPersistent()
{
	EIList.RemoveAllSwap([](ADMSActiveEffect* EI) {
		bool rv = EI->GetCurrentState() == EDMSEIState::EIS_PendingKill;
		if (rv) EI->Destroy();
		return rv;
	});
}