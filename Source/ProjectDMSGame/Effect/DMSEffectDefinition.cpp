// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/DMSEffectDefinition.h"
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

void UDMSEffectNode::CreateSelectors(APlayerController* WidgetOwner, UDMSSequence* inSequence)
{
	// TODO :: Callback and Queued Selectors

	//DMS_LOG_SCREEN(TEXT("CreateSelectors"));

	for (auto ED : EffectDefinitions)
	{
		ED->CreateSelectors_(WidgetOwner, inSequence);
	}

	inSequence->InitializeSelectorQueue();
}

void UDMSEffectDefinition::CreateSelectors_(APlayerController* WidgetOwner, UDMSSequence* inSequence)
{
	// TODO :: Callback and Queued Selectors

	//DMS_LOG_SCREEN(TEXT("CreateSelectors"));
	for (auto Selector : Selectors)
	{
		auto NewWidget = DuplicateObject<UDMSEffectElementSelectorWidget>(Selector, WidgetOwner);
		NewWidget->SetOwningPlayer(WidgetOwner);
		NewWidget->SetSourceEffectDefinition(this);
		inSequence->AddToSelectorQueue(NewWidget);
	}
}
