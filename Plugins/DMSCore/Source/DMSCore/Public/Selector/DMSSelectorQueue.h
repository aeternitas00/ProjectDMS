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
	TArray<TObjectPtr<UDMSSelectorHandle>> SelectorHandles;

	/**
	 * Reference of owner sequence.
	 */
	UPROPERTY()
	TObjectPtr<UDMSSequence> CurrentSequence;

	/**
	 * Current index of WidgetQueue.
	 */
	UPROPERTY()
	int8 CurrentIndex;

	/**
	 * Setup selector queue.
	 * @param	SourceSequence			Owner sequence.
	 * @param	Handles					Selector handles for instantiating widgets during this initialization process.
	 */
	bool SetupQueue(UDMSSequence* SourceSequence, TArray<UDMSSelectorHandle*> Handles);	
	
	/**
	 * Setup selector queue.
	 * @param	iOnQueueFinished			Lambda function to execute when the queue succeeds.
	 * @param	iOnQueueCanceled			Lambda function to execute when the queue canceled.
	*/
	template<typename FuncFinished, typename FuncCanceled >
	void RunWidgetQueue(FuncFinished&& iOnQueueFinished, FuncCanceled&& iOnQueueCanceled);

	/**
	 * Popup the next widget when queue is running.
	 */
	UFUNCTION()
	void PopupNextWidget();

	/**
	 * Reset queue's progress and restart from begin.
	 */
	UFUNCTION(BlueprintCallable)
	void RedoWidgetQueue();

	/**
	 * Clear queue and destory instanced widgets.
	 */
	UFUNCTION(BlueprintCallable)
	void ClearQueue();

	/**
	 * Getter of casted outer.
	 */
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
};

template<typename FuncCompleted, typename FuncCanceled >
void UDMSWidgetQueue::RunWidgetQueue(FuncCompleted&& iOnQueueFinished, FuncCanceled&& iOnQueueCanceled)
{
	//OnSelectorsCompleted.BindLambda(std::forward<FuncCompleted&&>(iOnSelectorsCompleted));
	OnSelectorsCompleted.AddLambda(iOnQueueFinished);
	OnSelectorsCanceled.AddLambda(iOnQueueCanceled);
	PopupNextWidget();
}
