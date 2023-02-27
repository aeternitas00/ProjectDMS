// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/DMSEffectInstance.h"
#include "Sequence/DMSSeqManager.h"
#include "GameModes/DMSGameMode.h"
#include "EffectSet/DMSEffect_ActivateEffect.h"
#include "Effect/DMSEffectHandler.h"
#include "Sequence/DMSSequence.h"
#include "Library/DMSCoreFunctionLibrary.h"

void UDMSEffectInstance::Apply()
{
	DMS_LOG_SCREEN(TEXT("%s : EI Apply [%s]"), *GetName(), EffectNode->NodeKeyword == "" ? *EffectNode->EffectDefinitions[0]->Keyword.ToString() : *EffectNode->NodeKeyword.ToString());
	for (auto EffectDefinition : EffectNode->EffectDefinitions )
	{
		EffectDefinition->Work(this);
	}
}

void UDMSEffectInstance::Initialize(UDMSEffectNode* iNode, UDMSDataObjectSet* iSet) { EffectNode = iNode; CurrentState = EDMSEIState::EIS_Pending; DataSet = iSet != nullptr ? iSet : NewObject<UDMSDataObjectSet>(); }

void UDMSEffectInstance::Initialize(UDMSEffectNode* iNode, UDMSSequence* iSeq) { EffectNode = iNode; SourceController=iSeq->SourceController; SourceObject = iSeq->SourceObject; DataSet = iSeq->EIDatas; CurrentState = EDMSEIState::EIS_Pending; }

void UDMSEffectInstance::AttachEffectInstance(UDMSEffectInstance* EI)
{
	SubEI.Add(EI);
	EI->Rename(nullptr,this);
}

void UDMSEffectInstance::OnNotifyReceived(UDMSSequence* Seq, UObject* SourceTweak)
{
	DMS_LOG_SCREEN(TEXT("%s : OnNotifyReceived"), *GetName());

	if (CurrentState == EDMSEIState::EIS_Pending) {
		DMS_LOG_SCREEN(TEXT("%s : Pending"), *GetName());
		return;
	}

	auto SM = UDMSCoreFunctionLibrary::GetDMSSequenceManager();
	if (SM == nullptr) return;

	if ( EffectNode->Conditions.CheckCondition(SourceTweak, Seq) )
	{
		DMS_LOG_SCREEN(TEXT("%s : OnNotifyReceived -> Notify Checked"), *GetName());

		// Inherit dataset is correct?
		UDMSSequence* ParentSeq = SM->RequestCreateSequence(SourceTweak, SourceController, EffectNode, {}, DataSet, Seq, Seq->Progress);

		if (EffectNode->ChildEffect != nullptr && EffectNode->AdvanceConditions.CheckCondition(SourceTweak, Seq))
		{	
			//DMS_LOG_SCREEN(TEXT("%s : OnNotifyReceived -> Advance"), *GetName());
			SM->RequestCreateSequence(SourceTweak, SourceController, EffectNode->ChildEffect->GetEffectNode(), {}, DataSet , ParentSeq);
		}
	}
	else
	{
		//DMS_LOG_SCREEN(TEXT("%s : OnNotifyReceived -> Notify Passed"), *GetName());
	}
}


//UDMSEffectSet* UDMSEffectInstance::GetOwningEffectSet()
//{
//	//
//
//	return nullptr;
//}
