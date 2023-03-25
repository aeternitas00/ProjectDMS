// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEffectHandler.h"

#include "Sequence/DMSSeqManager.h"
#include "Sequence/DMSSequence.h"

#include "GameModes/DMSGameMode.h"
#include "EffectSet/DMSEffect_ActivateEffect.h"
#include "Card/DMSCardBase.h"
#include "Library/DMSCoreFunctionLibrary.h"


void UDMSEffectInstance::Apply(UDMSSequence* SourceSequence, const FResolveIteratingDelegate& OnApplyCompleted)
{
	DMS_LOG_SCREEN(TEXT("%s : EI Apply [%s]"), *GetName(), EffectNode->NodeTag == FGameplayTag::EmptyTag ? *EffectNode->EffectDefinitions[0]->EffectTag.ToString() : *EffectNode->NodeTag.ToString());
	//for (auto EffectDefinition : EffectNode->EffectDefinitions )
	//{
	//	EffectDefinition->Work(this);
	//}

	if (EffectNode->EffectDefinitions.Num() == 0) {
		OnApplyCompleted.ExecuteIfBound(SourceSequence);
		return;
	}

	OnApplyCompletedMap.Add(SourceSequence);
	OnApplyCompletedMap[SourceSequence].CompletedDelegate=OnApplyCompleted;
	OnApplyCompletedMap[SourceSequence].Index = 0;
	OnApplyCompletedMap[SourceSequence].IteratingDelegate.BindDynamic(this,&UDMSEffectInstance::ApplyNextEffectDefinition);

	ApplyNextEffectDefinition(SourceSequence);
}

void UDMSEffectInstance::ApplyNextEffectDefinition(UDMSSequence* SourceSequence) 
{
	if (OnApplyCompletedMap[SourceSequence].Index == EffectNode->EffectDefinitions.Num())
		OnApplyCompletedMap[SourceSequence].CompletedDelegate.ExecuteIfBound(SourceSequence);
	else
		EffectNode->EffectDefinitions[OnApplyCompletedMap[SourceSequence].Index++]->Work(SourceSequence,this, OnApplyCompletedMap[SourceSequence].IteratingDelegate);
}

void UDMSEffectInstance::Initialize(UDMSEffectNode* iNode, UDMSDataObjectSet* iSet) { EffectNode = iNode; CurrentState = EDMSEIState::EIS_Pending; DataSet = iSet != nullptr ? iSet : NewObject<UDMSDataObjectSet>(); }

void UDMSEffectInstance::Initialize(UDMSEffectNode* iNode, UDMSSequence* iSeq) { EffectNode = iNode; SourceController=iSeq->SourceController; SourceObject = iSeq->SourceObject; DataSet = iSeq->EIDatas; CurrentState = EDMSEIState::EIS_Pending; }

UDMSSequence* UDMSEffectInstance::CreateSequenceFromNode(UObject* SourceTweak, UDMSSequence* ChainingSequence) 
{
	auto SM = UDMSCoreFunctionLibrary::GetDMSSequenceManager();
	if (SM == nullptr) return nullptr;
	return SM->RequestCreateSequence(SourceTweak, SourceController, EffectNode, {}, DataSet, ChainingSequence);
}

void UDMSEffectInstance::AttachEffectInstance(UDMSEffectInstance* EI)
{
	SubEI.Add(EI);
	EI->Rename(nullptr,this);
}

bool UDMSEffectInstance::OnNotifyReceived(TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*>& outResponsedObjects, 
	bool iChainable, UDMSSequence* Seq, UObject* SourceTweak)
{
	bool rv=false;
	if (!iChainable && !EffectNode->bForced) return rv;

	//DMS_LOG_SCREEN(TEXT("%s : OnNotifyReceived"), *GetName());

	if (CurrentState == EDMSEIState::EIS_Pending) {
		//DMS_LOG_SCREEN(TEXT("%s : Pending"), *GetName());
		return rv;
	}

	if ( EffectNode->Conditions->CheckCondition(SourceTweak, Seq) )
	{
		//DMS_LOG_SCREEN(TEXT("%s -> %s : Notify Checked"), GetOuter()->GetOuter() !=nullptr ? *GetOuter()->GetOuter()->GetName():TEXT("NullOuter"), *GetName());
		DMS_LOG_SCREEN(TEXT("%s -> %s : Notify Checked"), GetTypedOuter<AActor>() != nullptr ? *GetTypedOuter<AActor>()->GetName() : TEXT("NullOuter"), *GetName());
		
		rv=true;
		if ( !EffectNode->bForced ) outResponsedObjects.Add(SourceTweak, this);

		// Inherit dataset is correct?
		else {
			UDMSCoreFunctionLibrary::GetDMSSequenceManager()->RunSequence(CreateSequenceFromNode(SourceTweak, Seq));
		}
	}
	else
	{
		rv=false;
		//DMS_LOG_SCREEN(TEXT("%s : OnNotifyReceived -> Notify Passed"), *GetName());
	}
	return rv;
}


//UDMSEffectSet* UDMSEffectInstance::GetOwningEffectSet()
//{
//	//
//
//	return nullptr;
//}
