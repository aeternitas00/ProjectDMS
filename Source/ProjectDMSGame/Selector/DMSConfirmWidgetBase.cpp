// Fill out your copyright notice in the Description page of Project Settings.


#include "Selector/DMSConfirmWidgetBase.h"


template<typename FuncCompleted, typename FuncCanceled>
void UDMSConfirmWidgetBase::SetupWidgetDelegates(FuncCompleted&& iOnCompleted, FuncCanceled&& iOnCanceled, UDMSSequence* iOwnerSeq) {

	OwnerSeq = iOwnerSeq;
	//if (!SetupWidget()){return false;};

	OnSelectCompleted.Unbind();
	OnSelectCanceled.Unbind();

	OnSelectCompleted.BindLambda(std::forward<FuncCompleted&&>(iOnCompleted));
	OnSelectCanceled.BindLambda(std::forward<FuncCanceled&&>(iOnCanceled));

	//return true;
}


void UDMSConfirmWidgetBase::CompleteSelect(UDMSDataObjectSet* OutData)
{
	OnSelectCompleted.Execute(OutData);
	CloseSelector();
}

void UDMSConfirmWidgetBase::CancelSelect()
{
	OnSelectCanceled.Execute();
}

void UDMSConfirmWidgetBase::PopupSelector()
{
	// if (Candidates.Num() == 1) { /*autoselect and close..*/ } 
	//AddToViewport();
	AddToPlayerScreen();
}

void UDMSConfirmWidgetBase::CloseSelector()
{
	//...
	RemoveFromParent();
}