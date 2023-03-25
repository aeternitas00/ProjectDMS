// Fill out your copyright notice in the Description page of Project Settings.


#include "Selector/DMSSelectorQueue.h"
#include "Sequence/DMSSequence.h"
#include "DMSSelectorQueue.h"
#include "Selector/DMSEffectElementSelectorWidget.h"

void FDMSSelectorQueue::Initialize(UDMSSequence* OwnerSeq)
{
	CurrentIndex = -1;
	Owner = OwnerSeq;

	for (int32 i = SelectorQueue.Num() - 1; i >= 0; --i) {

		SelectorQueue[i]->SetupWidgetDelegates([&, Self=SelectorQueue[i]](UDMSDataObjectSet* Data) {
			// [ OK Bttn ]
			Owner->EIDatas->Merge(Data);
			Self->CloseSelector();
			RunNextSelector();
			//...
		},	[&]() {
			// [ X Bttn ]
			//OwnerSeq->UpdateData(Data);
			for (auto sWidget : SelectorQueue) { sWidget->CloseSelector(); }
			OnSelectorsCanceled.Execute(Owner);
			//...
		},	Owner);

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
	int8 LocalIdx = ++CurrentIndex;
	if (SelectorQueue.Num() == LocalIdx) {
		OnSelectorsCompleted.Execute(Owner);
		return;
	}
	SelectorQueue[LocalIdx]->PopupSelector();
	if (!SelectorQueue[LocalIdx]->SetupWidget()) {
		SelectorQueue[LocalIdx]->CloseSelector();
		RunNextSelector();
	}

}
