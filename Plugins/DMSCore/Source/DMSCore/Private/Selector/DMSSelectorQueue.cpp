// Fill out your copyright notice in the Description page of Project Settings.


#include "Selector/DMSSelectorQueue.h"
#include "Selector/DMSConfirmWidgetBase.h"
#include "Selector/DMSSelectorManager.h"

#include "Library/DMSCoreFunctionLibrary.h"

#include "GameModes/DMSGameStateBase.h"

#include "Player/DMSPlayerControllerBase.h"
#include "Sequence/DMSSeqManager.h"
#include "Sequence/DMSSequence.h"

void UDMSWidgetQueue::RedoWidgetQueue()
{
	CurrentIndex = -1;
	OnSelectorsCompleted_Handle.Clear();
	for ( auto& Handle : SelectorHandles ) {
		Handle->CloseSelector();
	}

	PopupNextWidget();
}

UDMSWidgetQueue::UDMSWidgetQueue():CurrentIndex(-1)
{
}

bool UDMSWidgetQueue::SetupQueue(UDMSSequence* SourceSequence,TArray<UDMSSelectorHandle*> Handles)
{
	auto WidgetOwner = GetWidgetOwner();
	if (WidgetOwner==nullptr) return false;
	CurrentIndex = -1;
	CurrentSequence=SourceSequence;
	SelectorHandles = Handles;

	for ( auto& Handle : SelectorHandles )
	{
		if (!Handle->SetupSelector(WidgetOwner)) return false;
		Handle->OwnerQueue=this;
		Handle->SetupDelegates(
			[this, Handle](){	Handle->Widget->CloseSelector();	PopupNextWidget(); } ,
			[this](){ClearQueue(); OnSelectorsCanceled.Broadcast(CurrentSequence); }
		);
	}

	return true;
}

void UDMSWidgetQueue::ClearQueue()
{
	for (auto Handle : SelectorHandles)
	{
		Handle->CloseSelector(); Handle->MarkAsGarbage();
	}
	OnSelectorsCompleted.Clear();
	OnSelectorsCompleted_Handle.Clear();
	SelectorHandles.Empty(); CurrentIndex = -1;
	
}

void UDMSWidgetQueue::PopupNextWidget()
{
	int8 LocalIdx = ++CurrentIndex;
	if (SelectorHandles.Num() <= LocalIdx) {
		auto CopiedCompleted = OnSelectorsCompleted;
		auto CopiedCompleted_Handle = OnSelectorsCompleted_Handle;
		ClearQueue();
		CopiedCompleted.Broadcast(CurrentSequence);
		CopiedCompleted_Handle.Broadcast(CurrentSequence);
		return;
	}
	SelectorHandles[LocalIdx]->RunSelector();
}
