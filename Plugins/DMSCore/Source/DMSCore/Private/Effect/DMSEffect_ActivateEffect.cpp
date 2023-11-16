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
UDMSEffect_ActivateEffect_Static::UDMSEffect_ActivateEffect_Static() : UDMSEffect_ActivateEffect(),UseEffectFromOuter(false)
{ 
	EffectSetName = TAG_DMS_EffectType_Effect;
}

void UDMSEffect_ActivateEffect::Work_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnExecuteCompleted& OnWorkCompleted)
{
	//DMS_LOG_SCREEN(TEXT("%s : %s"), *iEI->GetName(), *EffectTag.ToString());

	UDMSSeqManager* SeqMan = UDMSCoreFunctionLibrary::GetDMSSequenceManager();
	if(SeqMan==nullptr) { /*DMS_LOG_SCREEN(TEXT("%s : Seqman is nullptr"), *iEI->GetName());*/OnWorkCompleted.ExecuteIfBound(false); return;}
	
	TArray<UDMSEffectNodeWrapper*> NodeWrappers;
	
	if ( !GetEffectNodeWrappers(iEI,NodeWrappers)) {
		DMS_LOG_SIMPLE(TEXT("==== %s : ACTIVATE EFFECT WORK CANCELED ===="), *SourceSequence->GetName());
		OnWorkCompleted.ExecuteIfBound(false);
		return;
	}

	DMS_LOG_SIMPLE(TEXT("==== %s : ACTIVATE EFFECT WORK START ===="), *SourceSequence->GetName());

	TArray<UDMSSequence*> Sequences;

	for ( auto& NodeWrapper : NodeWrappers )
	{
		auto Node = NodeWrapper->GetEffectNode();
		auto NewSeq = SeqMan->RequestCreateSequence(SourceSequence->GetSourceObject(), SourceSequence->GetSourcePlayer(), Node, {}, nullptr);
		Sequences.Add(NewSeq);
	}

	for ( int i=0; i<Sequences.Num() ; i++ ) 
	{
		if ( i < Sequences.Num()-1){
			Sequences[i]->AddToOnSequenceFinished_Native([=,NextIdx = i+1, NextSequence = Sequences[i+1]](bool ChildSeqSuccessed) {
				DMS_LOG_SIMPLE(TEXT("==== %s : ACTIVATE EFFECT WORK : RUN NEXT EFFECT [%d] ===="),*SourceSequence->GetName(),NextIdx);
				SeqMan->RunSequence(NextSequence);
			});
		}
		else {
			Sequences[i]->AddToOnSequenceFinished_Native([=](bool ChildSeqSuccessed) {
				DMS_LOG_SIMPLE(TEXT("==== %s : ACTIVATE EFFECT WORK COMPLETED ===="),*SourceSequence->GetName());
				OnWorkCompleted.ExecuteIfBound(ChildSeqSuccessed);
			});
		}
	}
	DMS_LOG_SIMPLE(TEXT("==== %s : ACTIVATE EFFECT WORK : RUN FIRST EFFECT ===="),*SourceSequence->GetName());

	SeqMan->RunSequence(Sequences[0]);
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

bool UDMSEffect_ActivateEffect_Variable::GetEffectNodeWrappers(UDMSEffectInstance* iEI, TArray<UDMSEffectNodeWrapper*>& OutWrapperArr)
{ 	
	UDMSDataObject* rData = SelectorData.Get(iEI->DataSet);

	if (rData == nullptr) return false;

	// Get Input Data ( Skip if data doesn't exist. )
	else {
		if (rData->TypeCheck<TArray<UDMSDataObject*>>()) {
			for ( auto& WrapperData : rData->Get<TArray<UDMSDataObject*>>())
			{
				UObject* Wrapper = WrapperData->Get<UObject*>();
				if (Wrapper->IsA<UDMSEffectNodeWrapper>()) OutWrapperArr.Add(Cast<UDMSEffectNodeWrapper>(Wrapper));
			}
			return OutWrapperArr.Num() != 0;
		}
		else	
			return false;
	} }


bool UDMSEffect_ActivateEffect_Static::GetEffectNodeWrappers(UDMSEffectInstance* iEI, TArray<UDMSEffectNodeWrapper*>& OutWrapperArr)
{ 
	if (UseEffectFromOuter){
		auto Set = GetEffectSetFromOuter(iEI);
		for(auto& Idx: EffectIdxArr ) {
			if (Set != nullptr && Set->EffectNodes.Num() > Idx)
				OutWrapperArr.Add(Set->EffectNodes[Idx]);
		}
	}
	else
		OutWrapperArr = StaticEffectArr;

	return OutWrapperArr.Num() != 0;
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
