// Copyright JeongWoo Lee


#include "Phase/DMSPhaseManager.h"
#include "Phase/DMSPhase.h"

void UDMSPhaseManager::ProceedToNextPhase(FGameplayTag JumpTarget = FGameplayTag::EmptyTag)
{
	CurrentPhase->EndPhase();
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
	CurrentPhase->StartPhase();
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
		//CurrentPhase->OnPhaseEnded_Native.AddUObject(this,&UDMSPhaseManager::UpdatePhaseWidget);
	}
	CurrentPhase->NextPhase = RootPhase;

	auto StartIndexTemp = StartIndex % DefaultPhaseList.Num();
	CurrentPhase = RootPhase;
	for (uint8 i = 0; i < StartIndexTemp; i++) {
		CurrentPhase = CurrentPhase->NextPhase;
	}

}

void UDMSPhaseManager::SetupPhases_Implementation()
{
	auto it = CurrentPhase;
	if (CurrentPhase==nullptr) return;
	do {
		// Do Something 

		it = it->NextPhase;
	}
	while (it != CurrentPhase);
}
