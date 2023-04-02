// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/DMSEffectDefinition.h"
#include "Selector/DMSDecisionWidget.h"
#include "Sequence/DMSSequence.h"
#include "Conditions/DMSConditionObject.h"

//TArray<UDMSEffectNode*> UDMSEffectSet::SearchEffects(UDMSSequence* iSeq)
//{ 
//	TArray<UDMSEffectNode*> rv;
//
//	
//	for (auto fx : EffectNodes)
//	{
//		if (fx->Conditions.CheckOperation_Implementation(iSeq)) rv.Add(fx);
//	}
//	
//	return rv;
//}

TArray<TScriptInterface<IDMSEffectorInterface>> UDMSEffectNode::GenerateTarget_Implementation(UDMSSequence* iSequence)
{
	TArray<TScriptInterface<IDMSEffectorInterface>> rv;

	//for (auto ED : EffectDefinitions)
	//{
	//	for (auto Target : ED->GenerateTarget())	rv.AddUnique(Target);
	//}
	return rv;
}

TArray<TScriptInterface<IDMSEffectorInterface>> UDMSEffectNode::GeneratePresetTarget(UDMSEffectNode* Node, UDMSSequence* iSequence)
{	
	TArray<TScriptInterface<IDMSEffectorInterface>> TempTarget;
	switch (Node->PresetTargetFlag)
	{
	case EDMSPresetTargetFlag::PTF_Self:
		if (iSequence->SourceObject->Implements<UDMSEffectorInterface>()) {
			TempTarget.Add(TScriptInterface<IDMSEffectorInterface>(iSequence->SourceObject));
			DMS_LOG_SCREEN(TEXT("EH: CreateEI [%s] To %s"), *iSequence->GetName(), *iSequence->SourceObject->GetName());
		}
		break;
	case EDMSPresetTargetFlag::PTF_OC:
		if (iSequence->SourceController->Implements<UDMSEffectorInterface>()) {
			TempTarget.Add(TScriptInterface<IDMSEffectorInterface>(iSequence->SourceController));
			DMS_LOG_SCREEN(TEXT("EH: CreateEI [%s] To %s"), *iSequence->GetName(), *iSequence->SourceController->GetName());
		}
		break;
	case EDMSPresetTargetFlag::PTF_Parent:
		if (iSequence->ParentSequence == nullptr)
			break;
		TempTarget.Append(iSequence->ParentSequence->Targets);
		DMS_LOG_SCREEN(TEXT("EH: CreateEI [%s] To %s"), *iSequence->GetName(), *iSequence->SourceController->GetName());
		break;
	case EDMSPresetTargetFlag::PTF_Effect:
		TempTarget = Node->GenerateTarget(iSequence);
		DMS_LOG_SCREEN(TEXT("EH: CreateEI [%s] To EN's target (Num : %d)"), *iSequence->GetName(), TempTarget.Num());
		break;
	default:
		break;
	}
	return TempTarget;
}

bool UDMSEffectNode::ExecuteTagQuery(const FGameplayTagQuery& EffectTagQuery)
{
	FGameplayTagContainer ctn;
	ctn.AddTagFast(NodeTag);
	for (auto fx : EffectDefinitions)	ctn.AddTag(fx->EffectTag);
	
	return ctn.MatchesQuery(EffectTagQuery);
}

bool UDMSEffectSet::ExecuteTagQuery(const FGameplayTagQuery& EffectTagQuery)
{
	for (auto node : EffectNodes)
	{
		if (node->GetEffectNode()->ExecuteTagQuery(EffectTagQuery)) return true;
	}

	return false;
}

TArray<UDMSEffectElementSelectorWidget*> UDMSEffectNode::CreateSelectors()
{
	// TODO :: Callback and Queued Selectors

	//DMS_LOG_SCREEN(TEXT("CreateSelectors"));
	TArray<UDMSEffectElementSelectorWidget*> rv;
	for (auto ED : EffectDefinitions)
	{
		//rv.Append(ED->CreateSelectors());
		auto t= ED->CreatePairedSelector();	if(t==nullptr) continue;
		rv.Add(t);
	}
	return rv;

}

TArray<UDMSDecisionWidget*> UDMSEffectNode::CreateDecisionWidgets()
{
	TArray<UDMSDecisionWidget*> rv;
	for (auto WidgetClass : DecisionWidgetClasses)
	rv.Add(NewObject<UDMSDecisionWidget>(this,WidgetClass));

	InitializeDecisionWidget(rv);
	return rv;
}

UDMSEffectElementSelectorWidget* UDMSEffectDefinition::CreatePairedSelector()
{
	if(!bIsUsingSelector) return nullptr;
	
	auto rv = GetPairedSelector().Get() != nullptr ? NewObject<UDMSEffectElementSelectorWidget>(this, GetPairedSelector().Get()) : nullptr;
	
	// Never happen?
	if (rv == nullptr) return nullptr;
	rv->SetSourceEffectDefinition(this);
	InitializePairedSelector(rv);

	return rv;
}
