// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_GAME_MODULE
 *
 *	DMS Game's Player controller
 *
 *	========================================
 */

#include "ProjectDMS.h"
#include "GameFramework/PlayerController.h"
#include "Effect/DMSEffectorInterface.h"
#include "Common/DMSCommons.h"
#include "DMSPlayerController.generated.h"


struct FDMSAttributeModifier;
class UDMSCardManagerComponent;
class UDMSCardContainerComponent;
class ADMSCardBase;
class UDMSEIManagerComponent;
class UDMSAttributeComponent;
class UDMSAttribute;
class UDMSEffectNode;


/**
 * 	========================================
 *
 *	 
 *	
 *	========================================
 */
UCLASS(Blueprintable)
class PROJECTDMSGAME_API ADMSPlayerController : public APlayerController, public IDMSEffectorInterface//, public IDMSAttributeInterface
{
	GENERATED_BODY()

	// ===================== //
	//		Interaction
	// ===================== //
protected:

	/**
	 *
	 */
	UPROPERTY(BlueprintReadOnly)
	UObject* SelectingObject;

	/**
	 *
	 */
	UPROPERTY(BlueprintReadOnly)
	UObject* InstigatingObject;

public:

	/**
	 * Is this player can interact with other game object ( cards, enemy etc... )
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsInteractable;

	// 클릭해서 정보 조사 하는 용도로 사용 ( 우클릭 에 할당 ? )
	UFUNCTION(BlueprintCallable)
	void InstigateObject(UObject* Object);

	/**
	 * Sort of focusing
	 */
	UFUNCTION(BlueprintCallable)
	void SelectObject(UObject* Object);

	/**
	 * BP Implements, Get new one from "SelectingObject"
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnSelectedObject(UObject* FormerObject);


	// ====================== //
	//		DMS Features
	// ====================== //
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

public:
	ADMSPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	/**
	 * Deprecated
	 */
	UFUNCTION(BlueprintCallable)
	void PopupSelectorWidget(TSubclassOf<UDMSEffectElementSelectorWidget> WidgetClass);

	/**
	 * Play param card.
	 * @param	Card					Playing card.
	 * [ temporal implement ]
	 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void PlayCard(ADMSCardBase* Card);
	
	// ============================= //
	//		INTERFACE FUNCTIONS
	// ============================= //
	virtual void AttachEffectInstance(UDMSEffectInstance* EI) override;
	virtual bool OnNotifyReceived(TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*>& ResponsedObjects, bool iChainable,UDMSSequence* Seq,UObject* SourceTweak) override;
	virtual UObject* GetObject() override { return this; }
	virtual AActor* GetOwningPlayer() { return this; }
	virtual UDMSEffectSet* GetOwningEffectSet(const FName& iSetName) override { return nullptr; }
};
