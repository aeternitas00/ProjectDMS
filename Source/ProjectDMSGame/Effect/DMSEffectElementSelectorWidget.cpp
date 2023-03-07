// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/DMSEffectElementSelectorWidget.h"
#include "Effect/DMSEffectDefinition.h"

//template<typename FuncCompleted, typename FuncCanceled>
//void UDMSEffectElementSelectorWidget::InitializeSelector_Internal(FuncCompleted&& iOnCompleted, FuncCanceled&& iOnCanceled, UDMSSequence* iOwnerSeq) {
//	//CandidatesData=nullptr;
//	//SourceEffectDefinition=nullptr;
//	OnSelectCompleted.Unbind();
//	OnSelectCanceled.Unbind();
//	//support for l-value 
//	OnSelectCompleted.BindLambda(std::forward<FuncCompleted&&>(iOnCompleted));
//	OnSelectCanceled.BindLambda(std::forward<FuncCanceled&&>(iOnCanceled));
//	//OnSelectCompleted.BindLambda(Lambda);
//	OwnerSeq = iOwnerSeq;
//
//	InitializeSelector(OwnerSeq);
//}

void UDMSEffectElementSelectorWidget::CompleteSelect(UDMSDataObjectSet* OutData)
{
	OnSelectCompleted.Execute(OutData);
	CloseSelector();
}

void UDMSEffectElementSelectorWidget::CancelSelect()
{
	OnSelectCanceled.Execute();
}

bool UDMSEffectElementSelectorWidget::PopupSelector()
{
	AddToViewport();
	bool rv=SetupCandidates();
	return rv;
	// if (Candidates.Num() == 1) { /*autoselect and close..*/ } 
}

void UDMSEffectElementSelectorWidget::CloseSelector()
{
	//...
	RemoveFromParent();
}


bool UDMSEffectElementSelectorWidget::GetCandidatesFromED()
{
	return SourceEffectDefinition->GetCandidates(OwnerSeq,CandidatesData);
}


