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

void UDMSEffect_MoveLocatable::Work_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnWorkCompleted& OnWorkCompleted)
{
	UObject* DestLocation;

	AActor* ApplyTarget = iEI->GetTypedOuter<AActor>();

	if (!ApplyTarget->Implements<UDMSLocatableInterface>() )
	{
		OnWorkCompleted.ExecuteIfBound(SourceSequence);
		return;
	}

	if ( !SourceSequence->EIDatas->GetValidDataValue<UObject*>(EffectTag, DestLocation) )
	{
		OnWorkCompleted.ExecuteIfBound(SourceSequence);
		return;
	}

	IDMSLocatableInterface::Execute_LocatingTo(ApplyTarget, Cast<ADMSLocationBase>(DestLocation));

	OnWorkCompleted.ExecuteIfBound(SourceSequence);
}

AActor* UDMSEffect_MoveLocatable::GetPlayerFocusTarget_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI)
{
	// Default source target is Effect's Target.
	UObject* FocusTarget;
	if (!SourceSequence->EIDatas->GetValidDataValue<UObject*>(EffectTag, FocusTarget)) return nullptr;
	return Cast<AActor>(FocusTarget);

}

