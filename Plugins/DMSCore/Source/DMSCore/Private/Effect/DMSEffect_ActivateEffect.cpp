// Fill out your copyright notice in the Description page of Project Settings.

#include "Effect/DMSEffect_ActivateEffect.h"
#include "Common/DMSCommonTags.h"

#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEIManagerComponent.h"
#include "GameModes/DMSGameModeBase.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Sequence/DMSSequence.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Effect_ActivateEffect, "Effect.ActivateEffect");

UDMSEffect_ActivateEffect::UDMSEffect_ActivateEffect()
{ 
	EffectTag = TAG_DMS_Effect_ActivateEffect;
}
UDMSEffect_ActivateEffect_Static::UDMSEffect_ActivateEffect_Static() : UDMSEffect_ActivateEffect(),UseEffectFromOuter(false), EffectIdx(0)
{ 
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

UDMSEffectSet* UDMSEffect_ActivateEffect_Static::GetEffectSetFromOuter(UDMSEffectInstance* iEI)
{
	// Outer Validation
	auto tOuter = iEI->GetApplyTarget();
	if (tOuter == nullptr) { /*DMS_LOG_SCREEN(TEXT("%s : tOuter is Null"), *iEI->GetName());*/ return nullptr; }

	return tOuter != nullptr ? tOuter->GetOwningEffectSet(EffectSetName) : nullptr;
}

UDMSEffect_ActivateEffect_Variable::UDMSEffect_ActivateEffect_Variable()
{
	//SelectorData.ValueSelector = CreateDefaultSubobject<UDMSValueSelectorDefinition_Effect>("ValueSelector");
}

bool UDMSEffect_ActivateEffect_Variable::GetEffectNodeWrapper(UDMSEffectInstance* iEI, UDMSEffectNodeWrapper*& OutWrapper)
{ 
	// UseEffectFromOuter 상황별 분기 포함시켜야함
	UDMSDataObject* rData = SelectorData.Get(iEI->DataSet);

	if (rData == nullptr) return false;

	// Get Input Data ( Skip if data doesn't exist. )
	else {
		if (rData->TypeCheck<UDMSEffectNodeWrapper*>())	{
			OutWrapper = rData->Get<UDMSEffectNodeWrapper*>();
			return true; 
		}
		else	
			return false;
	}
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

TArray<UDMSDataObject*> UDMSSelectorRequestGenerator_AE::GenerateCandidates(UDMSSequence* Sequence, UDMSEffectInstance* TargetEI)
{	
	// 프로퍼티로 스태틱중에서 고르기 or EI Outer에서 고르기 의 분기 나누기.

	TArray<UDMSDataObject*> rv;
	if (UseEffectFromOuter) {
		if (TargetEI != nullptr)
			rv = MakeDataArray(TargetEI);
	
		else {
			for ( auto& EI :Sequence->GetAllEIs())
				rv.Append(MakeDataArray(EI));
		}
	}
	else {
		for (auto& EN : StaticEffects)
		{
			auto NewData = NewObject<UDMSDataObject>(TargetEI == nullptr ? (UObject*)Sequence : (UObject*)TargetEI);
			NewData->Set((UObject*)EN);
			rv.Add(NewData);
		}
	}
	return rv;
}

TArray<UDMSDataObject*> UDMSSelectorRequestGenerator_AE::MakeDataArray(UDMSEffectInstance* TargetEI)
{
	TArray<UDMSDataObject*> rv;
	auto tOuter = TargetEI->GetApplyTarget();

	if(tOuter == nullptr) { /*DMS_LOG_SCREEN(TEXT("%s : tOuter is Null"), *iEI->GetName());*/ return rv; }

	for(auto& EN : tOuter->GetOwningEffectSet(EffectSetTag)->EffectNodes)
	{
		auto NewData = NewObject<UDMSDataObject>(TargetEI);
		NewData->Set((UObject*)EN);
		rv.Add(NewData);
	}

	return rv;
}
