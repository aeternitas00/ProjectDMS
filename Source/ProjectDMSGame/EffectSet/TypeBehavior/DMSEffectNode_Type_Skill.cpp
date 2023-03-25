// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectSet/TypeBehavior/DMSEffectNode_Type_Skill.h"

#include "EffectSet/DMSEffect_MoveCard.h"



UDMSEffectNode_Type_Skill::UDMSEffectNode_Type_Skill() 
{ 
	TypeName = TEXT("Skill"); 
	bForced = true;

	Conditions->TimingConditions = CreateDefaultSubobject<UDMSTimingCombiner>("TimingConditions");
	
	UDMSTimingCondition* AfterEnteredPlayArea = CreateDefaultSubobject<UDMSTimingCondition>("AfterEnteredPlayArea");

	AfterEnteredPlayArea->Timing=EDMSTimingFlag::T_After;
	AfterEnteredPlayArea->EffectTagQuery = FGameplayTagQuery::MakeQuery_MatchTag(TAG_DMS_EffectKeyword_PlayCard);
	
	CheckerDefinition = CreateDefaultSubobject<UDMSNotifyCheckerDefinition_ObjectCompareBase>(TEXT("Skill_CheckerDefinition"));

	FDMSObjectSelectionRules NewRule;
	NewRule.Comparers.Add(CreateDefaultSubobject<UDMSObjectSelectionComparer_IsSelf>("IsSelfComparer"));
	
	CheckerDefinition->CompareTargetFlag= EDMSObjectSelectorFlag::OSF_Target;
	CheckerDefinition->SelectionRules = NewRule;

	AfterEnteredPlayArea->Checkers.Add(CheckerDefinition);

	//Conditions->AddTimingCondition(AfterEnteredPlayArea);
	Conditions->TimingConditions->Conditions.Add(AfterEnteredPlayArea);
	Conditions->StateConditions->bEmptyStateIsTrue=true;
	//Effect Initialize
	// 
	//Effects.Add(CreateDefaultSubobject<UDMSEffect_ActivateCard>("Skill_ActivateCard"));

	auto scnd = CreateDefaultSubobject<UDMSEffect_MoveCard>("Skill_MovetoDiscardPile");

	scnd->NameDestination=TEXT("DiscardPile");

	EffectDefinitions.Add(scnd);
}