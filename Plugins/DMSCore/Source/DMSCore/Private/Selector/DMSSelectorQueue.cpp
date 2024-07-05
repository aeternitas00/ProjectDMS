// Copyright JeongWoo Lee


#include "Selector/DMSSelectorQueue.h"
#include "Selector/DMSConfirmWidgetBase.h"
#include "Selector/DMSSelectorManager.h"

#include "Library/DMSCoreFunctionLibrary.h"

#include "GameModes/DMSGameStateBase.h"

#include "Player/DMSPlayerControllerBase.h"
#include "Sequence/DMSSeqManager.h"
#include "Sequence/DMSSequence.h"


void UDMSSelectorWorker::SetupSelectorWorker(ADMSSequence* iSequence, ADMSPlayerControllerBase* iWidgetOwner)
{
	CurrentSequence = iSequence;
	WidgetOwner = iWidgetOwner;
}

void UDMSSelectorWorker::Work_Implementation()
{
	auto CurrentHandle = Cast<UDMSSelectorHandle>(GetCurrentContext());
	CurrentHandle->bAutoExecuteForm=false;

	FOnSelectCompletedNative LazyUpdaterDelegate;

	LazyUpdaterDelegate.BindLambda([=,this](bool Succeed, const TArray<uint8>& SelectIndexes){

		OnAllSelectorCompleted.AddLambda ( [=,this](){
			CurrentHandle->StoredForm->OnCompleted.Broadcast(SelectIndexes);
			CurrentHandle->StoredForm->OnCompletedNative.ExecuteIfBound(SelectIndexes);
		});

		CompleteSingleTask(Succeed);
	});

	CurrentHandle->SetupDelegate(LazyUpdaterDelegate);
	CurrentHandle->RunSelector();
}

void UDMSSelectorWorker::OnAllTaskCompleted_Implementation(bool WorkerSucceeded)
{
	if(WorkerSucceeded) 
		OnAllSelectorCompleted.Broadcast();

	auto SelM = UDMSCoreFunctionLibrary::GetDMSSelectorManager(CurrentSequence);

	SelM->DeregisterWorker(this);
}








//UDMSWidgetQueue::UDMSWidgetQueue():CurrentIndex(-1)
//{
//}

//bool UDMSWidgetQueue::SetupQueue(ADMSSequence* SourceSequence, TArray<UDMSSelectorHandle*> Handles, const FOnTaskCompletedNative& OnQueueCompleted)
//{
//	auto WidgetOwner = GetWidgetOwner();
//	if (WidgetOwner==nullptr) return false;
//	////CurrentIndex = -1;
//	////CurrentSequence = SourceSequence;
//	////SelectorHandles = Handles;
//
//	//UDMSSelectorWorker* NewWorker = NewObject<UDMSSelectorWorker>(WidgetOwner);
//
//	//TArray<UObject*> Contexts;
//	//for(auto Handle:Handles) Contexts.Add(Handle);
//
//	//NewWorker->SetupSelectorWorker(SourceSequence);
//	//NewWorker->SetupTaskWorkerDelegate_Native(Contexts,OnQueueCompleted);
//
//	//FSimpleDelegate SuccessDelegate;
//	//FSimpleDelegate CancelDelegate;
//	//SuccessDelegate.BindLambda([this, Handle](){	Handle->Widget->CloseSelector();	PopupNextWidget();});	
//	//SuccessDelegate.BindLambda([this](){ OnSelectorsCanceled.Broadcast(CurrentSequence); ClearQueue(); });
//
//	//for ( auto& Handle : SelectorHandles )
//	//{
//	//	if (!Handle->SetupSelector(WidgetOwner)) return false;
//	//	Handle->OwnerQueue=this;
//	//	Handle->SetupDelegate(SuccessDelegate,CancelDelegate);
//	//}
//
//	return true;
//}
//
//void UDMSWidgetQueue::ClearQueue()
//{
//	for (auto Handle : SelectorHandles)
//	{
//		Handle->CloseSelector(); Handle->MarkAsGarbage();
//	}
//	OnSelectorsCompleted.Clear();
//	OnSelectorsCompleted_Handle.Clear();
//	OnSelectorsCanceled.Clear();
//	SelectorHandles.Empty(); CurrentIndex = -1;
//	
//}
//
//void UDMSWidgetQueue::PopupNextWidget()
//{
//	int8 LocalIdx = ++CurrentIndex;
//	if (SelectorHandles.Num() <= LocalIdx) {		
//		auto CopiedCompleted_Handle = OnSelectorsCompleted_Handle;
//		auto CopiedCompleted = OnSelectorsCompleted;
//
//		ClearQueue();
//		// Broadcast Behavior of handles first
//		CopiedCompleted_Handle.Broadcast(CurrentSequence);
//		// Complete the step
//		CopiedCompleted.Broadcast(CurrentSequence);
//
//		return;
//	}
//	SelectorHandles[LocalIdx]->RunSelector();
//}
//
//
//void UDMSWidgetQueue::RedoWidgetQueue()
//{
//	CurrentIndex = -1;
//	OnSelectorsCompleted_Handle.Clear();
//	for ( auto& Handle : SelectorHandles ) {
//		Handle->CloseSelector();
//	}
//
//	PopupNextWidget();
//}
