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

#include "DMSCoreIncludes.h"
#include "GameFramework/PlayerController.h"
#include "Common/DMSCommons.h"
#include "Common/DMSValueSelectorDefinition.h"
#include "DMSPlayerControllerBase.generated.h"

class UDMSConfirmWidgetBase;
class UDMSWidgetQueue;
class UDMSSelectorHandle;

/**
 * 	========================================
 *
 *	 
 *	
 *	========================================
 */
UCLASS(Blueprintable)
class DMSCORE_API ADMSPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()


protected:

public:
	ADMSPlayerControllerBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * Container for widgets used by the player to make decisions during the progress of a sequence.
	 */
	UPROPERTY()
	TObjectPtr<UDMSWidgetQueue> WidgetQueue;

	UFUNCTION(/*Client*/)
	bool SetupWidgetQueue(ADMSSequence* SourceSequence, TArray<UDMSSelectorHandle*> Handles);

	template<typename FuncFinished, typename FuncCanceled >
	void RunWidgetQueue(FuncFinished&& iOnQueueFinished, FuncCanceled&& iOnQueueCanceled);

};


template<typename FuncFinished, typename FuncCanceled >
void ADMSPlayerControllerBase::RunWidgetQueue(FuncFinished&& iOnQueueFinished, FuncCanceled&& iOnQueueCanceled)
{
	WidgetQueue->RunWidgetQueue(
		std::forward<FuncFinished&&>(iOnQueueFinished),
		std::forward<FuncCanceled&&>(iOnQueueCanceled)
	);
}