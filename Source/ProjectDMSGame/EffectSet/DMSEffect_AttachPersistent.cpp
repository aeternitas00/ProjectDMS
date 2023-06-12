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
	OutDataKey = TAG_DMS_Effect_AttachPersistent;
	bHasPairedSelector = true;
}
void UDMSEffect_AttachPersistent::Work_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnWorkCompleted& OnWorkCompleted)
{
	TArray<UDMSEffectInstance*> AddedEIs;

	auto Target = iEI->GetApplyTarget();

	if (Target == nullptr) {
		OnWorkCompleted.ExecuteIfBound(SourceSequence, false);
		return;
	}

	if(bIsUsingSelector){
		auto Data = iEI->DataSet->GetData(TAG_DMS_Effect_AttachPersistent);
		if (Data!=nullptr && Data->TypeCheck<TArray<uint8>>()){
			auto IndexArr = Data->Get<TArray<uint8>>();
				
			for (auto i : IndexArr) 
				AddedEIs.Append(UDMSCoreFunctionLibrary::GetDMSEffectHandler()->CreateEffectInstance(SourceSequence->GetSourceObject(), SourceSequence->GetSourcePlayer(), Target->GetObject(), EIEffects[i]->GetEffectNode()));
		}
	}
	else{
		for (auto Node : EIEffects)
			AddedEIs.Append(UDMSCoreFunctionLibrary::GetDMSEffectHandler()->CreateEffectInstance(SourceSequence->GetSourceObject(), SourceSequence->GetSourcePlayer(), Target->GetObject(), Node->GetEffectNode()));
	}
	
	SourceSequence->AddToOnSequenceFinished_Native([AddedEIs](){
		for (auto& EI : AddedEIs)	{EI->ChangeEIState(EDMSEIState::EIS_Persistent);}
	});
	OnWorkCompleted.ExecuteIfBound(SourceSequence,true);
}

void UDMSEffect_AttachPersistent::InitializePairedSelector(UDMSEffectElementSelectorWidget* WidgetInstance)
{
	auto NewDataObj = NewObject<UDMSDataObject>();
	TArray<UObject*> tArr;
	for (UObject* i : EIEffects) tArr.Add(i);
	NewDataObj->Set(tArr);
	WidgetInstance->CandidatesData.Add(NewDataObj);

}

UDMSDataObjectSet* UDMSSelector_AttachPersistent::MakeOutputData_Implementation()
{
	UDMSDataObjectSet* rv = NewObject<UDMSDataObjectSet>(OwnerSeq);

	rv->SetData(SourceEffectDefinition->OutDataKey, SelectedIndex);

	return rv;
}
