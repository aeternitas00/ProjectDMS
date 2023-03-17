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

//void UDMSSequence::AddToSelectorQueue(UDMSEffectElementSelectorWidget* iWidget) { SelectorQueue.AddSelector(iWidget); }

//FORCEINLINE void AddToSelectorQueue(UDMSEffectElementSelectorWidget* iWidget) { SelectorQueue.AddSelector(iWidget); }

void UDMSSequence::InitializeWidgetQueue(TArray<UDMSConfirmWidgetBase*> iWidgets, APlayerController* WidgetOwner)
{ 
	for(auto Widget : iWidgets) Widget->SetOwningPlayer(WidgetOwner);
	SelectorQueue.SelectorQueue.Empty();
	SelectorQueue.SelectorQueue.Append(iWidgets); 
	SelectorQueue.Initialize(this); 
}
