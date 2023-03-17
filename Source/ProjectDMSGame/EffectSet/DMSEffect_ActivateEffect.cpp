// Fill out your copyright notice in the Description page of Project Settings.

#include "EffectSet/DMSEffect_ActivateEffect.h"

#include "Attribute/DMSAttributeComponent.h"
#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEIManagerComponent.h"
#include "GameModes/DMSGameMode.h"
#include "Card/DMSCardBase.h"
#include "Card/DMSCardDefinition.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "DMSEffect_ActivateEffect.h"
#include "Sequence/DMSSequence.h"


UDMSEffect_ActivateEffect::UDMSEffect_ActivateEffect() :EffectIdx(0)
{ 
	Keyword = TEXT("ActivateEffect"); 
	EffectSetName = TEXT("Effect");
	bHasPairedSelector=true;
	//PairedSelector = UDMSSelector_ActivateEffect::StaticClass();
}

void UDMSEffect_ActivateEffect::Work_Implementation(UDMSEffectInstance* iEI)
{
	DMS_LOG_SCREEN(TEXT("%s : %s"), *iEI->GetName(), *Keyword.ToString());

	UDMSSeqManager* SeqMan = UDMSCoreFunctionLibrary::GetDMSSequenceManager();
	if(SeqMan==nullptr) { DMS_LOG_SCREEN(TEXT("%s : Seqman is nullptr"), *iEI->GetName()); return;}

	auto Set= GetEffectSetFromOuter(iEI);
	if (Set == nullptr) { /*DMS_LOG_SCREEN(TEXT("%s : Set is Null"), *iEI->GetOuter()->GetName());*/ return; }
	
	// Use Preset Idx;
	uint8 Idx = EffectIdx;
	
	// Override with Input Data ( Skip if data doesn't exist. )
	UDMSDataObject* rData = iEI->DataSet->GetData(ReferenceDataName);
	if (rData != nullptr && rData->TypeCheck<uint8>() && rData->Get<uint8>() < Set->EffectNodes.Num()) {
		Idx = rData->Get<uint8>();
	}

	// Get proper EffectNode from EffectSet
	auto NodeWrapper = (Set != nullptr && Set->EffectNodes.Num()>Idx ) ? Set->EffectNodes[Idx] : nullptr;
	if (NodeWrapper == nullptr) { /*DMS_LOG_SCREEN(TEXT("%s : Can't Find Effect Node"), *iEI->GetName() );*/ return;}
	//DMS_LOG_SCREEN(TEXT("%s : Found Effect Node"), *iEI->GetName());

	auto Node = NodeWrapper->GetEffectNode();
	SeqMan->RequestCreateSequence(iEI->SourceObject, iEI->SourceController,Node,{} );

}

void UDMSEffect_ActivateEffect::InitializePairedSelector(UDMSEffectElementSelectorWidget* WidgetInstance)
{
	auto CastedWidget = Cast<UDMSSelector_ActivateEffect>(WidgetInstance);
	if (CastedWidget == nullptr) return;
	CastedWidget->OutDataName = ReferenceDataName;
}

UDMSEffectSet* UDMSEffect_ActivateEffect::GetEffectSetFromOuter(UDMSEffectInstance* iEI)
{
	// Outer Validation
	auto tOuter = Cast<IDMSEffectorInterface>(iEI->GetOuter());
	if (tOuter == nullptr) { DMS_LOG_SCREEN(TEXT("%s : tOuter is Null"), *iEI->GetName()); return nullptr; }

	return tOuter != nullptr ? tOuter->GetOwningEffectSet(EffectSetName) : nullptr;
}


// 타겟이 여러개일때?
bool UDMSEffect_ActivateEffect::GetCandidates_Implementation(UDMSSequence* iSeq, TArray<UDMSDataObject*>& outDataObj)
{
	//return false;

	for (auto EI : iSeq->EIs)
	{
		auto Set = GetEffectSetFromOuter(EI);
		if (Set == nullptr) 
		{ DMS_LOG_SCREEN(TEXT("%s : Set is Null"), *EI->GetOuter()->GetName()); outDataObj.Empty(); return false; }

		auto NewDataObj = NewObject<UDMSDataObject>();
		TArray<UObject*> tArr;
		for (UObject* i : Set->EffectNodes) tArr.Add(i);
		NewDataObj->Set(tArr);

		outDataObj.Add(NewDataObj);
	}

	return true; 
}
