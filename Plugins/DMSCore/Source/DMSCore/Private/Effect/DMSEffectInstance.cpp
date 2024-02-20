// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEffectHandler.h"
//#include "Effect/DMSEffectDefinition.h"

#include "Sequence/DMSSeqManager.h"
#include "Sequence/DMSSequence.h"

#include "Conditions/DMSConditionObject.h"

#include "GameModes/DMSGameStateBase.h"
#include "GameModes/DMSGameModeBase.h"
#include "Attribute/DMSAttributeComponent.h"

#include "Effect/DMSEffect_CancelEffect.h"
#include "Effect/DMSEffectOption.h"
//#include "EffectSet/DMSEffect_ActivateEffect.h"
//#include "Card/DMSCardBase.h"
#include "Library/DMSCoreFunctionLibrary.h"

ADMSActiveEffect::ADMSActiveEffect() :CurrentState(EDMSAEState::AES_None),DataSet(nullptr)
{ 
	bReplicates = true;
	AttributeComponent = CreateDefaultSubobject<UDMSAttributeComponent>("AttributesComponent");
	AttributeComponent->SetIsReplicated(true);
}


void ADMSActiveEffect::Apply(UDMSSequence* SourceSequence, const FResolveIteratingDelegate& OnApplyCompleted)
{
	//DMS_LOG_SCREEN(TEXT("%s : EI Apply [%s]"), *GetName(), *EffectNode->GenerateTagContainer().ToString());

	if (EffectNode->EffectDefinitions.Num() == 0) {
		OnApplyCompleted.ExecuteIfBound(SourceSequence,true);
		return;
	}

	UDMSEffectApplyWorker* NewWorker = NewObject<UDMSEffectApplyWorker>(this);
	ApplyWorkers.Add(NewWorker);
	NewWorker->SetupWorker(SourceSequence,this, OnApplyCompleted);
	NewWorker->ApplyNextEffectDef(true);

}

IDMSEffectorInterface* ADMSActiveEffect::GetApplyTargetInterface()
{
	auto Outer = Cast<IDMSEffectorInterface>(GetOwner());

	return Outer;
}

AActor* ADMSActiveEffect::GetApplyTarget()
{
	auto Target = GetApplyTargetInterface();
	if (Target==nullptr) return nullptr;
	return Target->GetObject();
}

void ADMSActiveEffect::OnApplyComplete()
{
	OnApplyComplete_Native.Broadcast();
	OnApplyComplete_Native.Clear();
}

void ADMSActiveEffect::Initialize(UDMSEffectNode* iNode,const EDMSAEState& InitialState)
{
	EffectNode = iNode; 
	for (auto& Attribute : EffectNode->EffectAttributes)
		AttributeComponent->GenerateAndSetAttribute(Attribute.DefaultTag,Attribute.DefaultValue);
	CurrentState = InitialState;
}

//void ADMSActiveEffect_Applying::OnApplyComplete()
//{
//	Super::OnApplyComplete();
//}
//
//void ADMSActiveEffect_Persistent::Initialize(UDMSEffectNode* iNode)
//{
//	Super::Initialize(iNode);
//	CurrentState |= EDMSAEState::AES_Persistent;
//}
//
//void ADMSActiveEffect_Persistent::OnApplyComplete()
//{
//	CurrentState &= ~EDMSAEState::AES_NotifyClosed;
//
//	Super::OnApplyComplete();
//}

void ADMSActiveEffect::SetupDatas(UDMSDataObjectSet* iSet) 
{ 
	DataSet = iSet != nullptr ? iSet : NewObject<UDMSDataObjectSet>(this); 
}

void ADMSActiveEffect::InheritSequenceDatas(UDMSSequence* iSeq)
{ 
	SourcePlayer=iSeq->GetSourcePlayer(); SourceObject = iSeq->GetSourceObject(); 

	if (DataSet==nullptr) DataSet = NewObject<UDMSDataObjectSet>(this);

	DataSet->Inherit(iSeq->SequenceDatas);
	DataSet->ParentDataSet = iSeq->SequenceDatas;
}


UDMSSequence* ADMSActiveEffect::CreateApplyingSequence(AActor* SourceTweak, UDMSSequence* ChainingSequence) 
{
	auto SM = UDMSCoreFunctionLibrary::GetDMSSequenceManager();
	if (SM == nullptr) return nullptr;
	TArray<TScriptInterface<IDMSEffectorInterface>> Target;
	Target.Add(GetOwner());
	auto OutSeq = SM->RequestCreateSequence(SourceTweak, SourcePlayer, EffectNode, Target, DataSet, ChainingSequence);

	// 자기 자신의 효과의 노티파잉에 반응하여 무한루프 하는 것을 방지.
	OutSeq->AddToOnSequenceInitiated_Native([this](){CurrentState |= EDMSAEState::AES_NotifyClosed;});
	OutSeq->AddToOnSequenceFinished_Native([this](bool){CurrentState &= ~EDMSAEState::AES_NotifyClosed;});

	return OutSeq;
}

