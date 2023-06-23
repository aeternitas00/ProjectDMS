// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/DMSEffectDefinition.h"
#include "Selector/DMSDecisionWidget.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Sequence/DMSSequence.h"
#include "GameModes/DMSGameState.h"
#include "Effect/DMSEffectInstance.h"
#include "Sequence/Steps/DMSSequenceStep_Decision.h"
#include "Sequence/Steps/DMSSequenceStep_Apply.h"
#include "Conditions/DMSConditionObject.h"



AActor* UDMSEffectDefinition::GetPlayerFocusTarget_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI)
{
	// Default source target is Effect's Target.
	return iEI->GetTypedOuter<AActor>();
}

TArray<TScriptInterface<IDMSEffectorInterface>> UDMSEffectNode::GenerateTarget_Implementation(UDMSSequence* iSequence)
{
	TArray<TScriptInterface<IDMSEffectorInterface>> rv;
	return rv;
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

	//switch (Node->PresetTargetFlag)
	//{
	//case EDMSPresetTargetFlag::PTF_Self:
	//	if (iSequence->GetSourceObject()->Implements<UDMSEffectorInterface>()) {
	//		TempTarget.Add(TScriptInterface<IDMSEffectorInterface>(iSequence->GetSourceObject()));
	//		//DMS_LOG_SCREEN(TEXT("EH: CreateEI [%s] To %s"), *iSequence->GetName(), *iSequence->SourceObject->GetName());
	//	}
	//	break;
	//case EDMSPresetTargetFlag::PTF_OC:
	//	if (iSequence->GetSourcePlayer()->Implements<UDMSEffectorInterface>()) {
	//		TempTarget.Add(TScriptInterface<IDMSEffectorInterface>(iSequence->GetSourcePlayer()));
	//		//DMS_LOG_SCREEN(TEXT("EH: CreateEI [%s] To %s"), *iSequence->GetName(), *iSequence->SourcePlayer->GetName());
	//	}
	//	break;
	//case EDMSPresetTargetFlag::PTF_Parent:
	//	if (iSequence->ParentSequence == nullptr)
	//		break;
	//	TempTarget.Append(iSequence->ParentSequence->GetTargets());
	//	//DMS_LOG_SCREEN(TEXT("EH: CreateEI [%s] To %s"), *iSequence->GetName(), *iSequence->SourcePlayer->GetName());
	//	break;
	//case EDMSPresetTargetFlag::PTF_Effect:
	//	TempTarget = Node->GenerateTarget(iSequence);
	//	//DMS_LOG_SCREEN(TEXT("EH: CreateEI [%s] To EN's target (Num : %d)"), *iSequence->GetName(), TempTarget.Num());
	//	break;
	//default:
	//	break;
	//}

}

TArray<TScriptInterface<IDMSEffectorInterface>> UDMSEffectNode::GenerateApplyTarget_Implementation(UDMSSequence* iSequence)
{
	if (ApplyTargetGenerator == nullptr) return iSequence->GetTargets();

	TArray<TScriptInterface<IDMSEffectorInterface>> TempTarget;

	for (auto Target : ApplyTargetGenerator->GetTargets(iSequence->GetSourceObject(), iSequence))
	{
		if (Target->Implements<UDMSEffectorInterface>())
			TempTarget.Add(TScriptInterface<IDMSEffectorInterface>(Target));
	}

	return TempTarget;
}

UDMSEffectNode::UDMSEffectNode() : bForced(false), PresetTargetFlag(EDMSPresetTargetFlag::PTF_Self), bIsChainableEffect(true) 
{
	Conditions = CreateDefaultSubobject<UDMSConditionCombiner>("Conditions");

}

FGameplayTagContainer UDMSEffectNode::GenerateTagContainer()
{
	FGameplayTagContainer ctn;
	ctn.AddTagFast(NodeTag);
	for (auto fx : EffectDefinitions)	ctn.AppendTags(fx->GetEffectTags());

	return ctn;
}

TArray<TSubclassOf<UDMSSequenceStep>> UDMSEffectNode::GetStepRequirements_Implementation()
{
	//TArray<TSubclassOf<UDMSSequenceStep>> rv;
	//for (auto ED : EffectDefinitions)
	//{
	//	rv.Append(ED->GetStepRequirements());
	//}

	return { UDMSSequenceStep_Decision::StaticClass(),UDMSSequenceStep_Apply::StaticClass() };
}

