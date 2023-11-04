// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *	Selector queue
 *
 *	========================================
 */

//#include "DMSCoreIncludes.h"
#include "Common/DMSCommons.h"
#include "Common/DMSValueSelectorDefinition.h"
#include "DMSSelectorQueue.generated.h"

class UDMSConfirmWidgetBase;
class UDMSSequence;
class UDMSSelectorHandle;
class ADMSPlayerControllerBase;
class UDMSSelectorHandle;

UCLASS(BlueprintType)
class DMSCORE_API UDMSWidgetQueue : public UObject
{
	GENERATED_BODY()

public:
	UDMSWidgetQueue();
	/**
	 * List of selectors.
	 */
	UPROPERTY()
	TArray<UDMSSelectorHandle*> SelectorHandles;

	UPROPERTY()
	UDMSSequence* CurrentSequence;

	/**
	 * Current index of WidgetQueue.
	 */
	UPROPERTY()
	int8 CurrentIndex;

	bool SetupQueue(UDMSSequence* SourceSequence,TArray<UDMSSelectorHandle*> Handles);	
	
	template<typename FuncFinished, typename FuncCanceled >
	void RunWidgetQueue(FuncFinished&& iOnQueueFinished, FuncCanceled&& iOnQueueCanceled);

	UFUNCTION()
	void PopupNextWidget();

	UFUNCTION(BlueprintCallable)
	void RedoWidgetQueue();

	UFUNCTION(BlueprintCallable)
	void ClearQueue();

	APlayerController* GetWidgetOwner() { return Cast<APlayerController>(GetOuter()); }

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnWidgetQueuesClosed, UDMSSequence*);

	/**
	 * Delegate to be called when the Selector Queue was completed.
	 */
	FOnWidgetQueuesClosed OnSelectorsCompleted;
	FOnWidgetQueuesClosed OnSelectorsCompleted_Handle;
	/**
	 * Delegate to be called when the Selector Queue was canceled.
	 */
	FOnWidgetQueuesClosed OnSelectorsCanceled;

	//=== NEW SELECTOR ===//


	//UFUNCTION()
	//bool InitializeQueue(TArray<FDMSValueSelectionForm> RequestForms, APlayerController* WidgetOwner, UDMSSequence* iSequence);


};

template<typename FuncCompleted, typename FuncCanceled >
void UDMSWidgetQueue::RunWidgetQueue(FuncCompleted&& iOnQueueFinished, FuncCanceled&& iOnQueueCanceled)
{
	//OnSelectorsCompleted.BindLambda(std::forward<FuncCompleted&&>(iOnSelectorsCompleted));
	OnSelectorsCompleted.AddLambda(iOnQueueFinished);
	OnSelectorsCanceled.AddLambda(iOnQueueCanceled);
	PopupNextWidget();
}
