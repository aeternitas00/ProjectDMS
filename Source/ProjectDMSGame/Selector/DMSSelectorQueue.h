// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Common/DMSCommons.h"
#include "DMSSelectorQueue.generated.h"

class UDMSEffectElementSelectorWidget;

USTRUCT()
struct FDMSSelectorQueue
{
	GENERATED_BODY()

public:
	FDMSSelectorQueue():CurrentIndex(-1){}

	//Cache
	class UDMSSequence* Owner;

	UPROPERTY()
	TArray<UDMSEffectElementSelectorWidget*> SelectorQueue;

	//UPROPERTY()
	int8 CurrentIndex;

	// Chained Delegates..
	void Initialize(UDMSSequence* OwnerSeq);
	FORCEINLINE void AddSelector(UDMSEffectElementSelectorWidget* iWidget) { SelectorQueue.Add(iWidget); }
	
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