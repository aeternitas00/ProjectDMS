// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/DMSEffectInstance.h"
#include "Sequence/DMSSeqManager.h"
#include "GameModes/DMSGameMode.h"
#include "EffectSet/DMSEffect_ActivateEffect.h"
#include "Effect/DMSEffectHandler.h"
#include "Sequence/DMSSequence.h"
#include "Card/DMSCardBase.h"
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

void UDMSEffectInstance::OnNotifyReceived(bool iChainable, UDMSSequence* Seq, UObject* SourceTweak)
{
	if (!iChainable)if(!EffectNode->bForced) return;

	//DMS_LOG_SCREEN(TEXT("%s : OnNotifyReceived"), *GetName());

	if (CurrentState == EDMSEIState::EIS_Pending) {
		//DMS_LOG_SCREEN(TEXT("%s : Pending"), *GetName());
		return;
	}

	auto SM = UDMSCoreFunctionLibrary::GetDMSSequenceManager();
	if (SM == nullptr) return;

	if ( EffectNode->Conditions.CheckCondition(SourceTweak, Seq) )
	{
		//DMS_LOG_SCREEN(TEXT("%s -> %s : Notify Checked"), GetOuter()->GetOuter() !=nullptr ? *GetOuter()->GetOuter()->GetName():TEXT("NullOuter"), *GetName());
		DMS_LOG_SCREEN(TEXT("%s -> %s : Notify Checked"), GetTypedOuter<AActor>() != nullptr ? *GetTypedOuter<AActor>()->GetName() : TEXT("NullOuter"), *GetName());

		// Inherit dataset is correct?
		SM->RequestCreateSequence(SourceTweak, SourceController, EffectNode, {}, DataSet, Seq, Seq->Progress);
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
