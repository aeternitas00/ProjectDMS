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

ADMSActiveEffect::ADMSActiveEffect() :CurrentState(EDMSEIState::EIS_Default) 
{ 
	bReplicates = true;
	AttributeComponent = CreateDefaultSubobject<UDMSAttributeComponent>("AttributesComponent");
	AttributeComponent->SetIsReplicated(true);

	//IteratingDelegate.BindDynamic(this, &ADMSActiveEffect::ApplyNextEffectDefinition); 
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
	//if (CurrentState == EDMSEIState::EIS_Preview)
	//	return Outer==nullptr ? nullptr : Cast<ADMSActiveEffect>(Outer)->GetApplyTargetInterface();
	//else 	return Outer;
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
	//if (CurrentState == EDMSEIState::EIS_Preview ){ if(GetTypedOuter<ADMSActiveEffect>()->CurrentState != EDMSEIState::EIS_Persistent) CurrentState = EDMSEIState::EIS_PendingKill; }
	//else if (CurrentState != EDMSEIState::EIS_Persistent) CurrentState = EDMSEIState::EIS_PendingKill;

	if (CurrentState != EDMSEIState::EIS_Persistent) CurrentState = EDMSEIState::EIS_PendingKill;
}

void ADMSActiveEffect::Initialize(UDMSEffectNode* iNode, UDMSDataObjectSet* iSet) 
{ 
	EffectNode = iNode; 
	for (auto& Attribute : EffectNode->EffectAttributes)
		AttributeComponent->GenerateAndSetAttribute(Attribute.DefaultTag,Attribute.DefaultValue);
	
	CurrentState = iNode->bIsPersistent ? EDMSEIState::EIS_Persistent : EDMSEIState::EIS_PendingApply; 
	DataSet = iSet != nullptr ? iSet : NewObject<UDMSDataObjectSet>(); 
	
	//SetupPreviewDummy();
}

void ADMSActiveEffect::Initialize(UDMSEffectNode* iNode, UDMSSequence* iSeq)
{ 
	EffectNode = iNode; 
	for (auto& Attribute : EffectNode->EffectAttributes)
		AttributeComponent->GenerateAndSetAttribute(Attribute.DefaultTag,Attribute.DefaultValue);

	SourcePlayer=iSeq->GetSourcePlayer(); SourceObject = iSeq->GetSourceObject(); 

	UDMSDataObjectSet* NewData = NewObject<UDMSDataObjectSet>(this);
	NewData->Inherit(iSeq->SequenceDatas);
	NewData->ParentDataSet = iSeq->SequenceDatas;
	DataSet = NewData;
	CurrentState = iNode->bIsPersistent ? EDMSEIState::EIS_Persistent : EDMSEIState::EIS_PendingApply; 


}


UDMSSequence* ADMSActiveEffect::CreateSequenceFromNode(AActor* SourceTweak, UDMSSequence* ChainingSequence) 
{
	auto SM = UDMSCoreFunctionLibrary::GetDMSSequenceManager();
	if (SM == nullptr) return nullptr;
	TArray<TScriptInterface<IDMSEffectorInterface>> Target;
	Target.Add(GetOwner());
	return SM->RequestCreateSequence(SourceTweak, SourcePlayer, EffectNode, Target, DataSet, ChainingSequence);
}

//void ADMSActiveEffect::AttachEffectInstance(ADMSActiveEffect* EI)
//{
//	SubEI.Add(EI);
//	EI->Rename(nullptr,this);
//
//	// DO we really need to do this things?
//	//AddReplicatedSubObject(EI);
//}

bool ADMSActiveEffect::OnNotifyReceived(TMultiMap<TScriptInterface<IDMSEffectorInterface>, ADMSActiveEffect*>& outResponsedObjects, 
	bool iChainable, UDMSSequence* Seq, AActor* SourceTweak)
{
	//if (Seq->OriginalEffectNode == EffectNode) {
	//	DMS_LOG_SIMPLE(TEXT("Recursive Response Occured"));
	//	return rv;
	//}

	if ( EffectNode->bIgnoreNotify ) return false;

	if ( !iChainable && !EffectNode->bForced ) return false;

	if ( /*CurrentState != EDMSEIState::EIS_PendingApply &&*/ CurrentState != EDMSEIState::EIS_Persistent ) return false;

	if ( EffectNode->TerminateConditions && EffectNode->TerminateConditions->CheckCondition(SourceTweak, Seq) )
	{
		CurrentState = EDMSEIState::EIS_PendingKill;
		return false;	
	}

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


//UDMSEffectSet* ADMSActiveEffect::GetOwningEffectSet()
//{
//	//
//
//	return nullptr;
//}

//FArchive& operator<<(FArchive& Ar, ADMSActiveEffect*& EI)
//{
//	// TODO: 여기에 return 문을 삽입합니다.
//	EI->Serialize(Ar);
//	return Ar;
//}

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