bool UDMSEffectNode::ExecuteTagQuery(const FGameplayTagQuery& EffectTagQuery)
{
	//FGameplayTagContainer ctn;
	//ctn.AddTagFast(NodeTag);
	//for (auto fx : EffectDefinitions)	ctn.AppendTags(fx->GetEffectTags());

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


//TArray<UObject*> UDMSEffectNode::GenerateConditionTarget_Implementation(UDMSSequence* iSequence)
//{
//	return TArray<UObject*>();
//}

TArray<UDMSEffectElementSelectorWidget*> UDMSEffectNode::CreateSelectors(UDMSSequence* OwnerSeq,APlayerController* WidgetOwner)
{
	// TODO :: Callback and Queued Selectors

	//DMS_LOG_SCREEN(TEXT("CreateSelectors"));
	TArray<UDMSEffectElementSelectorWidget*> rv;
	for (auto ED : EffectDefinitions)
	{
		//rv.Append(ED->CreateSelectors());
		auto t= ED->CreatePairedSelector(OwnerSeq,WidgetOwner);	
		if(t==nullptr) continue;
		rv.Add(t);
	}
	return rv;

}

//void UDMSEffectNode::PostInitProperties()
//{
//	Super::PostInitProperties();
//
//	switch (PresetTargetFlag)
//	{
//	case EDMSPresetTargetFlag::PTF_Self:
//		TargetGenerator = NewObject<UDMSTargetGenerator_SourceObject>(this,"TargetGenerator");
//		break;
//	case EDMSPresetTargetFlag::PTF_OC:
//		TargetGenerator = NewObject<UDMSTargetGenerator_SourcePlayer>(this, "TargetGenerator");
//		break;
//	case EDMSPresetTargetFlag::PTF_Parent:
//		TargetGenerator = NewObject<UDMSTargetGenerator_FollowParentSeq>(this, "TargetGenerator");
//		break;
//	case EDMSPresetTargetFlag::PTF_Effect:
//		//TargetGenerator = CreateDefaultSubobject<UDMSTargetGenerator_SourceObject>("TargetGenerator");
//		break;
//	default:
//		//TargetGenerator = CreateDefaultSubobject<UDMSTargetGenerator_SequenceTarget>("TargetGenerator");
//		break;
//	}
//
//	//ApplyTargetGenerator = CreateDefaultSubobject<UDMSTargetGenerator_SequenceTarget>("ApplyTargetGenerator");
//}

TArray<UDMSDecisionWidget*> UDMSEffectNode::CreateDecisionWidgets(UDMSSequence* OwnerSequence,APlayerController* WidgetOwner)
{
	TArray<UDMSDecisionWidget*> rv;
	for (auto WidgetClass : DecisionWidgetClasses)
	{
		auto NewWidget = CreateWidget<UDMSDecisionWidget>(WidgetOwner,WidgetClass);
		NewWidget->OwnerSeq= OwnerSequence;
		rv.Add(NewWidget);
	}

	InitializeDecisionWidget(rv);
	return rv;
}



UDMSEffectElementSelectorWidget* UDMSEffectDefinition::CreatePairedSelector(UDMSSequence* OwnerSeq, APlayerController* WidgetOwner)
{
	if(!bIsUsingSelector) return nullptr;
	
	auto rv = GetPairedSelector().Get() != nullptr ? CreateWidget<UDMSEffectElementSelectorWidget>(WidgetOwner, GetPairedSelector().Get()) : nullptr;
	
	// Never happen?
	if (rv == nullptr) return nullptr;
	rv->SetSourceEffectDefinition(this);
	rv->OwnerSeq = OwnerSeq;
	InitializePairedSelector(rv);

	return rv;
}

TArray<UDMSEffectNodeWrapper*> UDMSEffectSet::GetEffectNodeWithComparer(const FNodeComparer& Comparer)
{
	TArray<UDMSEffectNodeWrapper*> rv;

	for (auto NodeWrapper : EffectNodes)
	{
		if (Comparer.Execute(NodeWrapper->GetEffectNode())) rv.Add(NodeWrapper);
	}
	return rv;
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
