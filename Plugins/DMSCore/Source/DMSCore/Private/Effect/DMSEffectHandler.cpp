// Copyright JeongWoo Lee


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

UDMSEffectHandler::UDMSEffectHandler()
{
	//ResolveIteratingDelegate.BindUObject(this, &UDMSEffectHandler::ApplyNextEffectInstance);
}

// Owned effect creating helper
ADMSActiveEffect* UDMSEffectHandler::CreatePersistentActiveEffect(AActor* SourceObject,AActor* SourcePlayer, AActor* Target, UDMSEffectNode* EffectNode)
{
	FActorSpawnParameters Param;
	Param.Owner=Target;

	ADMSActiveEffect* EffectInstance = GetWorld()->SpawnActor<ADMSActiveEffect>(Param);
	EffectInstance->AttachToActor(Target,FAttachmentTransformRules::SnapToTargetIncludingScale);
	EffectInstance->SourceObject = SourceObject;
	EffectInstance->SourcePlayer = SourcePlayer;	
	EffectInstance->Initialize(EffectNode,EDMSAEState::AES_Persistent);
	//EffectInstance->SetupDatas(iSet);

	EIList.Add(EffectInstance);

	return EffectInstance;
}


TArray<ADMSActiveEffect*> UDMSEffectHandler::CreateApplyingActiveEffect(ADMSSequence* Sequence, UDMSEffectNode* EffectNode)
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
		Sequence->SetTargetted(false);
		return {};
	}

	// Attach temporal AE to target. 
	TArray<ADMSActiveEffect*> rv;
	for (auto& ApplyStorage : Storages)
	{
		for (auto& Target : ApplyStorage.ApplyTargets)
		{
			auto Comp = Target->GetEffectorManagerComponent();
			if (!Comp) continue;

			FActorSpawnParameters Param;	Param.Owner=Target->GetObject();
			ADMSActiveEffect* EffectInstance;

			// Change this part.
			//if (EffectNode->bIsPersistent)	EffectInstance = GetWorld()->SpawnActor<ADMSActiveEffect_Persistent>(Param);
			//else							EffectInstance = GetWorld()->SpawnActor<ADMSActiveEffect_Applying>(Param);
			EDMSAEState InitialState = EDMSAEState::AES_NotifyClosed;
			if(EffectNode->TerminateConditions){
				InitialState |= EDMSAEState::AES_Persistent; 
				InitialState |= EDMSAEState::AES_PendingTerminate;
			}
			EffectInstance = GetWorld()->SpawnActor<ADMSActiveEffect>(Param);
			EffectInstance->AttachToActor(Target->GetObject(),FAttachmentTransformRules::SnapToTargetIncludingScale);
			EffectInstance->Initialize(EffectNode,InitialState);
			//flag?
			EffectInstance->InheritSequenceDatas(Sequence);

			Comp->AttachEffectInstance(EffectInstance);

			EIList.Add(EffectInstance);
			ApplyStorage.EIs.Add(EffectInstance);
			rv.Add(EffectInstance);
		}
	}
	
	Sequence->SetTargetted(true);

	return rv;
}

/**
* Resolve param sequence and executed paramed lambda function when resolve completed.
* @param	Sequence					Resolving sequence.
* @param	OnResolveCompleted			Lambda executed when resolve completed.
*/
//void UDMSEffectHandler::Resolve(ADMSSequence* Sequence, const FOnTaskCompleted& OnResolveCompleted)
//{
//	//DMS_LOG_SCREEN(TEXT("EH : Resolve %s"), *Sequence->GetName());
//
//	if(Sequence->GetAllActiveEffects().Num() == 0 || !Sequence->IsTargetted()) {
//		DMS_LOG_SIMPLE(TEXT("EffectHandler::Resolve : No Resolve Target"));
//		OnResolveCompleted.ExecuteIfBound(true);
//		return;
//	}
//	// seperate for logging
//	if(Sequence->SequenceState != EDMSSequenceState::SS_Default) {
//		DMS_LOG_SIMPLE(TEXT("EffectHandler::Resolve : Sequence is canceled or ignored"));
//		OnResolveCompleted.ExecuteIfBound(true);
//		return;
//	}
//	
//	UDMSEffectResolveWorker* NewWorker = NewObject<UDMSEffectResolveWorker>(this);
//
//	TArray<UObject*> Contexts;
//
//	// TODO :: Only for AE of current MainTarget
//	for(auto& AE : Sequence->GetCurrentActiveEffects()) 
//		Contexts.Add(AE);
//	NewWorker->SetupTaskWorkerDelegate(Contexts, OnResolveCompleted);	
//	NewWorker->SetupResolveWorker(Sequence);
//	NewWorker->RunTaskWorker(true);
//
//}

void UDMSEffectHandler::CleanupNonPersistent()
{
	//OnResolveCompletedMap.Empty();
	EIList.RemoveAllSwap([](ADMSActiveEffect* EI) {
		bool rv = (EI->GetCurrentState() & EDMSAEState::AES_Persistent) != EDMSAEState::AES_Persistent;
		if (rv){ EI->DetachFromOwner(); EI->Destroy(); }
		return rv;
	});
}


