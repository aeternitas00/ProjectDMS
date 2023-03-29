// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectSet/TypeBehavior/DMSEffectNode_Type_Skill.h"
#include "Conditions/DMSConditionObject_.h"
#include "Conditions/DMSSequenceCondition.h"
#include "Conditions/ObjectConditions/DMSObjectCompareCondition.h"
#include "EffectSet/DMSEffectNode_PlayCard.h"
#include "EffectSet/DMSEffect_MoveCard.h"

//UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_EffectKeyword_Play_Card, "EffectKeyword.Play.Card");

UDMSEffectNode_Type_Skill::UDMSEffectNode_Type_Skill() 
{ 
	TypeName = TEXT("Skill"); 
	bForced = true;

	UDMSSeqTimingCondition* TC = CreateDefaultSubobject<UDMSSeqTimingCondition>("TC");
	TC->Timing=EDMSTimingFlag::T_After;
	TC->EffectTagQuery = FGameplayTagQuery::MakeQuery_MatchTag(TAG_DMS_EffectKeyword_PlayCard);

	__Conditions->Conditions.Add(TC);

	UDMSObjectCompareCondition* OC = CreateDefaultSubobject<UDMSObjectCompareCondition>("OC");
	UDMSObjectSelectionComparer_IsSelf_* IsSelf = CreateDefaultSubobject<UDMSObjectSelectionComparer_IsSelf_>("IsSelf");
	OC->bAllObjectMustPassed=true;
	OC->TargetFlag = EDMSObjectSelectorFlag::OSF_Target;
	OC->Comparer= IsSelf;

	__Conditions->Conditions.Add(OC);


	//Conditions->TimingConditions = CreateDefaultSubobject<UDMSTimingCombiner>("TimingConditions");
	//
	//UDMSTimingCondition* AfterEnteredPlayArea = CreateDefaultSubobject<UDMSTimingCondition>("AfterEnteredPlayArea");

	//AfterEnteredPlayArea->Timing=EDMSTimingFlag::T_After;
	//AfterEnteredPlayArea->EffectTagQuery = FGameplayTagQuery::MakeQuery_MatchTag(TAG_DMS_EffectKeyword_PlayCard);
	//
	//CheckerDefinition = CreateDefaultSubobject<UDMSNotifyCheckerDefinition_ObjectCompareBase>(TEXT("Skill_CheckerDefinition"));

	//FDMSObjectSelectionRules NewRule;
	//NewRule.Comparers.Add(CreateDefaultSubobject<UDMSObjectSelectionComparer_IsSelf>("IsSelfComparer"));
	//
	//CheckerDefinition->CompareTargetFlag= EDMSObjectSelectorFlag::OSF_Target;
	//CheckerDefinition->SelectionRules = NewRule;

	//AfterEnteredPlayArea->Checkers.Add(CheckerDefinition);

	////Conditions->AddTimingCondition(AfterEnteredPlayArea);
	//Conditions->TimingConditions->Conditions.Add(AfterEnteredPlayArea);
	//Conditions->StateConditions->bEmptyStateIsTrue=true;
	//Effect Initialize
	// 
	//Effects.Add(CreateDefaultSubobject<UDMSEffect_ActivateCard>("Skill_ActivateCard"));

	auto scnd = CreateDefaultSubobject<UDMSEffect_MoveCard>("Skill_MovetoDiscardPile");

	scnd->NameDestination=TEXT("DiscardPile");

	EffectDefinitions.Add(scnd);
}