bool ADMSActiveEffect::OnNotifyReceived(TMultiMap<TScriptInterface<IDMSEffectorInterface>, ADMSActiveEffect*>& outResponsedObjects, 
	bool iChainable, UDMSSequence* Seq, AActor* SourceTweak)
{
	// 1. Ignore condition check
	if ( EffectNode->bIgnoreNotify || (CurrentState & EDMSAEState::AES_Persistent) != EDMSAEState::AES_Persistent ) return false;

	// 2. If the user cannot decide whether to chain and the effect is not a forced trigger, then pass.
	if ( !iChainable && !EffectNode->bForced ) return false;

	// 3. Check whether the state is explicitly closed to notifying.
	if ( (CurrentState & EDMSAEState::AES_NotifyClosed) == EDMSAEState::AES_NotifyClosed)  return false;

	// NOTIFIED

	// Terminate condition check
	if ( EffectNode->TerminateConditions && EffectNode->TerminateConditions->CheckCondition(SourceTweak, Seq) )
		CurrentState &= ~EDMSAEState::AES_Persistent;

	// Activate condition check
	if ( EffectNode->Conditions->CheckCondition(SourceTweak, Seq) )
	{
		//DMS_LOG_SCREEN(TEXT("%s -> %s : Notify Checked"), GetTypedOuter<AActor>() != nullptr ? *GetTypedOuter<AActor>()->GetName() : TEXT("NullOuter"), *GetName());
		outResponsedObjects.Add(SourceTweak, this);
		return true;	
	}	
	return false;
}

void ADMSActiveEffect::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADMSActiveEffect, CurrentState);
	DOREPLIFETIME(ADMSActiveEffect, EffectManagerComponent);
	DOREPLIFETIME(ADMSActiveEffect, AttributeComponent);

}


void ADMSActiveEffect::Serialize(FArchive& Ar)
{
	//DMS_LOG_SCREEN(TEXT("ADMSActiveEffect Serialized"));
	Super::Serialize(Ar);

	if (Ar.IsSaving())
	{
		// UDMSEffectNode* 멤버 변수를 직렬화하고 저장
		if (EffectNode)
		{
			UClass* NodeClass = EffectNode->GetClass();
			Ar << NodeClass;
			EffectNode->Serialize(Ar);
		}
		else
		{
			UClass* NullClass = nullptr;
			Ar << NullClass;
		}
	}
	else if (Ar.IsLoading())
	{
		// UDMSEffectNode* 멤버 변수를 로드하고 역직렬화하여 새로운 인스턴스를 생성

		UClass* NodeClass;
		Ar << NodeClass;

		if (NodeClass!=nullptr){
			EffectNode = NewObject<UDMSEffectNode>(this, NodeClass);
			EffectNode->Serialize(Ar);
		}
	}

}

void UDMSEffectApplyWorker::SetupWorker(UDMSSequence* iSequence, ADMSActiveEffect* iEI, const FOnApplyCompleted& iOnApplyCompleted)
{
	SourceSequence = iSequence;
	OwnerInstance = iEI;
	ApplyingEffect = iEI->EffectNode;
	CurrentEDIndex = 0;
	CompletedDelegate = iOnApplyCompleted;
	IteratingDelegate.BindDynamic(this, &UDMSEffectApplyWorker::ApplyNextEffectDef);
}

void UDMSEffectApplyWorker::ApplyNextEffectDef(bool PrevSucceeded)
{
	if (!PrevSucceeded)
	{
		// DISCUSSION :: Stopping immediately when failed is FINE?
		OwnerInstance->OnApplyComplete();
		CompletedDelegate.ExecuteIfBound(SourceSequence, false);
		return;
	}

	if (CurrentEDIndex == ApplyingEffect->EffectDefinitions.Num()) {
		// 여기서 AEState를 원상복구 할시 During notify부터 부착된 이펙트들이 다시 노티파이 콜을 받을 수 있는 상태가 되므로 조정이 필요함.
		OwnerInstance->OnApplyComplete();
		CompletedDelegate.ExecuteIfBound(SourceSequence, true);
	}
	else {
		CurrentDef = ApplyingEffect->EffectDefinitions[CurrentEDIndex++];

		// ====================== //
		//    Effect Canceling    //
		// ====================== // 
		// Have to think about more complicated situations.

		// Check CurrentDef which is a part of EI's effect has to be ignored.
		FGameplayTagQuery Query;
		if (SourceSequence->SequenceDatas->ContainData(TAG_DMS_Effect_IgnoreEffect) &&
			SourceSequence->SequenceDatas->GetData(TAG_DMS_Effect_IgnoreEffect)->TypeCheck<FGameplayTagQuery>())
			Query = SourceSequence->SequenceDatas->GetData(TAG_DMS_Effect_IgnoreEffect)->Get<FGameplayTagQuery>();

		if (Query.IsEmpty() || !Query.Matches(FGameplayTagContainer(CurrentDef->GetEffectTags()))) {
			
			// Predict first
			if (CurrentDef->Predict(SourceSequence, OwnerInstance)){

				ExecutedOptionNum = 0;
				EffectOptionCompleted.Unbind();
				EffectOptionCompleted.BindDynamic(this, &UDMSEffectApplyWorker::OnEffectOptionCompleted);
				CurrentDef->ExecuteEffectOptions(SourceSequence, OwnerInstance, EffectOptionCompleted);
				
			}
		// Going back if it'll be failed ( DISCUSSION :: OPTIONAL FAIL BACK? )
			else
				ApplyNextEffectDef(false);
		}

		else {

			// Ignored effect is considered to Succeeded.
			ApplyNextEffectDef(true);
		}
	}
}

void UDMSEffectApplyWorker::OnEffectOptionCompleted(UDMSEffectOption* CompletedOption)
{
	if (CompletedOption!=nullptr && CompletedOption->NextOption != nullptr)
	{
		CompletedOption->NextOption->ExecuteOption(SourceSequence, OwnerInstance, EffectOptionCompleted);
		return;
	}
	if (CurrentDef->EffectOptions.Num() <= ++ExecutedOptionNum)
	{
		CurrentDef->ExecuteEffectDefinition(SourceSequence, OwnerInstance, IteratingDelegate);
	}
}
