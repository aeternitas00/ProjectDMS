// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/DMSEffectDefinition.h"
#include "Selector/DMSDecisionWidget.h"
#include "Sequence/DMSSequence.h"


//TArray<UDMSEffectNode*> UDMSEffectSet::SearchEffects(UDMSSequence* iSeq)
//{ 
//	TArray<UDMSEffectNode*> rv;
//
//	
//	for (auto fx : EffectNodes)
//	{
//		if (fx->Conditions.CheckCondition(iSeq)) rv.Add(fx);
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

bool UDMSEffectNode::IsContainKeyword(const FName& iKeyword)
{
	if (NodeKeyword == iKeyword) return true;

	for (auto fx : EffectDefinitions)
	{
		if (fx->Keyword == iKeyword) return true;
		// some detail string comparer 
	}

	return false;
}


bool UDMSEffectSet::IsContainKeyword(const FName & iKeyword) 
{ 
	for (auto node : EffectNodes)
	{
		if (node->IsContainKeyword(iKeyword)) return true;
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

//TArray<UDMSEffectElementSelectorWidget*> UDMSEffectDefinition::CreateSelectors()
//{
//	TArray<UDMSEffectElementSelectorWidget*> rv;
//	for (auto Selector : Selectors)
//	{
//		auto NewWidget = DuplicateObject<UDMSEffectElementSelectorWidget>(Selector, this);
//		NewWidget->SetSourceEffectDefinition(this);
//		rv.Add(NewWidget);
//	}
//
//	return rv;
//}

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
