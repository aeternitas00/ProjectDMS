// Fill out your copyright notice in the Description page of Project Settings.

#include "Effect/DMSEffect_ActivateEffect.h"
#include "Common/DMSCommonTags.h"

#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEIManagerComponent.h"
#include "GameModes/DMSGameModeBase.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Sequence/DMSSequence.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Effect_ActivateEffect, "Effect.ActivateEffect");

UDMSEffect_ActivateEffect::UDMSEffect_ActivateEffect() :EffectIdx(0)
{ 
	EffectSetName = TAG_DMS_EffectType_Effect;
	EffectTag = TAG_DMS_Effect_ActivateEffect;
	bHasPairedSelector=true;
}

void UDMSEffect_ActivateEffect::Work_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnExecuteCompleted& OnWorkCompleted)
{
	//DMS_LOG_SCREEN(TEXT("%s : %s"), *iEI->GetName(), *EffectTag.ToString());

	UDMSSeqManager* SeqMan = UDMSCoreFunctionLibrary::GetDMSSequenceManager();
	if(SeqMan==nullptr) { /*DMS_LOG_SCREEN(TEXT("%s : Seqman is nullptr"), *iEI->GetName());*/OnWorkCompleted.ExecuteIfBound(false); return;}

	auto Set= GetEffectSetFromOuter(iEI);
	if (Set == nullptr) { 
		//DMS_LOG_SCREEN(TEXT("%s : Set is Null"), *iEI->GetApplyTarget()->GetName());
		DMS_LOG_SIMPLE(TEXT("==== %s : ACTIVATE EFFECT WORK CANCELED ===="), *SourceSequence->GetName());
		OnWorkCompleted.ExecuteIfBound(false);
		return; 
	}
	
	// Use Preset Idx;
	uint8 Idx = EffectIdx;
	
	// Override with Input Data ( Skip if data doesn't exist. )
	UDMSDataObject* rData = iEI->DataSet->GetData(EffectTag);
	if (rData != nullptr && rData->TypeCheck<uint8>() && rData->Get<uint8>() < Set->EffectNodes.Num()) {
		Idx = rData->Get<uint8>();
	}

	// Get proper EffectNode from EffectSet
	auto NodeWrapper = (Set != nullptr && Set->EffectNodes.Num()>Idx ) ? Set->EffectNodes[Idx] : nullptr;
	if (NodeWrapper == nullptr) { 
		/*DMS_LOG_SCREEN(TEXT("%s : Can't Find Effect Node"), *iEI->GetName() );*/ 
		DMS_LOG_SIMPLE(TEXT("==== %s : ACTIVATE EFFECT WORK CANCELED ===="), *SourceSequence->GetName());
		OnWorkCompleted.ExecuteIfBound(false);
		return;
	}
	//DMS_LOG_SCREEN(TEXT("%s : Found Effect Node"), *iEI->GetName());

	auto Node = NodeWrapper->GetEffectNode();

	DMS_LOG_SIMPLE(TEXT("==== %s : ACTIVATE EFFECT WORK START ===="), *SourceSequence->GetName());
	auto NewSeq = SeqMan->RequestCreateSequence(SourceSequence->GetSourceObject(), SourceSequence->GetSourcePlayer(), Node, {}, nullptr);
	
	// 차일드 노드 끝날때 박는게 아니고 파라미터로 델리게이트를 넘겨서 패런츠의 ONRESUME을 하는게 나아보임.
	NewSeq->AddToOnSequenceFinished_Native([=](bool ChildSeqSuccessed) {
		DMS_LOG_SIMPLE(TEXT("==== %s : ACTIVATE EFFECT WORK COMPLETED ===="),*SourceSequence->GetName());
		OnWorkCompleted.ExecuteIfBound(ChildSeqSuccessed);
	});
	SeqMan->RunSequence(NewSeq);
}

UDMSEffectSet* UDMSEffect_ActivateEffect::GetEffectSetFromOuter(UDMSEffectInstance* iEI)
{
	// Outer Validation
	auto tOuter = iEI->GetApplyTarget();
	if (tOuter == nullptr) { /*DMS_LOG_SCREEN(TEXT("%s : tOuter is Null"), *iEI->GetName());*/ return nullptr; }

	return tOuter != nullptr ? tOuter->GetOwningEffectSet(EffectSetName) : nullptr;
}

void UDMSEffect_ActivateEffect::InitializePairedSelector(UDMSEffectElementSelectorWidget* WidgetInstance)
{
	auto CastedWidget = Cast<UDMSSelector_ActivateEffect>(WidgetInstance);
	if (CastedWidget == nullptr) return;
	
	//CastedWidget->OutDataName = OutDataKey;

	auto& iSeq = WidgetInstance->OwnerSeq;
	
	for (auto& EI : iSeq->GetAllEIs())
	{
		auto Set = GetEffectSetFromOuter(EI);
		if (Set == nullptr) 
		{ /*DMS_LOG_SCREEN(TEXT("%s : Set is Null"), *EI->GetApplyTarget()->GetName());*/ WidgetInstance->CandidatesData.Empty(); return; }

		auto NewDataObj = NewObject<UDMSDataObject>();
		TArray<UObject*> tArr;
		for (UObject* i : Set->EffectNodes) tArr.Add(i);
		NewDataObj->Set(tArr);

		WidgetInstance->CandidatesData.Add(NewDataObj);
	} 
}
//
//bool UDMSEffect_ActivateEffect::GetCandidates_Implementation(UDMSSequence* iSeq, TArray<UDMSDataObject*>& outDataObj)
//{
//	//return false;
//
//
//}

UDMSDataObjectSet* UDMSSelector_ActivateEffect::MakeOutputData_Implementation()
{ 
	return NewObject<UDMSDataObjectSet>(); 
}
