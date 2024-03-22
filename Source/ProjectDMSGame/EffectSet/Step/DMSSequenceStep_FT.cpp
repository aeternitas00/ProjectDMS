// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectSet/Step/DMSSequenceStep_FT.h"

FGameplayTagContainer UDMSSequenceStep_FT::GetStepTag_Implementation() const
{
	return FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Step.Arkham.FreeTrigger"));
}
