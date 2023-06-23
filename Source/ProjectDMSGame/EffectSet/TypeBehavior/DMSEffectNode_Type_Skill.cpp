// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectSet/TypeBehavior/DMSEffectNode_Type_Skill.h"
#include "Conditions/DMSConditionObject.h"
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

	Conditions->Conditions.Add(TC);

	UDMSObjectCompareCondition* OC = CreateDefaultSubobject<UDMSObjectCompareCondition>("OC");
	UDMSObjectComparer_IsTarget* IsSelf = CreateDefaultSubobject<UDMSObjectComparer_IsTarget>("IsSelf");
	OC->bAllObjectMustPassed=true;
	OC->TargetFlag = EDMSObjectSelectorFlag::OSF_Target;
	OC->CompareTargetGenerator = CreateDefaultSubobject<UDMSTargetGenerator_SequenceTarget>("CompareTargetGenerator");
	OC->Comparer= IsSelf;

	Conditions->Conditions.Add(OC);

	auto scnd = CreateDefaultSubobject<UDMSEffect_MoveCard>("Skill_MovetoDiscardPile");

	scnd->NameDestination=TEXT("DiscardPile");

	EffectDefinitions.Add(scnd);
}