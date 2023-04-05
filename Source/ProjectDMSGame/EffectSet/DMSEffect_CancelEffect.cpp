// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectSet/DMSEffect_CancelEffect.h"
#include "Sequence/DMSSequence.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Effect_CancelEffect, "Effect.CancelEffect");

//UDMSEffect_CancelEffect::UDMSEffect_CancelEffect()
//{
//}

void UDMSEffect_CancelEffect::Work_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnWorkCompleted& OnWorkCompleted)
{
	SourceSequence->ParentSequence->bIsActive=false;
	OnWorkCompleted.ExecuteIfBound(SourceSequence);
}
