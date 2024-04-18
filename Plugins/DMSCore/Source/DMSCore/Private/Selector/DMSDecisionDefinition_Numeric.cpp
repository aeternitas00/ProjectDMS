// Fill out your copyright notice in the Description page of Project Settings.


#include "Selector/DMSDecisionDefinition_Numeric.h"
#include "Effect/DMSEffectInstance.h"
#include "Common/DMSTargetGenerator.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Attribute/DMSAttributeValue_Numeric.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Effect/DMSEffectHandler.h"
#include "Sequence/DMSSequence.h"

UDMSSelectorRequestForm* USelReqGenerator_NumericCand::CreateNewRequestForm(ADMSSequence* Sequence)
{ return NewObject<UDMSSelectorRequestForm_Numeric>(Sequence); }

void USelReqGenerator_NumericCand::CollectCandidates(ADMSSequence* Sequence, ADMSActiveEffect* TargetEI, UDMSSelectorRequestForm* CollectingRequestForm)
{
	auto CForm = Cast<UDMSSelectorRequestForm_Numeric>(CollectingRequestForm);
	if(!CForm) return;
	CForm->Candidates = CollectNumbers(Sequence, TargetEI);
}


TArray<float> USelReqGenerator_NumericCand_FromAtt::CollectNumbers(ADMSSequence* Sequence, ADMSActiveEffect* TargetEI)
{
	TArray<float> rv;

	auto lAttributeOwners = AttributeOwners->GetTargets(TargetEI,Sequence);

	for(auto& Owner : lAttributeOwners)
	{
		auto AOwner = Cast<AActor>(Owner);
		auto OwnerComp = AOwner ? AOwner->GetComponentByClass<UDMSAttributeComponent>() : nullptr;
		TArray<UDMSAttributeValue_Numeric*> NumAtts = OwnerComp ? OwnerComp->GetTypedAttributeValuesByQuery<UDMSAttributeValue_Numeric>(AttributeTagQuery) : TArray<UDMSAttributeValue_Numeric*>();
		for(auto& NumAtt : NumAtts) rv.Add(NumAtt->GetValue());
	}

	return rv;
}

TArray<float> USelReqGenerator_NumericCand_Ranged::CollectNumbers(ADMSSequence* Sequence, ADMSActiveEffect* TargetEI)
{
	TArray<float> rv;
	float Left,Right;
	auto MinActor = MinAttributeOwner && MinAttributeOwner->GetTargets(TargetEI,Sequence).Num()!=0 ? Cast<AActor>(MinAttributeOwner->GetTargets(TargetEI,Sequence)[0]) : nullptr;
	auto MinAttComp = MinActor ? MinActor->GetComponentByClass<UDMSAttributeComponent>() : nullptr;
	auto MinAtt = MinAttComp ? MinAttComp->GetTypedAttributeValue<UDMSAttributeValue_Numeric>(MinAttributeTag) : nullptr;
	Left = MinAtt ? MinAtt->GetValue(): MinDefaultValue;

	auto MaxActor = MaxAttributeOwner && MaxAttributeOwner->GetTargets(TargetEI,Sequence).Num()!=0 ? Cast<AActor>(MaxAttributeOwner->GetTargets(TargetEI,Sequence)[0]) : nullptr;
	auto MaxAttComp = MaxActor ? MaxActor->GetComponentByClass<UDMSAttributeComponent>() : nullptr;
	auto MaxAtt = MaxAttComp ? MaxAttComp->GetTypedAttributeValue<UDMSAttributeValue_Numeric>(MaxAttributeTag) : nullptr;

	Right = MaxAtt ? MaxAtt->GetValue(): MaxDefaultValue;
	if(Left>Right)Swap(Left,Right);
	for(int i=Left;Left<=Right;i++)
	{
		rv.Add(i);
	}
	return rv;
}

void UDMSSelBehavDefinition_Number_UpdateSeqAtt::SetupFormDelegates(ADMSSequence* Sequence, TArray<UDMSSelectorRequestForm*>& FormArr, bool SelectForEachEI)
{
	if (SelectForEachEI) // FormArr.Num() == AEArr.Num()
	{
		int FormIdx = 0;	
		TArray<ADMSActiveEffect*> AEArr = Sequence->GetAllActiveEffects();
		for (auto rawForm : FormArr)
		{
			auto Form = Cast<UDMSSelectorRequestForm_Numeric>(rawForm);
			// 이거 순서가 확실히 유지되나?
			Form->OnCompletedNative.BindLambda([=,AE = AEArr[FormIdx]](TArray<uint8> IndexArr)
			{
				DMS_LOG_SIMPLE(TEXT("==== %s : SelectorBehavior : Update Data ===="), *AE->GetName());

				float SumOfSelected=0;
				for (auto& i : IndexArr)
				{
					SumOfSelected+=Form->Candidates[i];
				}
				auto Att = AE->GetComponentByClass<UDMSAttributeComponent>()->MakeAttribute(AttributeTags,UDMSAttributeValue_Numeric::StaticClass());				
				auto AttValue = Cast<UDMSAttributeValue_Numeric>(Att->AttributeValue);
				if(!AttValue) return; 
				AttValue->SetValue(SumOfSelected);
			});
			FormIdx++;
		}

	}
	else // FormArr.Num() == '1'
	{
		auto Form = Cast<UDMSSelectorRequestForm_Numeric>(FormArr[0]);
		Form->OnCompletedNative.BindLambda([=](TArray<uint8> IndexArr)
		{
			float SumOfSelected = 0;
			for (auto& i : IndexArr)
			{
				SumOfSelected+=Form->Candidates[i];
			}
			auto Att = Sequence->GetComponentByClass<UDMSAttributeComponent>()->MakeAttribute(AttributeTags,UDMSAttributeValue_Numeric::StaticClass());
			auto AttValue = Cast<UDMSAttributeValue_Numeric>(Att->AttributeValue);
			if(!AttValue) return; 
			AttValue->SetValue(SumOfSelected);
		});
	}
}

