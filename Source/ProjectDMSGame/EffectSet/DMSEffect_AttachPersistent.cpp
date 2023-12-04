// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectSet/DMSEffect_AttachPersistent.h"
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
void UDMSEffect_AttachPersistent::Work_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnExecuteCompleted& OnWorkCompleted)
{
	TArray<UDMSEffectInstance*> AddedEIs;

	auto Target = iEI->GetApplyTargetInterface();

	if (Target == nullptr) {
		OnWorkCompleted.ExecuteIfBound(false);
		return;
	}

	//if(bIsUsingSelector){
	//	auto Data = iEI->DataSet->GetData(TAG_DMS_Effect_AttachPersistent);
	//	if (Data!=nullptr && Data->TypeCheck<TArray<uint8>>()){
	//		auto IndexArr = Data->Get<TArray<uint8>>();
	//			
	//		for (auto i : IndexArr) 
	//			AddedEIs.Append(UDMSCoreFunctionLibrary::GetDMSEffectHandler()->CreateEffectInstance(SourceSequence->GetSourceObject(), SourceSequence->GetSourcePlayer(), Target->GetObject(), EIEffects[i]->GetEffectNode()));
	//	}
	//}
	//else{
	//	for (auto Node : EIEffects)
	//		AddedEIs.Append(UDMSCoreFunctionLibrary::GetDMSEffectHandler()->CreateEffectInstance(SourceSequence->GetSourceObject(), SourceSequence->GetSourcePlayer(), Target->GetObject(), Node->GetEffectNode()));
	//}
	//
	//SourceSequence->AddToOnSequenceFinished_Native([AddedEIs](bool){
	//	for (auto& EI : AddedEIs)	{EI->ChangeEIState(EDMSEIState::EIS_Persistent);}
	//});

	OnWorkCompleted.ExecuteIfBound(true);
}
