// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/DMSEffectHandler.h"
#include "Effect/DMSEffectDefinition.h"
#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEffectorInterface.h"

#include "Player/DMSPlayerController.h"
#include "Camera/DMSCameraPawn.h"

#include "Notify/DMSNotifyManager.h"
#include "GameModes/DMSGameMode.h"
#include "Sequence/DMSSequence.h"
#include "GameModes/DMSGameState.h"

#include "Library/DMSCoreFunctionLibrary.h"


// Owned effect creating helper
TArray<UDMSEffectInstance*> UDMSEffectHandler::CreateEffectInstance(UObject* SourceObject,AActor* SourcePlayer, UObject* Target, UDMSEffectNode* EffectNode, UDMSDataObjectSet* iSet)
{
	TArray<UDMSEffectInstance*> rv;
	UDMSEffectInstance* EffectInstance = NewObject<UDMSEffectInstance>(Target);
	EffectInstance->SourceObject = SourceObject;
	EffectInstance->SourcePlayer = SourcePlayer;	
	EffectInstance->Initialize(EffectNode, iSet);

	EIList.Add(EffectInstance);

	rv.Add(EffectInstance);
	return rv;
}


TArray<UDMSEffectInstance*> UDMSEffectHandler::CreateEffectInstance(UDMSSequence* Sequence, UDMSEffectNode* EffectNode)
{
	// No Selected Target ( Passed or No selector for this effect node )
	// If PARAM_TARGET is exist, it will override preset generating.
	if (Sequence->GetTargets().Num() == 0)
		Sequence->SetTarget(UDMSEffectNode::GeneratePresetTarget(EffectNode,Sequence));

	auto ApplyTargets = EffectNode->GenerateApplyTarget(Sequence);

	if (ApplyTargets.Num() == 0)
	{
		DMS_LOG_SCREEN(TEXT("%s : ApplyTargets is emtpy "), *EffectNode->GetName());
	}

	for (auto TargetObject : ApplyTargets)
	{
		UDMSEffectInstance* EffectInstance = NewObject<UDMSEffectInstance>(TargetObject.GetObject());
		EffectInstance->Initialize(EffectNode, Sequence);

		TargetObject->AttachEffectInstance(EffectInstance);

		EIList.Add(EffectInstance);
		Sequence->EIs.Add(EffectInstance);
	}
	
	return Sequence->EIs;
}


void UDMSEffectHandler::ApplyNextEffectInstance(UDMSSequence* SourceSequence, bool PrevSuccessed)
{
	if (!PrevSuccessed)
	{
		// DISCUSSION :: Stopping immediately when failed is FINE?
		OnResolveCompletedMap[SourceSequence].Delegate.ExecuteIfBound(false);
		return;
	}

	if (OnResolveCompletedMap[SourceSequence].Count == SourceSequence->EIs.Num())
		OnResolveCompletedMap[SourceSequence].Delegate.ExecuteIfBound(true);
	else
		OnResolveCompletedMap[SourceSequence].Getter.Execute(SourceSequence)->Apply(SourceSequence, OnResolveCompletedMap[SourceSequence].IteratingDelegate);
}

void UDMSEffectHandler::CleanupNonPersistent()
{
	EIList.RemoveAllSwap([](UDMSEffectInstance* EI) {
		return EI->GetCurrentState() == EDMSEIState::EIS_PendingKill;
	});
}