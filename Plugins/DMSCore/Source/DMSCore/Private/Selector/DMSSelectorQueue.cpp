// Fill out your copyright notice in the Description page of Project Settings.


#include "Selector/DMSSelectorQueue.h"
#include "Sequence/DMSSequence.h"
#include "Selector/DMSConfirmWidgetBase.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "GameModes/DMSGameStateBase.h"
#include "Player/DMSPlayerControllerBase.h"
#include "Sequence/DMSSeqManager.h"


void UDMSWidgetQueue::RedoWidgetQueue()
{
	CurrentIndex = -1;
	for (auto sWidget : Widgets) { sWidget->CloseSelector(); }
	PopupNextWidget();
}

void UDMSWidgetQueue::ClearQueue()
{
	for (auto Widget : Widgets)
	{
		Widget->CloseSelector(); Widget->MarkAsGarbage();
	}
	
	Widgets.Empty(); CurrentIndex = -1;
	
}

bool UDMSWidgetQueue::InitializeQueue(TArray<FDMSValueSelectionForm> RequestForms, APlayerController* WidgetOwner, UDMSSequence* iSequenece)
{
	ClearQueue();
	CurrentSequence = iSequenece;
	auto SM = UDMSCoreFunctionLibrary::GetDMSSequenceManager(); check(SM);
	for (auto& RequestForm : RequestForms)
	{
		auto WidgetClass = RequestForm.ValueSelector->GetWidgetClass();
		if (WidgetClass == nullptr) {
			ClearQueue();
			return false;
		}
		auto NewWidget = CreateWidget<UDMSConfirmWidgetBase>(WidgetOwner, WidgetClass);
		if (!NewWidget->InitializeWidget(RequestForm, CurrentSequence)) {
			ClearQueue();
			return false;
		}
		NewWidget->SetupWidgetDelegates(
			[this,Self = NewWidget](){	Self->CloseSelector();	PopupNextWidget(); },
			[this](){ClearQueue(); OnSelectorsCanceled.Execute(CurrentSequence);}
		);
		Widgets.Add(NewWidget);
	}

	return true;
}

void UDMSWidgetQueue::PopupNextWidget()
{
	int8 LocalIdx = ++CurrentIndex;
	if (Widgets.Num() <= LocalIdx) {
		OnSelectorsCompleted.Execute(CurrentSequence);
		return;
	}
	Widgets[LocalIdx]->PopupSelector();
}


//
//bool FDMSSelectorQueue::SetupQueue(UDMSSequence* OwnerSeq)
//{
//	Owner = OwnerSeq;
//	if (SelectorQueue.Num()==0) return true;
//	CurrentIndex = -1;
//
//	for (auto Selector : SelectorQueue){
//		if (!Selector->SetupWidget() ) 
//			return false;
//	}
//
//	for (int32 i = SelectorQueue.Num() - 1; i >= 0; --i) {
//
//		SelectorQueue[i]->SetupWidgetDelegates([&, Self=SelectorQueue[i]](UDMSDataObjectSet* Data) {
//			// [ OK Bttn ]
//			Owner->SequenceDatas->Merge(Data);
//			Self->CloseSelector();
//			RunNextSelector();
//			//...
//		},	[&]() {
//			// [ X Bttn ]
//			//CurrentSequence->UpdateData(Data);
//			for (auto sWidget : SelectorQueue) { sWidget->CloseSelector(); }
//			OnSelectorsCanceled.Execute(Owner);
//			//...
//		});
//
//	}
//	return true;
//}




//void FDMSSelectorQueue::RunNextSelector()
//{
//	int8 LocalIdx = ++CurrentIndex;
//	if (SelectorQueue.Num() <= LocalIdx) {
//		OnSelectorsCompleted.Execute(Owner);
//		return;
//	}
//	SelectorQueue[LocalIdx]->PopupSelector();
//
//}