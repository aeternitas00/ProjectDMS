// Fill out your copyright notice in the Description page of Project Settings.


#include "Phase/DMSPhaseManager.h"
#include "Phase/DMSPhase.h"

void UDMSPhaseManager::ProceedToNextPhase(FGameplayTag JumpTarget = FGameplayTag::EmptyTag)
{
	CurrentPhase->PhaseEnd();
	if (JumpTarget== FGameplayTag::EmptyTag){
		CurrentPhase = CurrentPhase->NextPhase;
	}
	else {
		auto Start = CurrentPhase;
		auto It = CurrentPhase;
		do {
			if ( It->PhaseTag == JumpTarget ) {CurrentPhase = It; break;}
			It = Start->NextPhase;
		} while (Start != It);

	}
	CurrentPhase->PhaseStart();
}

void UDMSPhaseManager::GeneratePhases()
{
	CurrentPhase = nullptr;
	if (DefaultPhaseList.Num() == 0) return;
	UDMSPhase* RootPhase=nullptr;
	for (auto& PhaseClass : DefaultPhaseList)
	{
		if (CurrentPhase != nullptr) {
			CurrentPhase->NextPhase = NewObject<UDMSPhase>(this, PhaseClass);
			CurrentPhase = CurrentPhase->NextPhase;
		}
		else {
			CurrentPhase = NewObject<UDMSPhase>(this, PhaseClass);
			RootPhase = CurrentPhase;
		}
		CurrentPhase->OnPhaseEnded_Native.AddUObject(this,&UDMSPhaseManager::UpdatePhaseWidget);
	}
	CurrentPhase->NextPhase = RootPhase;

	auto StartIndexTemp = StartIndex % DefaultPhaseList.Num();
	CurrentPhase = RootPhase;
	for (uint8 i = 0; i < StartIndexTemp; i++) {
		CurrentPhase = CurrentPhase->NextPhase;
	}

}

//void UDMSPhaseManager::SetupPhases_Implementation()
//{
//	auto it = CurrentPhase;
//	if (CurrentPhase==nullptr) return;
//	do {
//		it->OnPhaseStarted_Native.AddUObject(this,&UDMSPhaseManager::BroadCastPhaseStarted);
//		it->OnPhaseEnded_Native.AddUObject(this, &UDMSPhaseManager::BroadCastPhaseEnded);
//		//for (auto& OPS : OnPhaseStart)it->OnPhaseStarted.Add(OPS);
//		//for (auto& OPE : OnPhaseEnd)it->OnPhaseEnded.Add(OPE);
//		it = it->NextPhase;
//	}
//	while (it != CurrentPhase);
//}
