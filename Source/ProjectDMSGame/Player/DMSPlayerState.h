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
#include "Character/DMSPlayerCharacterData.h"

#include "Location/DMSLocatableInterface.h"
#include "Effect/DMSEffectorInterface.h"

#include "DMSPlayerState.generated.h"

class UDMSCardManagerComponent;
class UDMSCardContainerComponent;
class UDMSAttributeComponent;
class UDMSEIManagerComponent;
class ADMSCharacterBase;
class UDMSEffectNode;

/**
 *	========================================
 *
 *
 *
 *	========================================
 */
UCLASS(Blueprintable)
class PROJECTDMSGAME_API ADMSPlayerState : public APlayerState, public IDMSEffectorInterface, public IDMSLocatableInterface
{
	GENERATED_BODY()
	
public:
	ADMSPlayerState(const FObjectInitializer& Initializer);

protected:
	UPROPERTY()
	TObjectPtr<ADMSPlayerState> PreviewDummy;
	/**
	 * Card manager component.
	 * Manage player's card containers like deck, hand, discard pile...
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDMSCardManagerComponent> CardManagerComponent;

	/**
	 * Player's default card containers list and intancing class.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FName, TSubclassOf<UDMSCardContainerComponent>> CardContainerTypes;

	/**
	 * Effect manager component.
	 * Manage effects that targeting players or triggered.
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDMSEIManagerComponent> EffectManagerComponent;

	/**
	 * Attribute component.
	 * Manage mana, resource, etc... of player.
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDMSAttributeComponent> AttributeComponent;

	/**
	 * Player's default attribute key, value pairs.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FGameplayTag, float> DefaultStats;

	/**
	 * Player's default basic actions.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced)
	TMap<FName, TObjectPtr<UDMSEffectNode>> DefaultBasicActions;


public:

	/**
	 * Storing original player's deck. Persistent effect will be saved at Level's cleanup phase.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FDMSCardData> OriginalCardDatas;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FDMSPlayerCharacterData PlayerCharacterData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<ADMSCharacterBase> CharacterRef;

	/**
	 *
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 EXP;

	/**
	 *
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Level;

	// 인게임 중에서 위의 분류중 비공개 정보는 다른 플레이어가 알 수 없는게 기본. 
	// 해당 정보를 공개할 수 있는 효과가 있을 때 가공을 해서 따로 이곳에 저장하는 형태로.

	/**
	 * Load player deck from save game.
	 * @param	SaveGame					Savegame of current slot.
	 * [ Temporal implements ]
	 */
	void LoadDatasFromSave(class UDMSSaveGame* SaveGame);

	/**
	 * Setter of OriginalCardDatas. Called at cleanup phase.
	 * @param	InDatas						New datas.
	 * [ Temporal implements ]
	 */
	UFUNCTION(BlueprintCallable)
	void SetCardDatas(const TArray<FDMSCardData>& InDatas);

	UFUNCTION(BlueprintCallable)
	void SetupDefaults();

	/**
	 *
	 */
	void SetupCardContainers();

	/**
	 *
	 */
	void SetupAttributes();

	/**
	 * Search for named card container from CardManagerComponent.
	 */
	UFUNCTION(BlueprintCallable)
	UDMSCardContainerComponent* SearchContainer(const FName& ContainerName);

	// ============================= //
	//		INTERFACE FUNCTIONS
	// ============================= //
	virtual AActor* GetOwningPlayer() { return this; }
	virtual IDMSEffectorInterface* GetPreviewObject() { return PreviewDummy; }
	// TODO :: Match with BA ?
	//virtual UDMSEffectSet* GetOwningEffectSet(const FGameplayTag& iSetName) override { return nullptr; }

	virtual void SetCurrentLocation_Implementation(ADMSLocationBase* iLoc);
	virtual ADMSLocationBase* GetCurrentLocation_Implementation();
	virtual int GetDistanceWith_Implementation(const TScriptInterface<IDMSLocatableInterface>& OtherObject);
	virtual bool LocatingTo_Implementation(ADMSLocationBase* TargetLocation);
};
