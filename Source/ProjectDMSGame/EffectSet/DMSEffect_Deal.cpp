// Fill out your copyright notice in the Description page of Project Settings.

#include "EffectSet/DMSEffect_Deal.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Effect_DealDamage, "Effect.ModAttribute.DealDamage");

UDMSEffect_Deal::UDMSEffect_Deal() 
{ 
	EffectTag = TAG_DMS_Effect_DealDamage;
	Value.ModifierType = EDMSModifierType::MT_Sub; 
}

//void UDMSEffect_Deal::Work_Implementation(UDMSEffectInstance* iEI,  const FOnWorkCompletedDynamic& OnWorkCompleted)
//{
//	//REQUIREMENT CHECK
//
//	//Next
//}