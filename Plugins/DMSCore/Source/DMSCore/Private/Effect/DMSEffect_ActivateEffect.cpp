// Copyright JeongWoo Lee

#include "Effect/DMSEffect_ActivateEffect.h"
#include "Common/DMSCommonTags.h"

#include "Attribute/DMSAttributeComponent.h"
#include "Attribute/DMSAttributeValue_Object.h"

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

void UDMSEffect_ActivateEffect::Work_Implementation(ADMSSequence* SourceSequence, ADMSActiveEffect* iEI, const FOnExecuteCompleted& OnWorkCompleted)
{
	//DMS_LOG_SCREEN(TEXT("%s : %s"), *iAE->GetName(), *EffectTag.ToString());

	UDMSSeqManager* SeqMan = UDMSCoreFunctionLibrary::GetDMSSequenceManager(iEI);
	if(SeqMan==nullptr) { /*DMS_LOG_SCREEN(TEXT("%s : Seqman is nullptr"), *iAE->GetName());*/OnWorkCompleted.ExecuteIfBound(false); return;}
	
	TArray<UDMSEffectNodeWrapper*> NodeWrappers;
	
	if ( !GetEffectNodeWrappers(iEI,NodeWrappers)) {
		DMS_LOG_SIMPLE(TEXT("==== %s : ACTIVATE EFFECT WORK CANCELED ===="), *SourceSequence->GetName());
		OnWorkCompleted.ExecuteIfBound(false);
		return;
	}

	DMS_LOG_SIMPLE(TEXT("==== %s : ACTIVATE EFFECT WORK START ===="), *SourceSequence->GetName());

	FSimpleDelegate ResumeDelegate;
	ResumeDelegate.BindLambda([=]() {
		DMS_LOG_SIMPLE(TEXT("==== %s : ACTIVATE EFFECT WORK COMPLETED ===="),*SourceSequence->GetName());
		OnWorkCompleted.ExecuteIfBound(true);
		DMS_LOG_SIMPLE(TEXT("==== %s : after activate effect completed lambda ends ===="),*SourceSequence->GetName());
	});

	TArray<ADMSSequence*> Sequences;

	for ( auto& NodeWrapper : NodeWrappers )
	{
		auto Node = NodeWrapper->GetEffectNode();
		auto NewSeq = SeqMan->RequestCreateSequence(iEI->GetApplyTargetInterface()->GetObject(), SourceSequence->GetSourcePlayer(), Node, {});
		Sequences.Add(NewSeq);
	}

	SourceSequence->AddEffectsToChildQueue(Sequences,ResumeDelegate);
	DMS_LOG_SIMPLE(TEXT("==== %s : ACTIVATE EFFECT WORK : RUN FIRST EFFECT ===="),*SourceSequence->GetName());
	SourceSequence->RunChildEffectQueue();
}

UDMSEffectSet* UDMSEffect_ActivateEffect_Static::GetEffectSetFromOuter(ADMSActiveEffect* iEI)
{
	// Outer Validation
	auto tOuter = iEI->GetApplyTargetInterface();
	if (tOuter == nullptr) { /*DMS_LOG_SCREEN(TEXT("%s : tOuter is Null"), *iAE->GetName());*/ return nullptr; }

	return tOuter != nullptr ? tOuter->GetOwningEffectSet(EffectSetName) : nullptr;
}

UDMSEffect_ActivateEffect_Variable::UDMSEffect_ActivateEffect_Variable()
{
	//DataPicker.ValueSelector = CreateDefaultSubobject<UDMSValueSelectorDefinition_Effect>("ValueSelector");
}

bool UDMSEffect_ActivateEffect_Variable::GetEffectNodeWrappers(ADMSActiveEffect* iAE, TArray<UDMSEffectNodeWrapper*>& OutWrapperArr)
{ 	
	// Get input data ( Skip if data doesn't exist. )

	auto AEAttComp = iAE->GetComponentByClass<UDMSAttributeComponent>();
	auto rData = AEAttComp->GetTypedAttributeValue<UDMSAttributeValue_Object>(AEAttributeTags);
	if (rData == nullptr) return false;
	auto ObjArr = rData->GetValue();
	if (ObjArr.Num()==0) return false;
	
	for(auto Obj : ObjArr)
	{
		if(Obj->IsA<UDMSEffectNodeWrapper>())
			OutWrapperArr.Add(Cast<UDMSEffectNodeWrapper>(Obj));
	}

	return true;
}


bool UDMSEffect_ActivateEffect_Static::GetEffectNodeWrappers(ADMSActiveEffect* iEI, TArray<UDMSEffectNodeWrapper*>& OutWrapperArr)
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

TArray<UObject*> USelReqGenerator_ObjCand_ActivateEffect::CollectObjects(ADMSSequence* Sequence, ADMSActiveEffect* TargetEI)
{	
	// 프로퍼티로 스태틱중에서 고르기 or EI Outer에서 고르기 의 분기 나누기.

	TArray<UObject*> rv;
	if (UseEffectFromOuter) {
		if (TargetEI != nullptr)
			rv = MakeDataArray(TargetEI);
	
		else {
			for ( auto& EI : Sequence->GetAllActiveEffects())
				rv.Append(MakeDataArray(EI));
		}
	}
	else {
		for (auto EN : StaticEffects)
		{
			rv.Add(EN);
		}
	}
	return rv;
}

TArray<UObject*> USelReqGenerator_ObjCand_ActivateEffect::MakeDataArray(ADMSActiveEffect* TargetEI)
{
	TArray<UObject*> rv;
	auto tOuter = TargetEI->GetApplyTargetInterface();

	if(tOuter == nullptr || tOuter->GetOwningEffectSet(EffectSetTag) == nullptr) { /*DMS_LOG_SCREEN(TEXT("%s : tOuter is Null"), *iAE->GetName());*/ return rv; }

	for(auto EN : tOuter->GetOwningEffectSet(EffectSetTag)->EffectNodes)
	{
		rv.Add(EN);
	}

	return rv;
}
