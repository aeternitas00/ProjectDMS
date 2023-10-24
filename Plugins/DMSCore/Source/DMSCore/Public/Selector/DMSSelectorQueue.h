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



UCLASS(BlueprintType)
class DMSCORE_API UDMSWidgetQueue : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * List of widgets.
	 */
	UPROPERTY()
	TArray<TObjectPtr<UDMSConfirmWidgetBase>> Widgets;

	UPROPERTY()
	UDMSSequence* CurrentSequence;

	/**
	 * Current index of WidgetQueue.
	 */
	UPROPERTY()
	int8 CurrentIndex;

	UFUNCTION(BlueprintCallable)
	void ClearQueue();

	UFUNCTION()
	bool InitializeQueue(TArray<FDMSValueSelectionForm> RequestForms, APlayerController* WidgetOwner, UDMSSequence* iSequence);

	UFUNCTION()
	void PopupNextWidget();

	template<typename FuncFinished, typename FuncCanceled >
	void RunWidgetQueue(FuncFinished&& iOnQueueFinished, FuncCanceled&& iOnQueueCanceled);

	UFUNCTION(BlueprintCallable)
	void RedoWidgetQueue();

	DECLARE_DELEGATE_OneParam(FOnWidgetQueuesClosed, UDMSSequence*);

	/**
	 * Delegate to be called when the Selector Queue was completed.
	 */
	FOnWidgetQueuesClosed OnSelectorsCompleted;

	/**
	 * Delegate to be called when the Selector Queue was canceled.
	 */
	FOnWidgetQueuesClosed OnSelectorsCanceled;
};

template<typename FuncCompleted, typename FuncCanceled >
void UDMSWidgetQueue::RunWidgetQueue(FuncCompleted&& iOnQueueFinished, FuncCanceled&& iOnQueueCanceled)
{
	//OnSelectorsCompleted.BindLambda(std::forward<FuncCompleted&&>(iOnSelectorsCompleted));
	OnSelectorsCompleted.BindLambda(iOnQueueFinished);
	OnSelectorsCanceled.BindLambda(iOnQueueCanceled);
	PopupNextWidget();
}




//	시퀀스에 종속적인 구조체로써 시퀀스가 돌아갈 때 띄울 위젯들을 관리하는데 사용.
//	
//USTRUCT()
//struct FDMSSelectorQueue
//{
//	GENERATED_BODY()
//
//	FDMSSelectorQueue():CurrentIndex(-1){}
//
//	/**
//	 * Caching owner sequence.
//	 */
//	TObjectPtr<UDMSSequence> Owner;
//
//	/**
//	 * List of selectors.
//	 */
//	UPROPERTY()
//	TArray<UDMSConfirmWidgetBase*> SelectorQueue;
//
//	/**
//	 * Current index of SelectorQueue.
//	 */
//	//UPROPERTY()
//	int8 CurrentIndex;
//
//	/**
//	 * Current index of SelectorQueue.
//	 * @param	CurrentSequence					Owner sequence of selector queue.
//	 * @return	true if Setup was successful.
//	 */
//	__declspec(noinline) bool SetupQueue(UDMSSequence* OwnerSeq);
//
//	/**
//	 * Add confirm widget to SelectorQueue.
//	 * @param	iWidget						Adding widget.
//	 */
//	FORCEINLINE void AddSelector(UDMSConfirmWidgetBase* iWidget) { SelectorQueue.Add(iWidget); }
//	
//	/**
//	 * Run SelectorQueue.
//	 * @param	iOnSelectorsCompleted		Lambda parameter binded to OnSelectorsCompleted.
//	 * @param	iOnSelectorsCanceled		Lambda parameter binded to OnSelectorsCanceled.
//	 */
//	template<typename FuncCompleted, typename FuncCanceled >
//	void RunSelectors(FuncCompleted&& iOnSelectorsCompleted, FuncCanceled&& iOnSelectorsCanceled);
//	
//	DECLARE_DELEGATE_OneParam(FOnSelectorsClosed, UDMSSequence*);
//
//	/**
//	 * Delegate to be called when the Selector Queue was completed.
//	 */
//	FOnSelectorsClosed OnSelectorsCompleted;
//
//	/**
//	 * Delegate to be called when the Selector Queue was canceled.
//	 */
//	FOnSelectorsClosed OnSelectorsCanceled;
//	
//	/**
//	 * Run next selector (internal).
//	 */
//	void RunNextSelector();
//
//	/**
//	 * Redo widget queue.
//	 */
//	void RedoWidgetQueue();
//};

//template<typename FuncCompleted, typename FuncCanceled >
//void FDMSSelectorQueue::RunSelectors(FuncCompleted&& iOnSelectorsCompleted, FuncCanceled&& iOnSelectorsCanceled)
//{
//	OnSelectorsCompleted.BindLambda(std::forward<FuncCompleted&&>(iOnSelectorsCompleted));
//	OnSelectorsCompleted.BindLambda(iOnSelectorsCompleted);
//	OnSelectorsCanceled.BindLambda(iOnSelectorsCanceled);
//	RunNextSelector();
//}
