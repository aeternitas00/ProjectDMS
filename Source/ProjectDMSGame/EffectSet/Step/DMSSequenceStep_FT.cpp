#include "EffectSet/Step/DMSSequenceStep_FT.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "GameModes/DMSGameStateBase.h"

FGameplayTag UDMSSequenceStep_FT::GetPureStepTag_Implementation() const
{
	return FGameplayTag::RequestGameplayTag("Step.Arkham.FreeTrigger");
}

//TScriptInterface<IDMSEffectorInterface> UDMSSelector_FT::GetTriggerableEffectors()
//{
//	return TScriptInterface<IDMSEffectorInterface>();
//
//	// additional condition check with owner step and find ft-able effectors.
//
//}

void UDMSSequenceStep_FT::Progress_FT(UDMSSequenceStep* InstancedStep)
{
	BroadcastProgress(InstancedStep,FName(NAME_None));
	//auto GS = UDMSCoreFunctionLibrary::GetDMSGameState(InstancedStep); check(GS);
	//auto SM = GS->GetSequenceManager(); check(SM);
	//auto EH = GS->GetEffectHandler(); check(EH);
	//auto SelM = GS->GetSelectorManager(); check(SelM);

	//TArray<FDMSSelectorRequestForm> DecisionForms;

	//GS->PlayerArray()

	//for ( auto& DD : DecisionDefinitions )
	//	DecisionForms.Append(DD.SetupRequestForm(InstancedStep->OwnerSequence));

	//if ( WidgetOwner == nullptr || !WidgetOwner->SetupWidgetQueue(InstancedStep->OwnerSequence, SelM->RequestCreateSelectors(DecisionForms))) {
	//	DMS_LOG_SIMPLE(TEXT("==== %s : EI Data Selection Canceled  ===="), *InstancedStep->OwnerSequence->GetName());

	//	InstancedStep->ProgressEnd(false); return;
	//}

	//RunWidgetQueue(InstancedStep, WidgetOwner, [=](UDMSSequence* pSequence) {
	//	// Run sequence ( Notifying steps and apply )
	//	DMS_LOG_SIMPLE(TEXT("==== %s : EI Data Selection Completed  ===="), *pSequence->GetName());
	//	InstancedStep->ProgressEnd(); return;
	//	});
}
