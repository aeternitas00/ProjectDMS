// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectSet/Options/DMSEffectOption_Focus.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Effect/DMSEffectInstance.h"
#include "Gamemodes/DMSGameState.h"
#include "Sequence/DMSSequence.h"
#include "Common/DMSTargetGenerator.h"

UDMSEffectOption_Focus::UDMSEffectOption_Focus()
{
	FocusTarget = CreateDefaultSubobject<UDMSTargetGenerator_Caller>("FocusTarget");
}

void UDMSEffectOption_Focus::OnExecuteOption_Implementation(UDMSSequence* iSourceSeq, ADMSActiveEffect* iEI, const FOnOptionCompleted& OnOptionCompleted)
{
	DMS_LOG_SIMPLE(TEXT("FOCUS"));
	AActor* Target = Cast<AActor>(FocusTarget->GetTargets(iEI, iSourceSeq)[0]);
	if (Target == nullptr) { OnOptionCompleted.Execute(this); return;}
	Cast<ADMSGameState>(UDMSCoreFunctionLibrary::GetDMSGameState(iEI))->SetPlayersFocusTarget(Target);
	OnOptionCompleted.Execute(this);
}
