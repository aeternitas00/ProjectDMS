// Copyright JeongWoo Lee


#include "EffectSet/DMSEffect_MoveLocatable.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "GameModes/DMSGameState.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Attribute/DMSAttributeValue_Object.h"
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

void UDMSEffect_MoveLocatable::Work_Implementation(ADMSSequence* SourceSequence, ADMSActiveEffect* iEI, const FOnExecuteCompleted& OnWorkCompleted)
{
	AActor* ApplyTarget = Cast<AActor>(iEI->GetApplyTargetInterface()->GetObject());

	if (!ApplyTarget->Implements<UDMSLocatableInterface>() )
	{
		OnWorkCompleted.ExecuteIfBound(false);
		return;
	}

	auto SeqAttComp = SourceSequence->GetComponentByClass<UDMSAttributeComponent>();
	auto DestAttribute = SeqAttComp ? SeqAttComp->GetTypedAttributeValue<UDMSAttributeValue_Object>(EffectTag.GetSingleTagContainer()) : nullptr;
	auto DestLocation = DestAttribute ? DestAttribute->GetValue()[0] : nullptr;
	
	if ( !DestLocation )
	{
		OnWorkCompleted.ExecuteIfBound(false);
		return;
	}

	bool retval = IDMSLocatableInterface::Execute_LocatingTo(ApplyTarget, Cast<ADMSLocationBase>(DestLocation));

	OnWorkCompleted.ExecuteIfBound(retval);
}

