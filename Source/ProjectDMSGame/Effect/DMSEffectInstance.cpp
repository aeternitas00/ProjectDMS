// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEffectHandler.h"
//#include "Effect/DMSEffectDefinition.h"

#include "Sequence/DMSSeqManager.h"
#include "Sequence/DMSSequence.h"

#include "Conditions/DMSConditionObject.h"

#include "GameModes/DMSGameState.h"
#include "GameModes/DMSGameMode.h"

#include "EffectSet/DMSEffect_CancelEffect.h"
#include "EffectSet/DMSEffect_ActivateEffect.h"
#include "Card/DMSCardBase.h"
#include "Library/DMSCoreFunctionLibrary.h"

UDMSEffectInstance::UDMSEffectInstance() :CurrentState(EDMSEIState::EIS_Default) { IteratingDelegate.BindDynamic(this, &UDMSEffectInstance::ApplyNextEffectDefinition); }


void UDMSEffectInstance::Apply(UDMSSequence* SourceSequence, const FResolveIteratingDelegate& OnApplyCompleted)
{
	//DMS_LOG_SCREEN(TEXT("%s : EI Apply [%s]"), *GetName(), *EffectNode->GenerateTagContainer().ToString());
	//for (auto EffectDefinition : EffectNode->EffectDefinitions )
	//{
	//	EffectDefinition->Work(this);
	//}

	if (EffectNode->EffectDefinitions.Num() == 0) {
		OnApplyCompleted.ExecuteIfBound(SourceSequence,true);
		return;
	}

	OnApplyCompletedMap.Add(SourceSequence);
	OnApplyCompletedMap[SourceSequence].CompletedDelegate=OnApplyCompleted;
	OnApplyCompletedMap[SourceSequence].Index = 0;
	//
	ApplyNextEffectDefinition(SourceSequence, true);
}

IDMSEffectorInterface* UDMSEffectInstance::GetApplyTarget()
{
	auto Outer = Cast<IDMSEffectorInterface>(GetOuter());
	if (CurrentState == EDMSEIState::EIS_Preview)
		return Outer==nullptr ? nullptr : Cast<UDMSEffectInstance>(Outer)->GetApplyTarget();
	else 
		return Outer;
}
void UDMSEffectInstance::SetToPendingKill()
{
	if (CurrentState == EDMSEIState::EIS_Preview ){ if(GetTypedOuter<UDMSEffectInstance>()->CurrentState != EDMSEIState::EIS_Persistent) CurrentState = EDMSEIState::EIS_PendingKill; }
	else if (CurrentState != EDMSEIState::EIS_Persistent) CurrentState = EDMSEIState::EIS_PendingKill;
}

void UDMSEffectInstance::ApplyNextEffectDefinition(UDMSSequence* SourceSequence, bool PrevSuccessed)
{
	if (!PrevSuccessed)
	{
		// DISCUSSION :: Stopping immediately when failed is FINE?
		SetToPendingKill();
		OnApplyCompletedMap[SourceSequence].CompletedDelegate.ExecuteIfBound(SourceSequence, false);
		return;
	}

	if (OnApplyCompletedMap[SourceSequence].Index == EffectNode->EffectDefinitions.Num()) {
		SetToPendingKill();
		OnApplyCompletedMap[SourceSequence].CompletedDelegate.ExecuteIfBound(SourceSequence,true);
	}
	else {
		FGameplayTagQuery Query;
		auto CurrentDef = EffectNode->EffectDefinitions[OnApplyCompletedMap[SourceSequence].Index++];

		// ====================== //
		//    Effect Canceling    //
		// ====================== // 
		// Have to think about more complicated situations.

		if (SourceSequence->EIDatas->ContainData(TAG_DMS_Effect_IgnoreEffect) && 
		SourceSequence->EIDatas->GetData(TAG_DMS_Effect_IgnoreEffect)->TypeCheck<FGameplayTagQuery>()) 
			Query = SourceSequence->EIDatas->GetData(TAG_DMS_Effect_IgnoreEffect)->Get<FGameplayTagQuery>();
		
		if (Query.IsEmpty() || !Query.Matches(FGameplayTagContainer(CurrentDef->EffectTag))){
			if (CurrentDef->bPlayerHasToBeFocused) 	
				UDMSCoreFunctionLibrary::GetDMSGameState()->SetPlayersFocusTarget(CurrentDef->GetPlayerFocusTarget(SourceSequence, this));

			// Predict first
			if ( CurrentDef->Predict(SourceSequence, this) )
				CurrentDef->Work(SourceSequence, this, IteratingDelegate);
			// Going back if it'll be failed ( DISCUSSION :: OPTIONAL FAIL BACK? )
			else
				ApplyNextEffectDefinition(SourceSequence, false);
		}

		else {
			// Ignored effect is considered to successed.
			ApplyNextEffectDefinition(SourceSequence, true);
		}
	}
}

void UDMSEffectInstance::Initialize(UDMSEffectNode* iNode, UDMSDataObjectSet* iSet) 
{ 
	EffectNode = iNode; 
	CurrentState = EDMSEIState::EIS_Pending; 
	DataSet = iSet != nullptr ? iSet : NewObject<UDMSDataObjectSet>(); 
	
	SetupPreviewDummy();
}

void UDMSEffectInstance::Initialize(UDMSEffectNode* iNode, UDMSSequence* iSeq)
{ 
	EffectNode = iNode; SourcePlayer=iSeq->GetSourcePlayer(true); 
	SourceObject = iSeq->GetSourceObject(true); 
	DataSet = iSeq->EIDatas; 
	CurrentState = EDMSEIState::EIS_Pending; 

	SetupPreviewDummy();
}

void UDMSEffectInstance::SetupPreviewDummy()
{
	// NOTE :: 자기 자신에게 체이닝 되는 이펙트들을 위한 프리뷰 더미이므로 부착은 자신 (EI) 에게 해놓을것..

	PreviewDummy = DuplicateObject<UDMSEffectInstance>(this, this, FName(GetName() + TEXT("_Preview")));
	PreviewDummy->CurrentState = EDMSEIState::EIS_Preview;
	// Reference update
	PreviewDummy->SourceObject = SourceObject;
	PreviewDummy->SourcePlayer = SourcePlayer;
}

UDMSSequence* UDMSEffectInstance::CreateSequenceFromNode(UObject* SourceTweak, UDMSSequence* ChainingSequence) 
{
	auto SM = UDMSCoreFunctionLibrary::GetDMSSequenceManager();
	if (SM == nullptr) return nullptr;
	return SM->RequestCreateSequence(SourceTweak, SourcePlayer, EffectNode, {}, DataSet, ChainingSequence);
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

	if (CurrentState == EDMSEIState::EIS_Pending) return rv;

	//if (Seq->OriginalEffectNode == EffectNode) {
	//	DMS_LOG_SIMPLE(TEXT("Recursive Response Occured"));

	//	return rv;
	//}
	if (EffectNode->Conditions->CheckCondition(SourceTweak, Seq))
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
