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

	//DMS_LOG_SCREEN(TEXT("EH: Generic CreateEI Start To %s"), *Sequence->SourceObject->GetName());
	// No Selected Target ( Passed or No selector for this effect node )
	if (Sequence->Targets.Num() == 0) 
	{
		//DMS_LOG_SCREEN(TEXT("EH: Generic CreateEI Meets no target %s"), *Sequence->SourceObject->GetName());
		TArray<TScriptInterface<IDMSEffectorInterface>> TempTarget;

		switch(EffectNode->PresetTargetFlag)
		{
			case EDMSPresetTargetFlag::PTF_Self:
				if (Sequence->SourceObject->Implements<UDMSEffectorInterface>()){
					TempTarget.Add(TScriptInterface<IDMSEffectorInterface>(Sequence->SourceObject));
					DMS_LOG_SCREEN(TEXT("EH: CreateEI [%s] To %s"), *Sequence->GetName(),*Sequence->SourceObject->GetName());
					}
				break;
			case EDMSPresetTargetFlag::PTF_OC:
				if (Sequence->SourceController->Implements<UDMSEffectorInterface>()){
					TempTarget.Add(TScriptInterface<IDMSEffectorInterface>(Sequence->SourceController));
					DMS_LOG_SCREEN(TEXT("EH: CreateEI [%s] To %s"), *Sequence->GetName(), *Sequence->SourceController->GetName());
				}
				break;
			//case EDMSPresetTargetFlag::PTF_Data:
			//	TargetData = Sequence->EIDatas->GetData("PresetTarget");
			//	
			//	TempTarget = (TargetData != nullptr && TargetData->TypeCheck<TArray<TScriptInterface<IDMSEffectorInterface>>>()) ? 
			//		TargetData->Get<TArray<TScriptInterface<IDMSEffectorInterface>>>() : 
			//		TArray<TScriptInterface<IDMSEffectorInterface>>();
			//	DMS_LOG_SCREEN(TEXT("EH: CreateEI [%s] To Data->PresetTarget (Num : %d)"), *Sequence->GetName(), TempTarget.Num());
			//	break;
			case EDMSPresetTargetFlag::PTF_Effect:
				TempTarget=EffectNode->GenerateTarget(Sequence);
				DMS_LOG_SCREEN(TEXT("EH: CreateEI [%s] To EN's target (Num : %d)"), *Sequence->GetName(), TempTarget.Num());
				break;
			default:
				break;
		}
		Sequence->Targets = std::move(TempTarget);
	}
	

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