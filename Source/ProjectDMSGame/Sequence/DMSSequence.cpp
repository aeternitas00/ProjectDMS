// Fill out your copyright notice in the Description page of Project Settings.


#include "Sequence/DMSSequence.h"
#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEffectorInterface.h"
#include "Effect/DMSEffectElementSelectorWidget.h"


//void UDMSSequence::InitializeSelectorQueue()
//{
//	SelectorQueue.Initialize(this);
//}

template<typename FuncFinished, typename FuncCanceled >
void UDMSSequence::RunSelectorQueue(FuncFinished&& iOnSelectorFinished, FuncCanceled&& iOnSelectorCanceled)
{
	SelectorQueue.RunSelectors(
		std::forward<FuncFinished&&>(iOnSelectorFinished),
		std::forward<FuncCanceled&&>(iOnSelectorCanceled)
	);
}

//void UDMSSequence::AddToSelectorQueue(UDMSEffectElementSelectorWidget* iWidget) { SelectorQueue.AddSelector(iWidget); }
