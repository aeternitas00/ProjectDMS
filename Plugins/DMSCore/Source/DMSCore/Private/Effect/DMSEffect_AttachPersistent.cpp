// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/DMSEffect_AttachPersistent.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Effect/DMSEffectHandler.h"
#include "Effect/DMSEffectInstance.h"
#include "Sequence/DMSSequence.h"
#include "Effect/DMSEffectorInterface.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Effect_AttachPersistent, "Effect.AttachPersistent");

//FGameplayTagContainer UDMSEffect_AttachPersistent::GetEffectTags_Implementation()
//{
//	return FGameplayTagContainer();
//}

UDMSEffect_AttachPersistent::UDMSEffect_AttachPersistent() {
	EffectTag = TAG_DMS_Effect_AttachPersistent;
}
void UDMSEffect_AttachPersistent::Work_Implementation(UDMSSequence* SourceSequence, ADMSActiveEffect* iEI, const FOnExecuteCompleted& OnWorkCompleted)
{
	auto Target = iEI->GetApplyTargetInterface();

	if (Target == nullptr) {
		OnWorkCompleted.ExecuteIfBound(false);
		return;
	}
	auto EH = UDMSCoreFunctionLibrary::GetDMSEffectHandler();
	check(EH);

	for (auto Node : AttachingEffects){
		EH->CreatePersistentActiveEffect(SourceSequence->GetSourceObject(), SourceSequence->GetSourcePlayer(), Target->GetObject(), Node->GetEffectNode());
	}
	//if(bIsUsingSelector){
	//	auto Data = iEI->DataSet->GetData(TAG_DMS_Effect_AttachPersistent);
	//	if (Data!=nullptr && Data->TypeCheck<TArray<uint8>>()){
	//		auto IndexArr = Data->Get<TArray<uint8>>();
	//			
	//		for (auto i : IndexArr) 
	//			EH->CreatePersistentActiveEffect(SourceSequence->GetSourceObject(), SourceSequence->GetSourcePlayer(), Target->GetObject(), AttachingEffects[i]->GetEffectNode());
	//	}
	//}
	//else{
	//	
	//}
	//
	//SourceSequence->AddToOnSequenceFinished_Native([AddedEIs](bool){
	//	for (auto& EI : AddedEIs)	{EI->ToggleEIState(EDMSAEState::AES_Persistent);}
	//});

	OnWorkCompleted.ExecuteIfBound(true);
}
