// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectSet/DMSEffectNode_PlayCard.h"
#include "Effect/DMSEffect_ActivateEffect.h"
#include "EffectSet/DMSEffect_MoveCard.h"
#include "Conditions/DMSConditionObject.h"
#include "Common/DMSGameTags.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_EffectKeyword_Play_Card, "EffectKeyword.Play.Card");

//UDMSEffectNode_PlayCard::UDMSEffectNode_PlayCard():UDMSEffectNode()
//{
//	NodeTag = TAG_DMS_EffectKeyword_Play_Card;
//	bForced=true;
//	bIsChainableEffect=true;
//
//	auto AEffect = CreateDefaultSubobject<UDMSEffect_ActivateEffect_Static>("AEffect");
//	AEffect->EffectSetName = TAG_DMS_EffectType_Cost;
//
//	EffectDefinitions.Add(AEffect);
//
//	auto ChildWrapper = CreateDefaultSubobject<UDMSEffectNodeWrapper_Manual>("ChildWrapper");
//	auto MoveCardNode = CreateDefaultSubobject<UDMSEffectNode>("MoveCardNode");
//	auto MoveCardEffect = CreateDefaultSubobject<UDMSEffect_MoveCard>("MoveCardEffect");
//	MoveCardEffect->TagDestination = FGameplayTag::RequestGameplayTag("Field.Vanilla.PlayArea");
//
//	MoveCardNode->bForced = true;
//	MoveCardNode->bIsChainableEffect = false;
//
//	MoveCardNode->EffectDefinitions.Add(MoveCardEffect);
//	MoveCardNode->Conditions->bEmptyIsTrue=true;
//	ChildWrapper->EffectNode = MoveCardNode;
//
//	ChildEffect = ChildWrapper;
//}
