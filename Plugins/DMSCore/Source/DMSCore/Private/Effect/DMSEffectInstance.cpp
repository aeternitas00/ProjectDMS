// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEffectHandler.h"
#include "Effect/DMSEIManagerComponent.h"
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
	EffectManagerComponent = CreateDefaultSubobject<UDMSEIManagerComponent>("EffectManagerComponent");
	AttributeComponent->SetIsReplicated(true);
	EffectManagerComponent->SetIsReplicated(true);
}


void ADMSActiveEffect::Apply(ADMSSequence* SourceSequence, const FResolveIteratingDelegate& OnApplyCompleted)
{
	//DMS_LOG_SCREEN(TEXT("%s : EI Apply [%s]"), *GetName(), *EffectNode->GenerateTagContainer().ToString());

	//if (EffectNode->ConditionedEffectDefinitions.Num() == 0) {
	if (EffectNode->EffectDefinitions.Num() == 0) {
		OnApplyCompleted.ExecuteIfBound(SourceSequence,true);
		return;
	}

	// If any of the Effects is predicted to fail, the sequence is handled as a failure.
	for (auto& CurrentDef : EffectNode->EffectDefinitions)
	{
		if (!CurrentDef->Predict(SourceSequence, this)){
			OnApplyCompleted.ExecuteIfBound(SourceSequence,false);
			return;
		}
	}

	UDMSEffectApplyWorker* NewWorker = NewObject<UDMSEffectApplyWorker>(this);
	ApplyWorkers.Add(NewWorker);
	NewWorker->SetupWorker(SourceSequence,this,OnApplyCompleted);
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

void ADMSActiveEffect::SetupDatas(UDMSDataObjectSet* iSet) 
{ 
	DataSet = iSet != nullptr ? iSet : NewObject<UDMSDataObjectSet>(this); 
}

void ADMSActiveEffect::InheritSequenceDatas(ADMSSequence* iSeq)
{ 
	SourcePlayer=iSeq->GetSourcePlayer(); SourceObject = iSeq->GetSourceObject(); 

	AttributeComponent->ParentComponent = iSeq->GetComponentByClass<UDMSAttributeComponent>();
}

ADMSSequence* ADMSActiveEffect::CreateApplyingSequence(AActor* SourceTweak, ADMSSequence* ChainingSequence) 
{
	auto SM = UDMSCoreFunctionLibrary::GetDMSSequenceManager(this);
	if (SM == nullptr) return nullptr;
	TArray<TScriptInterface<IDMSEffectorInterface>> Target;
	Target.Add(GetOwner());
	auto OutSeq = SM->RequestCreateSequence(SourceTweak, SourcePlayer, EffectNode, Target, true, ChainingSequence);

	// 자기 자신의 효과의 노티파잉에 반응하여 무한루프 하는 것을 방지.
	OutSeq->AddToOnSequenceInitiated_Native([this](){CurrentState |= EDMSAEState::AES_NotifyClosed;});
	OutSeq->AddToOnSequenceFinished_Native([this](bool){CurrentState &= ~EDMSAEState::AES_NotifyClosed;});

	return OutSeq;
}

bool ADMSActiveEffect::ReceiveNotify(TMultiMap<TScriptInterface<IDMSEffectorInterface>, ADMSActiveEffect*>& outResponsedObjects, 
	bool iChainable, ADMSSequence* Seq, AActor* SourceTweak)
{
	// 1. Ignore condition check
	if ( EffectNode->bIgnoreNotify || (CurrentState & EDMSAEState::AES_Persistent) != EDMSAEState::AES_Persistent ) return false;

	// 2. If the user cannot decide whether to chain and the effect is not a forced trigger, then pass.
	if ( !iChainable && !EffectNode->bForced ) return false;

	// 3. Check whether the state is explicitly closed to notifying.
	if ( (CurrentState & EDMSAEState::AES_NotifyClosed) == EDMSAEState::AES_NotifyClosed)  return false;

	// FURTHER CHECKS
	SourceTweak = SourceTweak==nullptr? GetOwner() : SourceTweak;
	// Terminate condition check
	if ( EffectNode->TerminateConditions && EffectNode->TerminateConditions->CheckCondition(SourceTweak, Seq) ){
		CurrentState &= ~EDMSAEState::AES_Persistent;
	}
	// Activate condition check
	if ( EffectNode->Conditions->CheckCondition(SourceTweak, Seq) )
	{
		//DMS_LOG_SCREEN(TEXT("%s -> %s : Notify Checked"), GetTypedOuter<AActor>() != nullptr ? *GetTypedOuter<AActor>()->GetName() : TEXT("NullOuter"), *GetName());
		outResponsedObjects.Add(SourceTweak, this);
		return true;	
	}	
	return false;
}

void ADMSActiveEffect::DetachFromOwner()
{
	auto Comp = GetOwner()->GetComponentByClass<UDMSEIManagerComponent>();
	if(Comp==nullptr){/* Impossible case */return;}
	OnDetach.Broadcast();
	Comp->DetachActiveEffect(this);
}

void ADMSActiveEffect::CleanupWorker(UDMSEffectApplyWorker* Worker)
{
	DMS_LOG_C(Warning);
	ApplyWorkers.Remove(Worker);
	Worker->MarkAsGarbage();
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

void UDMSEffectApplyWorker::SetupWorker(ADMSSequence* iSequence, ADMSActiveEffect* iEI,const FOnApplyCompleted& iOnApplyCompleted)
{
	SourceSequence = iSequence;
	OwnerInstance = iEI;
	ApplyingEffect = iEI->EffectNode;
	//ApplyingEffectDefinitions = iEDs;
	CurrentEDIndex = 0;
	CompletedDelegate = iOnApplyCompleted;
	IteratingDelegate.BindDynamic(this, &UDMSEffectApplyWorker::ApplyNextEffectDef);
}

void UDMSEffectApplyWorker::ApplyNextEffectDef(bool PrevSucceeded)
{
	if (!PrevSucceeded)
	{
		DMS_LOG_SIMPLE(TEXT("==== %s : ON Apply CompletedDelegate ===="),*SourceSequence->GetName());
		// DISCUSSION :: Stopping immediately when failed is FINE?
		OwnerInstance->OnApplyComplete();
		//OwnerInstance->CleanupWorker(this);
		CompletedDelegate.ExecuteIfBound(SourceSequence, false);
		DMS_LOG_SIMPLE(TEXT("==== %s : ON Apply CompletedDelegate closed ===="),*SourceSequence->GetName());
		return;
	}

	//if (CurrentEDIndex == ApplyingEffectDefinitions.Num()) {
	if (CurrentEDIndex == ApplyingEffect->EffectDefinitions.Num()) {
		DMS_LOG_SIMPLE(TEXT("==== %s : ON Apply CompletedDelegate ===="),*SourceSequence->GetName());
		// 여기서 AEState를 원상복구 할시 During notify부터 부착된 이펙트들이 다시 노티파이 콜을 받을 수 있는 상태가 되므로 조정이 필요함.
		OwnerInstance->OnApplyComplete();
		OwnerInstance->CleanupWorker(this);
		CompletedDelegate.ExecuteIfBound(SourceSequence, true);
		DMS_LOG_SIMPLE(TEXT("==== %s : ON Apply CompletedDelegate closed ===="),*SourceSequence->GetName());
	}
	else {
		//CurrentDef = ApplyingEffectDefinitions[CurrentEDIndex++];
		CurrentDef = ApplyingEffect->EffectDefinitions[CurrentEDIndex++];
		FString DebugStr = CurrentDef->GetEffectTags().ToString();
		DMS_LOG_SIMPLE(TEXT("==== %s : ApplyNextEffectDef [%s] ===="),*SourceSequence->GetName(),*DebugStr);

		// ====================== //
		//    Effect Canceling    //
		// ====================== // 
		// Have to think about more complicated situations.

		// Check CurrentDef which is a part of EI's effect has to be ignored.
		//FGameplayTagQuery Query;
		//if (SourceSequence->SequenceDatas->ContainData(TAG_DMS_Effect_IgnoreEffect) &&
		//	SourceSequence->SequenceDatas->GetData(TAG_DMS_Effect_IgnoreEffect)->TypeCheck<FGameplayTagQuery>())
		//	Query = SourceSequence->SequenceDatas->GetData(TAG_DMS_Effect_IgnoreEffect)->Get<FGameplayTagQuery>();

		//if (Query.IsEmpty() || !Query.Matches(FGameplayTagContainer(CurrentDef->GetEffectTags()))) {
		//	
			//// Predict check moved to before than setupworkers
			//ExecutedOptionNum = 0;
			//EffectOptionCompleted.Unbind();
			//EffectOptionCompleted.BindDynamic(this, &UDMSEffectApplyWorker::OnEffectOptionCompleted);
			//CurrentDef->ExecuteEffectOptions(SourceSequence, OwnerInstance, EffectOptionCompleted);
		//}

		//else {

		//	// Ignored effect is considered to Succeeded.
		//	ApplyNextEffectDef(true);
		//}

		ExecutedOptionNum = 0;
		EffectOptionCompleted.Unbind();
		EffectOptionCompleted.BindDynamic(this, &UDMSEffectApplyWorker::OnEffectOptionCompleted);
		CurrentDef->ExecuteEffectOptions(SourceSequence, OwnerInstance, EffectOptionCompleted);

		DMS_LOG_SIMPLE(TEXT("==== %s : ApplyNextEffectDef closed [%s] ===="),*SourceSequence->GetName(),*DebugStr);
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
