// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectSet/DMSEffect_MoveLocatable.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "GameModes/DMSGameState.h"
#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEIManagerComponent.h"
#include "Location/DMSLocatableInterface.h"
#include "Location/DMSLocationBase.h"
#include "Sequence/DMSSequence.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Effect_MoveLocatable, "Effect.MoveLocatable");

UDMSEffect_MoveLocatable::UDMSEffect_MoveLocatable()
{ 
	EffectTag = TAG_DMS_Effect_MoveLocatable;
}

void UDMSEffect_MoveLocatable::Work_Implementation(UDMSSequence* SourceSequence, ADMSActiveEffect* iEI, const FOnExecuteCompleted& OnWorkCompleted)
{
	UObject* DestLocation;

	AActor* ApplyTarget = Cast<AActor>(iEI->GetApplyTargetInterface()->GetObject());

	if (!ApplyTarget->Implements<UDMSLocatableInterface>() )
	{
		OnWorkCompleted.ExecuteIfBound(false);
		return;
	}

	if ( !SourceSequence->SequenceDatas->GetValidDataValue<UObject*>(EffectTag, DestLocation) )
	{
		OnWorkCompleted.ExecuteIfBound(false);
		return;
	}

	IDMSLocatableInterface::Execute_LocatingTo(ApplyTarget, Cast<ADMSLocationBase>(DestLocation));

	OnWorkCompleted.ExecuteIfBound(true);
}

