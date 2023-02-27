// Fill out your copyright notice in the Description page of Project Settings.

#include "EffectSet/DMSEffect_ModAtt.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Effect/DMSEffectInstance.h"
void UDMSEffect_ModAtt::Work_Implementation(UDMSEffectInstance* iEI)
{
	DMS_LOG_SCREEN(TEXT("%s : ModAtt"), *iEI->GetName());

	IDMSAttributeInterface* AttInterface = Cast<IDMSAttributeInterface>(iEI->GetOuter());

	if (AttInterface==nullptr) {
		DMS_LOG_SCREEN(TEXT("%s : Outer (%s) doesn't implements AttributeInterface"), *iEI->GetName(),*iEI->GetOuter()->GetName());
		return;
	}

	AttInterface->TryModAttribute(Value);

}