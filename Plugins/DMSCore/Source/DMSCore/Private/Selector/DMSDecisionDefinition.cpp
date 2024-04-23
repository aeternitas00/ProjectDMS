// Fill out your copyright notice in the Description page of Project Settings.


#include "Selector/DMSDecisionDefinition.h"
#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEffectHandler.h"
#include "Sequence/DMSSequence.h"


// 리퀘스트 폼을 제작 
TArray<UDMSSelectorRequestForm*> UDMSSelectorRequestGenerator::GenerateRequestForm(ADMSSequence* Sequence, bool SelectForEachEI)
{
	TArray<UDMSSelectorRequestForm*> Rv;

	if (SelectForEachEI) {
		TArray<ADMSActiveEffect*> EIArr = Sequence->GetAllActiveEffects();
		for (auto& TargetEI : EIArr)
		{
			UDMSSelectorRequestForm* NewForm = CreateNewRequestForm(Sequence);// NewObject<UDMSSelectorRequestForm>(Sequence,RequestFormClass);
			CollectCandidates(Sequence, TargetEI, NewForm);
			NewForm->SelectAmount = SelectAmount;
			NewForm->SelectorClass = SelectorClass;
			Rv.Add(NewForm);
		}
	}
	else {
		UDMSSelectorRequestForm* NewForm = CreateNewRequestForm(Sequence);// NewObject<UDMSSelectorRequestForm>(Sequence,RequestFormClass);
		// Main AE 없을 때 source object 사용??.. 일단 TG 사용할 때 일관성 유지를 생각해봐야할듯.
		ADMSActiveEffect* MainAE = Sequence->GetAllActiveEffects().Num() == 0 ? nullptr : Sequence->GetAllActiveEffects()[0];		
		CollectCandidates(Sequence, MainAE, NewForm);
		NewForm->SelectAmount = SelectAmount;
		NewForm->SelectorClass = SelectorClass;
		Rv.Add(NewForm);
	}
	return Rv;
}


TArray<UDMSSelectorRequestForm*> UDMSDecisionDefinitionBase::CreateRequestForm(ADMSSequence* Sequence)
{
	TArray<UDMSSelectorRequestForm*> rv;

	auto GeneratorField = FindFieldChecked<FObjectProperty>(GetClass(),TEXT("Generator"));
	auto BehaviorField = FindFieldChecked<FObjectProperty>(GetClass(),TEXT("Behavior"));

	auto Generator = GeneratorField ? *(GeneratorField->ContainerPtrToValuePtr<UDMSSelectorRequestGenerator*>(this)) : nullptr;
	auto Behavior = BehaviorField ? *(BehaviorField->ContainerPtrToValuePtr<UDMSSelectorBehaviorDefinition*>(this)) : nullptr;

	if(!Generator || !Behavior) return rv;

	auto temp = SelectForEachAE && Sequence->IsTargetted();
	rv = Generator->GenerateRequestForm( Sequence , temp );
	Behavior->SetupFormDelegates( Sequence, rv, temp );

	return rv;
}
