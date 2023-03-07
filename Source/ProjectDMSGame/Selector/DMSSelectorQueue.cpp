// Fill out your copyright notice in the Description page of Project Settings.


#include "Selector/DMSSelectorQueue.h"
#include "Sequence/DMSSequence.h"
#include "DMSSelectorQueue.h"
#include "Effect/DMSEffectElementSelectorWidget.h"

void FDMSSelectorQueue::Initialize(UDMSSequence* OwnerSeq)
{
	CurrentIndex = -1;
	Owner = OwnerSeq;
	for (auto Selector : SelectorQueue)
	{
		//NEED PARAM?
		Selector->InitializeSelector_Internal([&](UDMSDataObjectSet* Data){
			
			Owner->EIDatas->Merge(Data);		
			RunNextSelector();
			//...
		},
		[&]() {
			//OwnerSeq->UpdateData(Data);
			for ( auto sWidget : SelectorQueue ) {sWidget->CloseSelector();}
			OnSelectorsCanceled.Execute(Owner);
			//...
		}, 
		Owner);
	}
}

template<typename FuncCompleted, typename FuncCanceled >
void FDMSSelectorQueue::RunSelectors(FuncCompleted&& iOnSelectorsCompleted, FuncCanceled&& iOnSelectorsCanceled)
{
	//OnSelectorsCompleted.BindLambda(std::forward<FuncCompleted&&>(iOnSelectorsCompleted));
	OnSelectorsCompleted.BindLambda(iOnSelectorsCompleted);
	OnSelectorsCanceled.BindLambda(iOnSelectorsCanceled);
	RunNextSelector();
}


void FDMSSelectorQueue::RunNextSelector()
{
	CurrentIndex++;
	if (SelectorQueue.Num() == CurrentIndex ) {
		OnSelectorsCompleted.Execute(Owner);
		return;
	}
	if (!SelectorQueue[CurrentIndex]->PopupSelector()) {
		SelectorQueue[CurrentIndex]->CloseSelector();
		RunNextSelector();
		return;
	}

}

//inline void FDMSSelectorQueue::AddSelector(UDMSEffectElementSelectorWidget* iWidget) { SelectorQueue.Add(iWidget); }
