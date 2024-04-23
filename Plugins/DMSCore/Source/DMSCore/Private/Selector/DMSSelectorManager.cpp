// Fill out your copyright notice in the Description page of Project Settings.


#include "Selector/DMSSelectorManager.h"
#include "Selector/DMSConfirmWidgetBase.h"
// Sets default values for this component's properties
UDMSSelectorManager::UDMSSelectorManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	

	// ...
}

UDMSSelectorHandle* UDMSSelectorManager::RequestCreateSelector(UDMSSelectorRequestForm* Form)
{
	auto rv = NewObject<UDMSSelectorHandle>(this);

	rv->StoredForm = Form;
	return rv;
}

TArray<UDMSSelectorHandle*> UDMSSelectorManager::RequestCreateSelectors ( TArray<UDMSSelectorRequestForm*>& Forms )
{
	TArray<UDMSSelectorHandle*> rv;
	for ( auto& Form : Forms )
		rv.Add(RequestCreateSelector(Form));
	
	return rv;
}

UDMSSelectorHandle::UDMSSelectorHandle():Widget(nullptr),OwnerQueue(nullptr)
{
}

void UDMSSelectorHandle::RunSelector()
{
	Widget->PopupSelector();
}

void UDMSSelectorHandle::HideSelector()
{
	Widget->SetVisibility(ESlateVisibility::Collapsed);
}

void UDMSSelectorHandle::CreateSelectorWidget(APlayerController* WidgetOwner)
{
	if (Widget != nullptr || WidgetOwner == nullptr) return; 
	Widget = CreateWidget<UDMSSelectorBase>(WidgetOwner, StoredForm->SelectorClass);
	Widget->OwnerHandle = this;
}

bool UDMSSelectorHandle::SetupSelector(APlayerController* WidgetOwner)
{
	CreateSelectorWidget(WidgetOwner);
	return Widget != nullptr ? Widget->InitializeSelector(StoredForm) : false;
}

void UDMSSelectorHandle::CloseSelector()
{
	if (Widget==nullptr) return;
	Widget->CloseSelector();
}

void UDMSSelectorHandle::CompleteHandle(const TArray<uint8>& SelectedIdx)
{
	if ( OwnerQueue == nullptr ) {
		StoredForm->OnCompleted.Broadcast( SelectedIdx );
		StoredForm->OnCompletedNative.ExecuteIfBound( SelectedIdx );
	}
	else {
		OwnerQueue->OnSelectorsCompleted_Handle.AddLambda ( [=](ADMSSequence* ) {
			StoredForm->OnCompleted.Broadcast(SelectedIdx);
			StoredForm->OnCompletedNative.ExecuteIfBound(SelectedIdx);
		} );
	}

	OnSelectCompleted.Broadcast();
}

void UDMSSelectorHandle::CancelHandle()
{
	OnSelectCanceled.Broadcast();
}
