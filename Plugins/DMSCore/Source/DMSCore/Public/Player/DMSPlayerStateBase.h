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

#include "DMSCoreIncludes.h"
#include "GameFramework/PlayerState.h"

#include "Effect/DMSEffectorInterface.h"
#include "Effect/DMSEffectorOwnableInterface.h"
#include "DMSPlayerStateBase.generated.h"

class UDMSEIManagerComponent;

/**
 *	========================================
 *
 *
 *
 *	========================================
 */
UCLASS(Blueprintable)
class DMSCORE_API ADMSPlayerStateBase : public APlayerState, public IDMSEffectorInterface, public IDMSEffectorOwnableInterface
{
	GENERATED_BODY()
	
public:
	ADMSPlayerStateBase(const FObjectInitializer& Initializer);

protected:
	/**
	 * Effect manager component.
	 * Manage effects that targeting players or triggered.
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDMSEIManagerComponent> EIManagerComponent;

public:
	// ============================= //
	//		INTERFACE FUNCTIONS
	// ============================= //
	virtual AActor* GetOwningPlayer_Implementation() { return this; }
	virtual int32 GetID() override { /* temp */return GetPlayerId(); }
	virtual ADMSPlayerControllerBase* GetWidgetOwner() override { return Cast<ADMSPlayerControllerBase>(GetPlayerController()); }
};
