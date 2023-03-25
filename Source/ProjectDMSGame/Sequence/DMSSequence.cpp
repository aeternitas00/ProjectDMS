// Fill out your copyright notice in the Description page of Project Settings.


#include "Sequence/DMSSequence.h"
#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEffectorInterface.h"
#include "Selector/DMSEffectElementSelectorWidget.h"


//void UDMSSequence::InitializeWidgetQueue()
//{
//	SelectorQueue.Initialize(this);
//}

template<typename FuncFinished, typename FuncCanceled >
void UDMSSequence::RunWidgetQueue(FuncFinished&& iOnSelectorFinished, FuncCanceled&& iOnSelectorCanceled)
{
	SelectorQueue.RunSelectors(
		std::forward<FuncFinished&&>(iOnSelectorFinished),
		std::forward<FuncCanceled&&>(iOnSelectorCanceled)
	);
}

template<typename FuncFinished>
void UDMSSequence::AddToOnSequenceFinished_Native(FuncFinished&& OnSequenceFinished)
{
	DMS_LOG_SIMPLE(TEXT("==== %s : ADD TO SEQ FINISHIED ===="), *GetName());

	OnSequenceFinished_Native.AddLambda(OnSequenceFinished);
}

//void UDMSSequence::AddToSelectorQueue(UDMSEffectElementSelectorWidget* iWidget) { SelectorQueue.AddSelector(iWidget); }

//FORCEINLINE void AddToSelectorQueue(UDMSEffectElementSelectorWidget* iWidget) { SelectorQueue.AddSelector(iWidget); }

void UDMSSequence::AttachChildSequence(UDMSSequence* iSeq) 
{
	iSeq->ParentSequence = this;
	ChildSequence = iSeq;
}

void UDMSSequence::InitializeWidgetQueue(TArray<UDMSConfirmWidgetBase*> iWidgets, APlayerController* WidgetOwner)
{ 
	for(auto Widget : iWidgets) Widget->SetOwningPlayer(WidgetOwner);
	SelectorQueue.SelectorQueue.Empty();
	SelectorQueue.SelectorQueue.Append(iWidgets); 
	SelectorQueue.Initialize(this); 
}

void UDMSSequence::OnSequenceFinish()
{
	OnSequenceFinished.Broadcast();	
	OnSequenceFinished_Native.Broadcast();

	// Cleanup 
	OnSequenceFinished.Clear();
	OnSequenceFinished_Native.Clear();
}
