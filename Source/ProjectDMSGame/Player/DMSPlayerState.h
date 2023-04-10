// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_GAME_MODULE
 *
 *	DMS Game's Player state
 *
 *	========================================
 */

#include "ProjectDMS.h"
#include "GameFramework/PlayerState.h"
#include "Card/DMSCardData.h"
#include "DMSPlayerState.generated.h"

/**
 *	========================================
 *
 *
 *
 *	========================================
 */
UCLASS(Blueprintable)
class PROJECTDMSGAME_API ADMSPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	
	TArray<FDMSCardData> OriginalCardDatas;
	// Original card infos.

	// �ΰ��� �߿��� ���� �з��� ����� ������ �ٸ� �÷��̾ �� �� ���°� �⺻. 
	// �ش� ������ ������ �� �ִ� ȿ���� ���� �� ������ �ؼ� ���� �̰��� �����ϴ� ���·�.

	void LoadDeck(class UDMSSaveGame* SaveGame){}


};
