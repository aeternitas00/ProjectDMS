// Fill out your copyright notice in the Description page of Project Settings.


#include "Selector/DMSConfirmWidgetBase.h"
#include "Sequence/DMSSequence.h"
#include "Effect/DMSEffectInstance.h"

void UDMSConfirmWidgetBase::CompleteSelect()
{
	if (SelectionForm.ForEachTargets && EIArray.Num()< CurrentEIIndex++)
		SetupWidget();
	
	else
	OnSelectCompleted.ExecuteIfBound();
	//CloseSelector();
}

void UDMSConfirmWidgetBase::CancelSelect()
{
	OnSelectCanceled.ExecuteIfBound();
	//CloseSelector();
}

UDMSDataObject* UDMSConfirmWidgetBase::GetCurrentCandidatesData()
{
	if (!SelectionForm.ValueSelector->bIsCandidatesFromData) return nullptr;
	return SelectionForm.ForEachTargets ? CandidatesData[CurrentEIIndex] : CandidatesData[0];
}

UDMSDataObjectSet* UDMSConfirmWidgetBase::GetTargetDataSet_Implementation()
{
	return SelectionForm.ForEachTargets ? EIArray[CurrentEIIndex]->DataSet : CurrentSequence->SequenceDatas;
}

bool UDMSConfirmWidgetBase::InitializeWidget(FDMSValueSelectionForm InitializeData,UDMSSequence* iSequence)
{
	SelectionForm = InitializeData;
	CurrentSequence = iSequence;

	if (SelectionForm.ValueSelector->bIsCandidatesFromData){
		if (SelectionForm.ForEachTargets){
			EIArray = CurrentSequence->GetAllEIs();
			for (auto& EI : EIArray)
				CandidatesData.Add(EI->DataSet->GetData(SelectionForm.ValueSelector->CandidatesKey));
		}
		else{
			CandidatesData.Add(CurrentSequence->SequenceDatas->GetData(SelectionForm.ValueSelector->CandidatesKey));
		}
	}
	else
		GenerateCandidaes();

	return SetupWidget();
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