// Copyright JeongWoo Lee


#include "Effect/DMSEffectDefinition.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Sequence/DMSSequence.h"
#include "GameModes/DMSGameStateBase.h"
#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEffectOption.h"
#include "Attribute/DMSAttribute.h"
#include "Sequence/Steps/DMSSequenceStep_Decision.h"
#include "Sequence/Steps/DMSSequenceStep_Apply.h"
#include "Conditions/DMSConditionObject.h"


void UDMSEffectDefinition::ExecuteEffectOptions(ADMSSequence* SourceSequence, ADMSActiveEffect* iEI, const FOnOptionCompleted& OnOptionCompleted)
{
	if (EffectOptions.Num()== 0 ) {OnOptionCompleted.Execute(nullptr);return;}
	for (auto& EO : EffectOptions)
	{
		EO->ExecuteOption(SourceSequence, iEI, OnOptionCompleted);
	}
}

void UDMSEffectDefinition::ExecuteEffectDefinition(ADMSSequence* SourceSequence, ADMSActiveEffect* iEI, const FOnExecuteCompleted& OnExecuteCompleted)
{
	//EffectOption->ExecuteOption(SourceSequence, iEI, [=]() {
	//	Work(SourceSequence, iEI, OnExecuteCompleted);
	//});

	Work(SourceSequence, iEI, OnExecuteCompleted);
}

TArray<TScriptInterface<IDMSEffectorInterface>> UDMSEffectNode::GeneratePresetTarget(UDMSEffectNode* Node, ADMSSequence* iSequence)
{	
	TArray<TScriptInterface<IDMSEffectorInterface>> TempTarget;

	if (Node->TargetGenerator == nullptr) return TempTarget;

	for (auto Target : Node->TargetGenerator->GetTargets(iSequence->GetSourceObject(), iSequence))
	{
		if (Target->Implements<UDMSEffectorInterface>())
			TempTarget.Add(TScriptInterface<IDMSEffectorInterface>(Target));
	}

	return TempTarget;
}

TArray<FDMSSequenceEIStorage> UDMSEffectNode::GenerateApplyTarget(UDMSEffectNode* Node, ADMSSequence* iSequence)
{
	TArray<FDMSSequenceEIStorage>& Storages = iSequence->GetEIStorage();

	for (auto& Storage : Storages)
		Storage.ApplyTargets.Reset();
	
	// 별도의 어플라이 타게터가 없으면 메인 타겟을 따라가도록.
	if (Node->ApplyTargetGenerator == nullptr) {
		for ( auto& Storage : Storages )
			Storage.ApplyTargets.Add(Storage.MainTarget);
		return Storages;
	}

	for (auto& Storage : Storages)
	{
		for (auto& Target : Node->ApplyTargetGenerator->GetTargets(Storage.MainTarget->GetObject(), iSequence))
		{
			if (Target->Implements<UDMSEffectorInterface>())
				Storage.ApplyTargets.Add(TScriptInterface<IDMSEffectorInterface>(Target));
		}
	}

	return Storages;
}


UDMSEffectNode::UDMSEffectNode() : bForced(false), bCanResponseMulTime(false), bIgnoreNotify(false), bIsChainableEffect(true)
{
	Conditions = CreateDefaultSubobject<UDMSConditionCombiner>("Conditions");

	//StepDefinitions = StepClassRequirements.Array();
}

FGameplayTagContainer UDMSEffectNode::GenerateTagContainer_Implementation(ADMSSequence* CurrentSequence)
{
	FGameplayTagContainer ctn;
	ctn.AppendTags(NodeTags);

	//for (auto& fx : EffectDefinitions)	ctn.AppendTags(fx->GetEffectTags());

	return ctn;
}

bool UDMSEffectNode::ExecuteTagQuery(const FGameplayTagQuery& EffectTagQuery,ADMSSequence* CurrentSequence)
{
	return GenerateTagContainer(CurrentSequence).MatchesQuery(EffectTagQuery);
}

bool UDMSEffectSet::ExecuteTagQuery(const FGameplayTagQuery& EffectTagQuery,ADMSSequence* CurrentSequence)
{
	for (auto& Node : EffectNodes)
	{
		if (Node->GetEffectNode()->ExecuteTagQuery(EffectTagQuery,CurrentSequence)) return true;
	}

	return false;
}

// SERIALIZATIONS

//void UDMSEffectDefinition::Serialize(FArchive& Ar)
//{
//	Super::Serialize(Ar);
//
//	if (Ar.IsSaving()) {}
//	else if (Ar.IsLoading()) {}
//}
//void UDMSEffectNode::Serialize(FArchive& Ar)
//{
//	Super::Serialize(Ar);
//
//	if (Ar.IsSaving()){}
//	else if (Ar.IsLoading()) {}
//}
//
//void UDMSEffectSet::Serialize(FArchive& Ar)
//{
//	Super::Serialize(Ar);
//
//	if (Ar.IsSaving()) {}
//	else if (Ar.IsLoading()) {}
//}
