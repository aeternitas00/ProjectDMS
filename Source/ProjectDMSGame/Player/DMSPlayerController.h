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
#include "Player/DMSPlayerControllerBase.h"
#include "Location/DMSLocatableInterface.h"
#include "Common/DMSCommons.h"
#include "DMSPlayerController.generated.h"


struct FDMSAttributeModifier;
class ADMSCameraPawn;
class UDMSCardManagerComponent;
class UDMSCardContainerComponent;
class ADMSCardBase;
class UDMSEIManagerComponent;
class UDMSAttributeComponent;
class UDMSEffectNode;
class UDMSSaveGame;
class ADMSCharacterBase;

/**
 * 	========================================
 *
 *	 
 *	
 *	========================================
 */
UCLASS(Blueprintable)
class PROJECTDMSGAME_API ADMSPlayerController : public ADMSPlayerControllerBase
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


public:
	ADMSPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	/**
	 * Play param card.
	 * @param	Card					Playing card.
	 * [ temporal implement ]
	 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void PlayCard(ADMSCardBase* Card);
	
	UFUNCTION(BlueprintCallable)
	ADMSCameraPawn* GetCameraPawn();

	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void CreateHUDWidgets();
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void SetupHUDWidgets();	

	UFUNCTION(BlueprintCallable, Server,Reliable)
	void Server_OnSetupComplete();


};
