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

//	시퀀스에 종속적인 구조체로써 시퀀스가 돌아갈 때 띄울 위젯들을 관리하는데 사용.
//	
USTRUCT()
struct FDMSSelectorQueue
{
	GENERATED_BODY()

public:
	FDMSSelectorQueue():CurrentIndex(-1){}

	//Cache
	class UDMSSequence* Owner;

	UPROPERTY()
	TArray<UDMSConfirmWidgetBase*> SelectorQueue;

	//UPROPERTY()
	int8 CurrentIndex;

	// Chained Delegates..
	void Initialize(UDMSSequence* OwnerSeq);
	FORCEINLINE void AddSelector(UDMSConfirmWidgetBase* iWidget) { SelectorQueue.Add(iWidget); }
	
	template<typename FuncCompleted, typename FuncCanceled >
	void RunSelectors(FuncCompleted&& iOnSelectorsCompleted, FuncCanceled&& iOnSelectorsCanceled);

	void RunNextSelector();

	DECLARE_DELEGATE_OneParam(FOnSelectorsClosed, UDMSSequence*);

	FOnSelectorsClosed OnSelectorsCompleted;
	FOnSelectorsClosed OnSelectorsCanceled;
};

//UCLASS()
//class PROJECTDMSGAME_API UDMSSelectorQueue : public UObject;
//{
//
//}