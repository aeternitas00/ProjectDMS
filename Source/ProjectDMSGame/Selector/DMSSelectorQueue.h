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

#include "ProjectDMS.h"
#include "Common/DMSCommons.h"
#include "DMSSelectorQueue.generated.h"

class UDMSConfirmWidgetBase;
class UDMSSequence;

//	시퀀스에 종속적인 구조체로써 시퀀스가 돌아갈 때 띄울 위젯들을 관리하는데 사용.
//	
USTRUCT()
struct FDMSSelectorQueue
{
	GENERATED_BODY()

	FDMSSelectorQueue():CurrentIndex(-1){}

	/**
	 * Caching owner sequence.
	 */
	TObjectPtr<UDMSSequence> Owner;

	/**
	 * List of selectors.
	 */
	UPROPERTY()
	TArray<UDMSConfirmWidgetBase*> SelectorQueue;

	/**
	 * Current index of SelectorQueue.
	 */
	//UPROPERTY()
	int8 CurrentIndex;

	/**
	 * Current index of SelectorQueue.
	 * @param	OwnerSeq					Owner sequence of selector queue.
	 * @return	true if Setup was successful.
	 */
	bool SetupQueue(UDMSSequence* OwnerSeq);

	/**
	 * Add confirm widget to SelectorQueue.
	 * @param	iWidget						Adding widget.
	 */
	FORCEINLINE void AddSelector(UDMSConfirmWidgetBase* iWidget) { SelectorQueue.Add(iWidget); }
	
	/**
	 * Run SelectorQueue.
	 * @param	iOnSelectorsCompleted		Lambda parameter binded to OnSelectorsCompleted.
	 * @param	iOnSelectorsCanceled		Lambda parameter binded to OnSelectorsCanceled.
	 */
	template<typename FuncCompleted, typename FuncCanceled >
	void RunSelectors(FuncCompleted&& iOnSelectorsCompleted, FuncCanceled&& iOnSelectorsCanceled);
	
	DECLARE_DELEGATE_OneParam(FOnSelectorsClosed, UDMSSequence*);

	/**
	 * Delegate to be called when the Selector Queue was completed.
	 */
	FOnSelectorsClosed OnSelectorsCompleted;

	/**
	 * Delegate to be called when the Selector Queue was canceled.
	 */
	FOnSelectorsClosed OnSelectorsCanceled;
	
	/**
	 * Run next selector (internal).
	 */
	void RunNextSelector();
};
