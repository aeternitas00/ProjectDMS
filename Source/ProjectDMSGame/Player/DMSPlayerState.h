// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "GameFramework/PlayerState.h"
#include "Card/DMSCardContainerComponent.h"
#include "DMSPlayerState.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTDMSGAME_API ADMSPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	// Player Area
	// {
	// private:
	// Deck,Hand
	// 
	// public:
	// DiscardPile,Original deck,Resource
	// }

	// 인게임 중에서 위의 분류중 비공개 정보는 다른 플레이어가 알 수 없는게 기본. 
	// 해당 정보를 공개할 수 있는 효과가 있을 때 가공을 해서 따로 이곳에 저장하는 형태로.
};
