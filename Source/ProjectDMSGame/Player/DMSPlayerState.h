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

	// �ΰ��� �߿��� ���� �з��� ����� ������ �ٸ� �÷��̾ �� �� ���°� �⺻. 
	// �ش� ������ ������ �� �ִ� ȿ���� ���� �� ������ �ؼ� ���� �̰��� �����ϴ� ���·�.
};
