// Fill out your copyright notice in the Description page of Project Settings.


#include "Selector/DMSDecisionDefinition.h"
#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEffectHandler.h"
#include "Sequence/DMSSequence.h"
#include "Common/DMSTargetGenerator.h"
#include "Library/DMSDataObjectHelperLibrary.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Attribute/DMSAttributeComponent.h"
// 리퀘스트 폼을 제작 
TArray<FDMSSelectorRequestForm> UDMSSelectorRequestGenerator::GenerateRequestForm(ADMSSequence* Sequence, bool SelectForEachEI)
{
	TArray<FDMSSelectorRequestForm> Rv;

	if (SelectForEachEI) {
		TArray<ADMSActiveEffect*> EIArr = Sequence->GetAllEIs();
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


TArray<UObject*> UDMSSelectorRequestGenerator::GenerateCandidates(ADMSSequence* Sequence, ADMSActiveEffect* TargetEI)
{
	return TArray<UObject*>();
}

TArray<FDMSSelectorRequestForm> FDMSDecisionDefinition::SetupRequestForm( ADMSSequence* Sequence )
{
	auto temp = SelectForEachEI && Sequence->IsTargetted();
	auto Rv = Generator->GenerateRequestForm( Sequence , temp );
	Behavior->SetupFormDelegates( Sequence , Rv , temp );
	return Rv;
}







TArray<UObject*> USelectorRequestGenerator_FromSequenceData::GenerateCandidates ( ADMSSequence* Sequence , ADMSActiveEffect* TargetEI )
{
	TArray<UObject*> rv;
	auto EffectorAtt = Sequence->GetComponentByClass<UDMSAttributeComponent>()->GetTypedAttributeValue<UDMSAttributeValue_Effector>(DataTag);
	if ( EffectorAtt )
	{
		for(auto& i : EffectorAtt->GetValue()) rv.Add(i.GetObject());
	}
	return rv;

}

TArray<UObject*> USelectorRequestGenerator_FromEIData::GenerateCandidates ( ADMSSequence* Sequence , ADMSActiveEffect* TargetEI )
{
	TArray<UObject*> rv;
	auto EffectorAtt = TargetEI->GetComponentByClass<UDMSAttributeComponent>()->GetTypedAttributeValue<UDMSAttributeValue_Effector>(DataTag);
	if ( EffectorAtt )
	{
		for(auto& i : EffectorAtt->GetValue()) rv.Add(i.GetObject());
	}
	return rv;
}


TArray<UObject*> USelectorRequestGenerator_WithTG::GenerateCandidates(ADMSSequence* Sequence, ADMSActiveEffect* TargetEI)
{
	return TargetGenerator->GetTargets(Sequence->GetSourceObject(),Sequence);
}





void UDMSSelectorBehaviorDefinition_UpdateData::SetupFormDelegates(ADMSSequence* Sequence, TArray<FDMSSelectorRequestForm>& FormArr, bool SelectForEachEI)
{
	if (SelectForEachEI) // FormArr.Num() == EIArr.Num()
	{
		int FormIdx = 0;	
		TArray<ADMSActiveEffect*> EIArr = Sequence->GetAllEIs();
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

void UDMSSelectorBehaviorDefinition_UpdateTarget::SetupFormDelegates(ADMSSequence* Sequence, TArray<FDMSSelectorRequestForm>& FormArr, bool SelectForEachEI)
{
	FormArr[0].OnCompletedNative.BindLambda([=](TArray<uint8> IndexArr) {
			auto EH = UDMSCoreFunctionLibrary::GetDMSEffectHandler(Sequence); check(EH);

			TArray<TScriptInterface<IDMSEffectorInterface>> SelectedTargets;
			for (auto& i : IndexArr)
			{
				UDMSDataObject* TargetObject = FormArr[0].Candidates[i];
				if ( TargetObject->TypeCheck<UObject*>() ) {
					TScriptInterface<IDMSEffectorInterface> TargetInf = TargetObject->Get<UObject*>();
					SelectedTargets.Add(TargetInf);
				}
			}
			Sequence->SetTarget(SelectedTargets);
			EH->CreateApplyingActiveEffect(Sequence, Sequence->OriginalEffectNode);
	});
	
}
