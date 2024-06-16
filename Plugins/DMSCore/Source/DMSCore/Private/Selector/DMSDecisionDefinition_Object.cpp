// Copyright JeongWoo Lee


#include "Selector/DMSDecisionDefinition_Object.h"
#include "Effect/DMSEffectInstance.h"
#include "Common/DMSTargetGenerator.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Attribute/DMSAttributeValue_Object.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Effect/DMSEffectHandler.h"
#include "Sequence/DMSSequence.h"

UDMSSelectorRequestForm* USelReqGenerator_ObjCand::CreateNewRequestForm(ADMSSequence* Sequence)
{ return NewObject<UDMSSelectorRequestForm_Object>(Sequence); }

void USelReqGenerator_ObjCand::CollectCandidates(ADMSSequence* Sequence, ADMSActiveEffect* TargetEI, UDMSSelectorRequestForm* CollectingRequestForm)
{
	auto CForm = Cast<UDMSSelectorRequestForm_Object>(CollectingRequestForm);
	if(!CForm) return;
	CForm->Candidates = CollectObjects(Sequence, TargetEI);
}

TArray<UObject*> USelReqGenerator_ObjCand_FromAtt::CollectObjects(ADMSSequence* Sequence, ADMSActiveEffect* TargetEI)
{
	TArray<UObject*> rv;
	auto lAttributeOwners = AttributeOwners->GetTargets(TargetEI,Sequence);

	for(auto& Owner : lAttributeOwners)
	{
		auto AOwner = Cast<AActor>(Owner);
		auto OwnerComp = AOwner ? AOwner->GetComponentByClass<UDMSAttributeComponent>() : nullptr;
		TArray<UDMSAttributeValue_Object*> ObjAtts = OwnerComp ? OwnerComp->GetTypedAttributeValuesByQuery<UDMSAttributeValue_Object>(AttributeTagQuery) : TArray<UDMSAttributeValue_Object*>();
		for(auto& ObjAtt : ObjAtts) rv.Append(ObjAtt->GetValue());
	}

	return rv;
}

TArray<UObject*> USelReqGenerator_ObjCand_WithTG::CollectObjects(ADMSSequence* Sequence, ADMSActiveEffect* TargetEI)
{
	return Candidates->GetTargets(TargetEI,Sequence);
}

void UDMSSelBehavDefinition_Object_UpdateTarget::SetupFormDelegates(ADMSSequence* Sequence, TArray<UDMSSelectorRequestForm*>& FormArr, bool SelectForEachEI)
{
	auto Form = Cast<UDMSSelectorRequestForm_Object>(FormArr[0]);
	Form->OnCompletedNative.BindLambda([=](TArray<uint8> IndexArr) {
		auto EH = UDMSCoreFunctionLibrary::GetDMSEffectHandler(Sequence); check(EH);
		TArray<TScriptInterface<IDMSEffectorInterface>> SelectedTargets;
		for (auto& i : IndexArr)
		{
			SelectedTargets.Add(Form->Candidates[i]);
		}
		Sequence->SetTarget(SelectedTargets);
		EH->CreateApplyingActiveEffect(Sequence, Sequence->OriginalEffectNode);
	});
	
}

void UDMSSelBehavDefinition_Object_UpdateSeqAtt::SetupFormDelegates(ADMSSequence* Sequence, TArray<UDMSSelectorRequestForm*>& FormArr, bool SelectForEachEI)
{
	if (SelectForEachEI) // FormArr.Num() == AEArr.Num()
	{
		int FormIdx = 0;	
		TArray<ADMSActiveEffect*> AEArr = Sequence->GetAllActiveEffects();
		for (auto rawForm : FormArr)
		{
			auto Form = Cast<UDMSSelectorRequestForm_Object>(rawForm);
			// 이거 순서가 확실히 유지되나?
			Form->OnCompletedNative.BindLambda([=,AE = AEArr[FormIdx]](TArray<uint8> IndexArr)
			{
				DMS_LOG_SIMPLE(TEXT("==== %s : SelectorBehavior : Update Data ===="), *AE->GetName());
	
				TArray<UObject*> SelectedItems;
				for (auto& i : IndexArr)
				{
					SelectedItems.Add(Form->Candidates[i]);
				}
				auto Att = AE->GetComponentByClass<UDMSAttributeComponent>()->MakeAttribute(AttributeTags,UDMSAttributeValue_Object::StaticClass());				
				auto AttValue = Cast<UDMSAttributeValue_Object>(Att->AttributeValue);
				if(!AttValue) return; 
				AttValue->SetValue(SelectedItems);
			});
			FormIdx++;
		}
	
	}
	else // FormArr.Num() == '1'
	{
		auto Form = Cast<UDMSSelectorRequestForm_Object>(FormArr[0]);
		Form->OnCompletedNative.BindLambda([=](TArray<uint8> IndexArr)
		{
			TArray<UObject*> SelectedItems;
			for (auto& i : IndexArr)
			{
				SelectedItems.Add(Form->Candidates[i]);
			}
			auto Att = Sequence->GetComponentByClass<UDMSAttributeComponent>()->MakeAttribute(AttributeTags,UDMSAttributeValue_Object::StaticClass());
			auto AttValue = Cast<UDMSAttributeValue_Object>(Att->AttributeValue);
			if(!AttValue) return; 
			AttValue->SetValue(SelectedItems);
		});
	}
}

