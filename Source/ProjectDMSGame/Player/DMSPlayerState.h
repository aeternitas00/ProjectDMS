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
#include "Player/DMSPlayerStateBase.h"

#include "Card/DMSCardData.h"
#include "Character/DMSPlayerCharacterData.h"

#include "Location/DMSLocatableInterface.h"
#include "Effect/DMSEffectorInterface.h"
#include "Effect/DMSEffectorOwnableInterface.h"
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
class PROJECTDMSGAME_API ADMSPlayerState : public ADMSPlayerStateBase, public IDMSLocatableInterface
{
	GENERATED_BODY()
	
public:
	ADMSPlayerState(const FObjectInitializer& Initializer);

protected:

	/**
	 * Card manager component.
	 * Manage player's card containers like deck, hand, discard pile...
	 */
	UPROPERTY(BlueprintReadOnly,Replicated)
	TObjectPtr<UDMSCardManagerComponent> CardManagerComponent;

	/**
	 * Player's default card containers list and intancing class.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FName, TSubclassOf<UDMSCardContainerComponent>> CardContainerTypes;

	/**
	 * Attribute component.
	 * Manage mana, resource, etc... of player.
	 */
	UPROPERTY(BlueprintReadOnly,Replicated)
	TObjectPtr<UDMSAttributeComponent> AttributeComponent;

	/**
	 * Player's default attribute key, value pairs.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<FDMSAttributeDefinition> DefaultAttributes;

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

	/**
	 * Setup defaults of player state ( component, effect, ... ).
	 */
	UFUNCTION(BlueprintCallable)
	void SetupDefaults();

	/**
	 * Setup player's CardManager.
	 */
	void SetupCardContainers();

	/**
	 * Setup player's AttributeManager.
	 */
	void SetupAttributes();

	/**
	 * Search for named card container from CardManagerComponent.
	 */
	UFUNCTION(BlueprintCallable)
	UDMSCardContainerComponent* SearchContainer(const FName& ContainerName);

	UFUNCTION(BlueprintNativeEvent)
	void OnLoadSaveGame(UDMSSaveGame* LoadedItem);
	virtual void OnLoadSaveGame_Implementation(UDMSSaveGame* LoadedItem);

	UFUNCTION()
	void LoadSaveGame(const FString& SlotName, const int32 UserIndex);

	// ============================= //
	//		INTERFACE FUNCTIONS
	// ============================= //
	//virtual IDMSEffectorInterface* GetPreviewObject() { return PreviewDummy; }
	// TODO :: Match with BA ?
	//virtual UDMSEffectSet* GetOwningEffectSet(const FGameplayTag& iSetName) override { return nullptr; }

	virtual void SetCurrentLocation_Implementation(ADMSLocationBase* iLoc);
	virtual ADMSLocationBase* GetCurrentLocation_Implementation();
	virtual int GetDistanceWith_Implementation(const TScriptInterface<IDMSLocatableInterface>& OtherObject);
	virtual bool LocatingTo_Implementation(ADMSLocationBase* TargetLocation);

};
