// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/DMSEffectDefinition.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Sequence/DMSSequence.h"
#include "GameModes/DMSGameStateBase.h"
#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEffectOption.h"
#include "Sequence/Steps/DMSSequenceStep_Decision.h"
#include "Sequence/Steps/DMSSequenceStep_Apply.h"
#include "Conditions/DMSConditionObject.h"


void UDMSEffectDefinition::ExecuteEffectOptions(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnOptionCompleted& OnOptionCompleted)
{
	if (EffectOptions.Num()== 0 ) {OnOptionCompleted.Execute(nullptr);return;}
	for (auto& EO : EffectOptions)
	{
		EO->ExecuteOption(SourceSequence, iEI, OnOptionCompleted);
	}
}

void UDMSEffectDefinition::ExecuteEffectDefinition(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnExecuteCompleted& OnExecuteCompleted)
{
	//EffectOption->ExecuteOption(SourceSequence, iEI, [=]() {
	//	Work(SourceSequence, iEI, OnExecuteCompleted);
	//});
	Work(SourceSequence, iEI, OnExecuteCompleted);
}

TArray<TScriptInterface<IDMSEffectorInterface>> UDMSEffectNode::GeneratePresetTarget(UDMSEffectNode* Node, UDMSSequence* iSequence)
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

TArray<FDMSSequenceEIStorage> UDMSEffectNode::GenerateApplyTarget(UDMSEffectNode* Node, UDMSSequence* iSequence)
{
	TArray<FDMSSequenceEIStorage>& Storages = iSequence->GetEIStorage();

	for (auto& Storage : Storages)
		Storage.ApplyTargets.Reset();
	

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


UDMSEffectNode::UDMSEffectNode() : bForced(false),bIgnoreNotify(false), bIsChainableEffect(true)
{
	Conditions = CreateDefaultSubobject<UDMSConditionCombiner>("Conditions");

	StepRequirements.Add(CreateDefaultSubobject<UDMSSequenceStep_Decision>("DecisionStep"));
	StepRequirements.Add(CreateDefaultSubobject<UDMSSequenceStep_Apply>("ApplyStep"));
}

FGameplayTagContainer UDMSEffectNode::GenerateTagContainer()
{
	FGameplayTagContainer ctn;
	ctn.AddTagFast(NodeTag);
	for (auto fx : EffectDefinitions)	ctn.AppendTags(fx->GetEffectTags());

	return ctn;
}

bool UDMSEffectNode::ExecuteTagQuery(const FGameplayTagQuery& EffectTagQuery)
{
	return GenerateTagContainer().MatchesQuery(EffectTagQuery);
}

bool UDMSEffectSet::ExecuteTagQuery(const FGameplayTagQuery& EffectTagQuery)
{
	for (auto node : EffectNodes)
	{
		if (node->GetEffectNode()->ExecuteTagQuery(EffectTagQuery)) return true;
	}

	return false;
}

//TArray<UDMSEffectElementSelectorWidget*> UDMSEffectNode::CreateSelectors(UDMSSequence* OwnerSeq,APlayerController* WidgetOwner)
//{
//	// TODO :: Callback and Queued Selectors
//
//	//DMS_LOG_SCREEN(TEXT("CreateSelectors"));
//	TArray<UDMSEffectElementSelectorWidget*> rv;
//	for (auto ED : EffectDefinitions)
//	{
//		//rv.Append(ED->CreateSelectors());
//		auto t= ED->CreatePairedSelector(OwnerSeq,WidgetOwner);	
//		if(t==nullptr) continue;
//		rv.Add(t);
//	}
//	return rv;
//
//}
//
//TArray<UDMSDecisionWidget*> UDMSEffectNode::CreateDecisionWidgets(UDMSSequence* OwnerSequence,APlayerController* WidgetOwner)
//{
//	TArray<UDMSDecisionWidget*> rv;
//	for (auto& WidgetClass : DecisionWidgetClasses)
//	{
//		auto NewWidget = CreateWidget<UDMSDecisionWidget>(WidgetOwner,WidgetClass);
//		NewWidget->CurrentSequence= OwnerSequence;
//		rv.Add(NewWidget);
//	}
//
//	InitializeDecisionWidget(rv);
//	return rv;
//}

//UDMSEffectElementSelectorWidget* UDMSEffectDefinition::CreatePairedSelector(UDMSSequence* OwnerSeq, APlayerController* WidgetOwner)
//{
//	if(!bIsUsingSelector) return nullptr;
//	
//	auto rv = GetPairedSelector().Get() != nullptr ? CreateWidget<UDMSEffectElementSelectorWidget>(WidgetOwner, GetPairedSelector().Get()) : nullptr;
//	
//	// Never happen?
//	if (rv == nullptr) return nullptr;
//	rv->SetSourceEffectDefinition(this);
//	rv->CurrentSequence = OwnerSeq;
//	InitializePairedSelector(rv);
//
//	return rv;
//}

//TArray<UDMSEffectNodeWrapper*> UDMSEffectSet::GetEffectNodeWithComparer(const FNodeComparer& Comparer)
//{
//	TArray<UDMSEffectNodeWrapper*> rv;
//
//	for (auto NodeWrapper : EffectNodes)
//	{
//		if (Comparer.Execute(NodeWrapper->GetEffectNode())) rv.Add(NodeWrapper);
//	}
//	return rv;
//}


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
