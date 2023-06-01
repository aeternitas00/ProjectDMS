// Fill out your copyright notice in the Description page of Project Settings.


#include "Selector/DMSConfirmWidgetBase.h"



void UDMSConfirmWidgetBase::CompleteSelect(UDMSDataObjectSet* OutData)
{
	OnSelectCompleted.ExecuteIfBound(OutData);
	//CloseSelector();
}

void UDMSConfirmWidgetBase::CancelSelect()
{
	OnSelectCanceled.ExecuteIfBound();
	//CloseSelector();
}

void UDMSConfirmWidgetBase::PopupSelector()
{
	AddToViewport();
	// multi?
	//AddToPlayerScreen();
	OnPopupSelector();
}

void UDMSConfirmWidgetBase::CloseSelector()
{
	//...
	RemoveFromParent();
	OnCloseSelector();
}