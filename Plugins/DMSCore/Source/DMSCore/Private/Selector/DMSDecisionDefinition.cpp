// Fill out your copyright notice in the Description page of Project Settings.


#include "Selector/DMSDecisionDefinition.h"
#include "Effect/DMSEffectInstance.h"
#include "Sequence/DMSSequence.h"
#include "Common/DMSTargetGenerator.h"
#include "Library/DMSDataObjectHelperLibrary.h"

// 리퀘스트 폼을 제작 

TArray<FDMSSelectorRequestForm> UDMSSelectorRequestGenerator::GenerateRequestForm(UDMSSequence* Sequence, bool SelectForEachEI)
{
	TArray<FDMSSelectorRequestForm> Rv;

	if (SelectForEachEI) {
		TArray<UDMSEffectInstance*> EIArr = Sequence->GetAllEIs();
		for (auto& TargetEI : EIArr)
		{
			FDMSSelectorRequestForm NewForm;
			NewForm.Candidates = GenerateCandidates(Sequence, TargetEI);
			NewForm.SelectAmount = SelectAmount;
			NewForm.SelectorClass = SelectorClass;
			Rv.Add(NewForm);
		}
	}
	else {
		FDMSSelectorRequestForm NewForm;
		NewForm.Candidates = GenerateCandidates(Sequence, nullptr);
		NewForm.SelectAmount = SelectAmount;
		NewForm.SelectorClass = SelectorClass;
		Rv.Add(NewForm);
	}
	return Rv;
}


TArray<UDMSDataObject*> UDMSSelectorRequestGenerator::GenerateCandidates(UDMSSequence* Sequence, UDMSEffectInstance* TargetEI)
{
	return TArray<UDMSDataObject*>();
}

TArray<FDMSSelectorRequestForm> FDMSDecisionDefinition::SetupRequestForm ( UDMSSequence* Sequence )
{
	auto Rv = Generator->GenerateRequestForm ( Sequence , SelectForEachEI );
	Behavior->SetupFormDelegates ( Sequence , Rv , SelectForEachEI );
	return Rv;
}







TArray<UDMSDataObject*> USelectorRequestGenerator_FromSequenceData::GenerateCandidates ( UDMSSequence* Sequence , UDMSEffectInstance* TargetEI )
{
	TArray<UDMSDataObject*> rv;
	Sequence->SequenceDatas->GetValidDataValue<TArray<UDMSDataObject*>>(DataTag,rv);
	return rv;
}

TArray<UDMSDataObject*> USelectorRequestGenerator_FromEIData::GenerateCandidates ( UDMSSequence* Sequence , UDMSEffectInstance* TargetEI )
{
	TArray<UDMSDataObject*> rv;
	TargetEI->DataSet->GetValidDataValue<TArray<UDMSDataObject*>>(DataTag,rv);
	return rv;
}


TArray<UDMSDataObject*> USelectorRequestGenerator_WithTG::GenerateCandidates(UDMSSequence* Sequence, UDMSEffectInstance* TargetEI)
{
	auto Targets = TargetGenerator->GetTargets(Sequence->GetSourceObject(),Sequence);
	return UDMSDataObjectHelperLibrary::RawDataToDataObjectArr(Targets,Sequence);
}





void UDMSSelectorBehaviorDefinition_UpdateData::SetupFormDelegates(UDMSSequence* Sequence, TArray<FDMSSelectorRequestForm>& FormArr, bool SelectForEachEI)
{
	if (SelectForEachEI) // FormArr.Num() == EIArr.Num()
	{
		int FormIdx = 0;	
		TArray<UDMSEffectInstance*> EIArr = Sequence->GetAllEIs();
		for (auto& Form : FormArr)
		{
			// 이거 순서가 확실히 유지되나?
			Form.OnCompletedNative.BindLambda([=,EI = EIArr[FormIdx]](TArray<uint8> IndexArr)
			{
				DMS_LOG_SIMPLE(TEXT("==== %s : SelectorBehavior : Update Data ===="), *EI->GetName());

				TArray<UDMSDataObject*> SelectedItems;
				for (auto& i : IndexArr)
				{
					UDMSDataObject* TargetObject = Form.Candidates[i];
					SelectedItems.Add(TargetObject);
				}
				EI->DataSet->SetData(OutKey, SelectedItems);
			});
			FormIdx++;
		}

	}
	else // FormArr.Num() == '1'
	{
		FormArr[0].OnCompletedNative.BindLambda([=](TArray<uint8> IndexArr)
			{
				TArray<UDMSDataObject*> SelectedItems;
				for (auto& i : IndexArr)
				{
					UDMSDataObject* TargetObject = FormArr[0].Candidates[i];
					SelectedItems.Add(TargetObject);
				}
				Sequence->SequenceDatas->SetData(OutKey, SelectedItems);
			});
	}
}

