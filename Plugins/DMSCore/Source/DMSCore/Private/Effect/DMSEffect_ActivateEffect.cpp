// Fill out your copyright notice in the Description page of Project Settings.

#include "Effect/DMSEffect_ActivateEffect.h"
#include "Common/DMSCommonTags.h"

#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEIManagerComponent.h"
#include "GameModes/DMSGameModeBase.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Sequence/DMSSequence.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Effect_ActivateEffect, "Effect.ActivateEffect");

UDMSEffect_ActivateEffect::UDMSEffect_ActivateEffect() : UseEffectFromOuter(false), EffectIdx(0)
{ 
	EffectTag = TAG_DMS_Effect_ActivateEffect;
	EffectSetName = TAG_DMS_EffectType_Effect;
}

void UDMSEffect_ActivateEffect::Work_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnExecuteCompleted& OnWorkCompleted)
{
	//DMS_LOG_SCREEN(TEXT("%s : %s"), *iEI->GetName(), *EffectTag.ToString());

	UDMSSeqManager* SeqMan = UDMSCoreFunctionLibrary::GetDMSSequenceManager();
	if(SeqMan==nullptr) { /*DMS_LOG_SCREEN(TEXT("%s : Seqman is nullptr"), *iEI->GetName());*/OnWorkCompleted.ExecuteIfBound(false); return;}
	
	UDMSEffectNodeWrapper* NodeWrapper = nullptr;
	
	if ( !GetEffectNodeWrapper(iEI,NodeWrapper) || NodeWrapper == nullptr || NodeWrapper->GetEffectNode() == nullptr) {
		DMS_LOG_SIMPLE(TEXT("==== %s : ACTIVATE EFFECT WORK CANCELED ===="), *SourceSequence->GetName());
		OnWorkCompleted.ExecuteIfBound(false);
		return;
	}

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

UDMSEffect_ActivateEffect_Variable::UDMSEffect_ActivateEffect_Variable()
{
	SelectorData.ValueSelector = CreateDefaultSubobject<UDMSValueSelectorDefinition_Effect>("ValueSelector");
}

bool UDMSEffect_ActivateEffect_Variable::GetEffectNodeWrapper(UDMSEffectInstance* iEI, UDMSEffectNodeWrapper*& OutWrapper)
{ 
	UDMSDataObject* rData = iEI->DataSet->GetData(SelectorData.OutDataKey);
	uint8 Idx = UINT8_MAX;	
	auto Selector = Cast<UDMSValueSelectorDefinition_Effect>(SelectorData.ValueSelector); 
	UDMSEffectSet* Set = UseEffectFromOuter ? GetEffectSetFromOuter(iEI) : Selector->EffectSet;

	// Get Input Data ( Skip if data doesn't exist. )
	if (rData != nullptr && rData->TypeCheck<uint8>())	Idx = rData->Get<uint8>();
	else	return false;
	
	if (Set != nullptr && Set->EffectNodes.Num() > Idx)	OutWrapper = Set->EffectNodes[Idx];
	else	return false;

	return true; 
	
}

bool UDMSEffect_ActivateEffect_Static::GetEffectNodeWrapper(UDMSEffectInstance* iEI, UDMSEffectNodeWrapper*& OutWrapper) 
{ 
	if (UseEffectFromOuter){
		auto Set = GetEffectSetFromOuter(iEI);
		if (Set != nullptr && Set->EffectNodes.Num() > EffectIdx)
			OutWrapper = Set->EffectNodes[EffectIdx];
		else
			return false;
	}
	else
		OutWrapper = StaticEffect;

	return true; 
}
