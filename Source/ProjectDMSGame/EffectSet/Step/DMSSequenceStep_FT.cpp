// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectSet/Step/DMSSequenceStep_FT.h"

FGameplayTag UDMSSequenceStep_FT::GetPureStepTag_Implementation() const
{
	return FGameplayTag::RequestGameplayTag("Step.Arkham.FreeTrigger");
}

TScriptInterface<IDMSEffectorInterface> UDMSSelector_FT::GetTriggerableEffectors()
{
	return TScriptInterface<IDMSEffectorInterface>();

	// additional condition check with owner step and find ft-able effectors.

}
