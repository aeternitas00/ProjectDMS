// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/DMSEffectHandler.h"
#include "Effect/DMSEffectDefinition.h"
#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEffectorInterface.h"
#include "Notify/DMSNotifyManager.h"
#include "GameModes/DMSGameMode.h"
#include "Sequence/DMSSequence.h"
#include "GameModes/DMSGameMode.h"




TArray<UDMSEffectInstance*> UDMSEffectHandler::CreateEffectInstance(UObject* SourceObject,AActor* SourceController, UDMSEffectNode* EffectNode, UDMSDataObjectSet* iSet)
{
	TArray<UDMSEffectInstance*> rv;
	UDMSEffectInstance* EffectInstance = NewObject<UDMSEffectInstance>(this);
	EffectInstance->Initialize(EffectNode, iSet);
	EffectInstance->SourceObject = SourceObject;
	EffectInstance->SourceController = SourceController;

	rv.Add(EffectInstance);
	return rv;
}


TArray<UDMSEffectInstance*> UDMSEffectHandler::CreateEffectInstance(UDMSSequence* Sequence, UDMSEffectNode* EffectNode)
{
	// No Selected Target ( Passed or No selector for this effect node )
	// If PARAM_TARGET is exist, it will override preset generating.
	if (Sequence->Targets.Num() == 0) 
		Sequence->Targets = UDMSEffectNode::GeneratePresetTarget(EffectNode,Sequence);

	for (auto TargetObject : Sequence->Targets)
	{
		UDMSEffectInstance* EffectInstance = NewObject<UDMSEffectInstance>(this);
		EffectInstance->Initialize(EffectNode, Sequence);

		TargetObject->AttachEffectInstance(EffectInstance);

		Sequence->EIs.Add(EffectInstance);
	}
	
	return Sequence->EIs;
}

template <typename FuncFinished>
void UDMSEffectHandler::Resolve(UDMSSequence* Sequence, FuncFinished&& OnResolveCompleted)
{
	DMS_LOG_SCREEN(TEXT("EH : Resolve %s"), *Sequence->GetName());
	
	if (Sequence->EIs.Num() == 0) {
		DMS_LOG_SIMPLE(TEXT("No Resolve Target"));
		OnResolveCompleted();
		return;
	}

	OnResolveCompletedMap.Add(Sequence);
	OnResolveCompletedMap[Sequence].Delegate.BindLambda(OnResolveCompleted);
	OnResolveCompletedMap[Sequence].Count = 0;
	OnResolveCompletedMap[Sequence].IteratingDelegate.BindUObject(this,&UDMSEffectHandler::ApplyNextEffectInstance);
	
	ApplyNextEffectInstance(Sequence);
}

void UDMSEffectHandler::ApplyNextEffectInstance(UDMSSequence* SourceSequence)
{
	if (OnResolveCompletedMap[SourceSequence].Count == SourceSequence->EIs.Num())
		OnResolveCompletedMap[SourceSequence].Delegate.ExecuteIfBound();
	else
		SourceSequence->EIs[OnResolveCompletedMap[SourceSequence].Count++]->Apply(SourceSequence, OnResolveCompletedMap[SourceSequence].IteratingDelegate);
}