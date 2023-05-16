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
class ADMSCharacterBase;

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

	/**
	 * Card manager component.
	 * Manage player's card containers like deck, hand, discard pile...
	 */
	UPROPERTY(BlueprintReadOnly)
	UDMSCardManagerComponent* CardManagerComponent;

	/**
	 * Player's default card containers list and intancing class.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FName, TSubclassOf<UDMSCardContainerComponent>> CardContainerTypes;

	/**
	 * Search for named card container from CardManagerComponent.
	 */
	UFUNCTION(BlueprintCallable)
	UDMSCardContainerComponent* SearchContainer(const FName& ContainerName);

	/**
	 * Effect manager component.
	 * Manage effects that targeting players or triggered.
	 */
	UPROPERTY(BlueprintReadOnly)
	UDMSEIManagerComponent* EffectManagerComponent;

	/**
	 * Attribute component.
	 * Manage mana, resource, etc... of player.
	 */
	UPROPERTY(BlueprintReadOnly)
	UDMSAttributeComponent* AttributeComponent;

	/**
	 * Player's default attribute key, value pairs.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FGameplayTag, float> DefaultStats;

	/**
	 * Player's default basic actions.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced)
	TMap<FName, UDMSEffectNode*> DefaultBasicActions;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ADMSCharacterBase* CharacterRef;

public:

	/**
	 * Storing original player's deck. Persistent effect will be saved at Level's cleanup phase.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FDMSCardData> OriginalCardDatas;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FDMSPlayerCharacterData PlayerCharacterData;

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

	/**
	 *
	 */
	UFUNCTION(BlueprintCallable)
	void SetupCardContainers();

	/**
	 *
	 */
	UFUNCTION(BlueprintCallable)
	void SetupAttributes();

	// ============================= //
	//		INTERFACE FUNCTIONS
	// ============================= //
	//virtual void AttachEffectInstance(UDMSEffectInstance* EI) override;
	//virtual bool OnNotifyReceived(TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*>& ResponsedObjects, bool iChainable, UDMSSequence* Seq, UObject* SourceTweak) override;
	//virtual UObject* GetObject() override { return this; }
	virtual AActor* GetOwningPlayer() { return this; }
	// TODO :: Match with BA ?
	//virtual UDMSEffectSet* GetOwningEffectSet(const FGameplayTag& iSetName) override { return nullptr; }

	virtual void SetCurrentLocation_Implementation(ADMSLocationBase* iLoc);
	virtual ADMSLocationBase* GetCurrentLocation_Implementation();
	virtual int GetDistanceWith_Implementation(const TScriptInterface<IDMSLocatableInterface>& OtherObject);
	virtual bool LocatingTo_Implementation(ADMSLocationBase* TargetLocation);
};
