// Fill out your copyright notice in the Description page of Project Settings.


#include "Sequence/DMSSequence.h"
#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEffectorInterface.h"
#include "Selector/DMSEffectElementSelectorWidget.h"

template<typename FuncFinished, typename FuncCanceled >
void UDMSSequence::RunWidgetQueue(FuncFinished&& iOnSelectorFinished, FuncCanceled&& iOnSelectorCanceled)
{
	SelectorQueue.RunSelectors(
		std::forward<FuncFinished&&>(iOnSelectorFinished),
		std::forward<FuncCanceled&&>(iOnSelectorCanceled)
	);
}

template<typename FuncInitiated>
void UDMSSequence::AddToOnSequenceInitiated_Native(FuncInitiated&& iOnSequenceInitiated)
{
	OnSequenceFinished.AddLambda(iOnSequenceInitiated);
}

template<typename FuncFinished>
void UDMSSequence::AddToOnSequenceFinished_Native(FuncFinished&& iOnSequenceFinished)
{
	DMS_LOG_SIMPLE(TEXT("==== %s : ADD TO SEQ FINISHIED ===="), *GetName());

	if (OnSequenceFinished.IsBound()) {
		DMS_LOG_SIMPLE(TEXT("==== %s : SEQ FINISHIED HAS MUILTIPLE DELEGATES ===="), *GetName());
	}
	OnSequenceFinished.AddLambda(iOnSequenceFinished);

}

void UDMSSequence::AddToOnSequenceInitiated(const FOnSequenceStateChanged_Signature& iOnSequenceInitiated)
{
	OnSequenceInitiated_Dynamic.Add(iOnSequenceInitiated);
}

void UDMSSequence::AddToOnSequenceFinished(const FOnSequenceStateChanged_Signature& iOnSequenceFinished)
{
	DMS_LOG_SIMPLE(TEXT("==== %s : ADD TO SEQ FINISHIED ===="), *GetName());

	if (OnSequenceFinished_Dynamic.IsBound()) {
		DMS_LOG_SIMPLE(TEXT("==== %s : SEQ FINISHIED HAS MUILTIPLE DELEGATES ===="), *GetName());
	}
	OnSequenceFinished_Dynamic.Add(iOnSequenceFinished);
}

void UDMSSequence::AttachChildSequence(UDMSSequence* iSeq) 
{
	iSeq->ParentSequence = this;
	ChildSequence = iSeq;
}

bool UDMSSequence::SetupWidgetQueue(TArray<UDMSConfirmWidgetBase*> iWidgets)
{ 
	//for(auto Widget : iWidgets) Widget->SetOwningPlayer(WidgetOwner);
	SelectorQueue.SelectorQueue.Empty();
	SelectorQueue.SelectorQueue.Append(iWidgets); 
	bool rv= SelectorQueue.SetupQueue(this);
	return rv;
}

void UDMSSequence::OnSequenceInitiate()
{
	OnSequenceInitiated.Broadcast();
	OnSequenceInitiated_Dynamic.Broadcast();
}

void UDMSSequence::OnSequenceFinish()
{
	auto temp = std::move(OnSequenceFinished);
	auto temp_Dynamic = std::move(OnSequenceFinished_Dynamic);
	OnSequenceFinished = FSimpleMulticastEventSignature();
	OnSequenceFinished_Dynamic = FOnSequenceStateChanged_Dynamic();
	temp.Broadcast();
	temp_Dynamic.Broadcast();
	// Cleanup 
	
}

