// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEffectHandler.h"
//#include "Effect/DMSEffectDefinition.h"

#include "Sequence/DMSSeqManager.h"
#include "Sequence/DMSSequence.h"

#include "Conditions/DMSConditionObject.h"

#include "GameModes/DMSGameStateBase.h"
#include "GameModes/DMSGameModeBase.h"

#include "Effect/DMSEffect_CancelEffect.h"
#include "Effect/DMSEffectOption.h"
//#include "EffectSet/DMSEffect_ActivateEffect.h"
//#include "Card/DMSCardBase.h"
#include "Library/DMSCoreFunctionLibrary.h"

UDMSEffectInstance::UDMSEffectInstance() :CurrentState(EDMSEIState::EIS_Default) 
{ 
	//IteratingDelegate.BindDynamic(this, &UDMSEffectInstance::ApplyNextEffectDefinition); 
}


void UDMSEffectInstance::Apply(UDMSSequence* SourceSequence, const FResolveIteratingDelegate& OnApplyCompleted)
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

IDMSEffectorInterface* UDMSEffectInstance::GetApplyTarget()
{
	auto Outer = Cast<IDMSEffectorInterface>(GetOuter());
	//if (CurrentState == EDMSEIState::EIS_Preview)
	//	return Outer==nullptr ? nullptr : Cast<UDMSEffectInstance>(Outer)->GetApplyTarget();
	//else 	return Outer;
	return Outer;
}
void UDMSEffectInstance::SetToPendingKill()
{
	//if (CurrentState == EDMSEIState::EIS_Preview ){ if(GetTypedOuter<UDMSEffectInstance>()->CurrentState != EDMSEIState::EIS_Persistent) CurrentState = EDMSEIState::EIS_PendingKill; }
	//else if (CurrentState != EDMSEIState::EIS_Persistent) CurrentState = EDMSEIState::EIS_PendingKill;

	if (CurrentState != EDMSEIState::EIS_Persistent) CurrentState = EDMSEIState::EIS_PendingKill;
}

void UDMSEffectInstance::Initialize(UDMSEffectNode* iNode, UDMSDataObjectSet* iSet) 
{ 
	EffectNode = iNode; 
	CurrentState = EDMSEIState::EIS_Pending; 
	DataSet = iSet != nullptr ? iSet : NewObject<UDMSDataObjectSet>(); 
	
	//SetupPreviewDummy();
}

void UDMSEffectInstance::Initialize(UDMSEffectNode* iNode, UDMSSequence* iSeq)
{ 
	EffectNode = iNode; SourcePlayer=iSeq->GetSourcePlayer(); 
	SourceObject = iSeq->GetSourceObject(); 

	UDMSDataObjectSet* NewData = NewObject<UDMSDataObjectSet>(this);
	NewData->Inherit(iSeq->SequenceDatas);
	NewData->ParentDataSet = iSeq->SequenceDatas;
	DataSet = NewData;
	CurrentState = EDMSEIState::EIS_Pending; 

}


UDMSSequence* UDMSEffectInstance::CreateSequenceFromNode(UObject* SourceTweak, UDMSSequence* ChainingSequence) 
{
	auto SM = UDMSCoreFunctionLibrary::GetDMSSequenceManager();
	if (SM == nullptr) return nullptr;
	TArray<TScriptInterface<IDMSEffectorInterface>> Target;
	Target.Add(GetOuter());
	return SM->RequestCreateSequence(SourceTweak, SourcePlayer, EffectNode, Target, DataSet, ChainingSequence);
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

	if ( EffectNode->bIgnoreNotify ) {
		//DMS_LOG_SIMPLE(TEXT("Ignore Notify"));
		return rv;
	}

	if ( !iChainable && !EffectNode->bForced ) return rv;

	if ( CurrentState == EDMSEIState::EIS_Pending ) return rv;

	//if (Seq->OriginalEffectNode == EffectNode) {
	//	DMS_LOG_SIMPLE(TEXT("Recursive Response Occured"));
	//	return rv;
	//}

	if ( EffectNode->Conditions->CheckCondition(SourceTweak, Seq) )
	{
		//DMS_LOG_SCREEN(TEXT("%s -> %s : Notify Checked"), GetTypedOuter<AActor>() != nullptr ? *GetTypedOuter<AActor>()->GetName() : TEXT("NullOuter"), *GetName());
		outResponsedObjects.Add(SourceTweak, this);
		rv=true;	
	}
	else rv=false;
	return rv;
}

void UDMSEffectInstance::Serialize(FArchive& Ar)
{
	//DMS_LOG_SCREEN(TEXT("UDMSEffectInstance Serialized"));
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


//UDMSEffectSet* UDMSEffectInstance::GetOwningEffectSet()
//{
//	//
//
//	return nullptr;
//}

//FArchive& operator<<(FArchive& Ar, UDMSEffectInstance*& EI)
//{
//	// TODO: 여기에 return 문을 삽입합니다.
//	EI->Serialize(Ar);
//	return Ar;
//}

void UDMSEffectApplyWorker::SetupWorker(UDMSSequence* iSequence, UDMSEffectInstance* iEI, const FOnApplyCompleted& iOnApplyCompleted)
{
	SourceSequence = iSequence;
	OwnerInstance = iEI;
	ApplyingEffect = iEI->EffectNode;
	CurrentEDIndex = 0;
	CompletedDelegate = iOnApplyCompleted;
	IteratingDelegate.BindDynamic(this, &UDMSEffectApplyWorker::ApplyNextEffectDef);
}

void UDMSEffectApplyWorker::ApplyNextEffectDef(bool PrevSuccessed)
{
	if (!PrevSuccessed)
	{
		// DISCUSSION :: Stopping immediately when failed is FINE?
		OwnerInstance->SetToPendingKill();
		CompletedDelegate.ExecuteIfBound(SourceSequence, false);
		return;
	}

	if (CurrentEDIndex == ApplyingEffect->EffectDefinitions.Num()) {
		OwnerInstance->SetToPendingKill();
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

			// Ignored effect is considered to successed.
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
