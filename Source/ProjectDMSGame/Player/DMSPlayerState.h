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

	/**
	 * Storing original player's deck. Persistent effect will be saved at Level's cleanup phase.
	 */
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<FDMSCardData> OriginalCardDatas;
	// Original card infos.

	// �ΰ��� �߿��� ���� �з��� ����� ������ �ٸ� �÷��̾ �� �� ���°� �⺻. 
	// �ش� ������ ������ �� �ִ� ȿ���� ���� �� ������ �ؼ� ���� �̰��� �����ϴ� ���·�.

	/**
	 * Load player deck from save game.
	 * @param	SaveGame					Savegame of current slot.
	 * [ Temporal implements ]
	 */
	void LoadDeck(class UDMSSaveGame* SaveGame){}

	/**
	 * Setter of OriginalCardDatas. Called at cleanup phase.
	 * @param	InDatas						New datas.
	 * [ Temporal implements ]
	 */
	UFUNCTION(BlueprintCallable)
	void SetCardDatas(const TArray<FDMSCardData>& InDatas);
};
