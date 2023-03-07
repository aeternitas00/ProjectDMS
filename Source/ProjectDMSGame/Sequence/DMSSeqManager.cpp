// Fill out your copyright notice in the Description page of Project Settings.


#include "Sequence/DMSSeqManager.h"
#include "Sequence/DMSSequence.h"
#include "Kismet/GameplayStatics.h"
#include "GameModes/DMSGameMode.h"
#include "Effect/DMSEffectHandler.h"
#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEffectDefinition.h"
#include "Effect/DMSEffectorInterface.h"
#include "Notify/DMSNotifyManager.h"
#include "Library/DMSCoreFunctionLibrary.h"



UDMSSeqManager::UDMSSeqManager() : RootSequence(nullptr)
{

}



UDMSSequence* UDMSSeqManager::RequestCreateSequence(
	UObject* SourceObject,
	AActor* SourceController, 
	UDMSEffectNode* EffectNode, 
	TArray<TScriptInterface<IDMSEffectorInterface>> Targets, 
	UDMSDataObjectSet* Datas, 
	UDMSSequence* ParentSequence, 
	EDMSTimingFlag RelationFlag
)
{
	// TODO :: Root Seq Check and make Chain tree work / Cleanup

	DMS_LOG_SCREEN(TEXT("%s : Request Create Sequence of [%s]"), *SourceObject->GetName(),*EffectNode->NodeKeyword.ToString());

	UDMSEffectHandler* EH = UDMSCoreFunctionLibrary::GetDMSEffectHandler();
	if (EH==nullptr) { /*DMS_LOG("Invalid EH");*/ return nullptr; }

	//if (EffectNode==nullptr){ DMS_LOG_SCREEN(TEXT("%s : EffectNode is Null"), *SourceObject->GetName()); return nullptr;}
	
	UDMSDataObjectSet* NewData = NewObject<UDMSDataObjectSet>();
	NewData->Inherit(Datas);

	// 셀렉터 실행
	// 이 단계 ( 셀렉터에서 캔슬 실행 ) 에서 취소시 시퀀스 생성 단계로 진입 X
	// 셀렉터가 없거나 완료 되면 DataSet 갱신하고 
	// DataSet & EN 을 이용하여 EI를 생성.

	UDMSSequence* Sequence = NewObject<UDMSSequence>(this);
	Sequence->OriginalEffectNode = EffectNode;
	Sequence->SourceObject = SourceObject;
	Sequence->SourceController = SourceController;
	Sequence->EIDatas = NewData;
	Sequence->Targets = Targets;
	switch(RelationFlag)	{
		case EDMSTimingFlag::T_Before:
			Sequence->BeforeNode = ParentSequence;
			break;
		case EDMSTimingFlag::T_During:
			Sequence->DuringNode = ParentSequence;
			break;
		case EDMSTimingFlag::T_After:
			Sequence->AfterNode = ParentSequence;
			break;
		case EDMSTimingFlag::T_Null:
		default:
			if (RootSequence==nullptr) RootSequence = ParentSequence;
			else Sequence->ParentNode = ParentSequence;
			break;
		
	};
	
	EH->CreateEffectInstance(Sequence, EffectNode);

	auto WidgetOwner = Cast<APlayerController>(SourceController);
	
	if (WidgetOwner==nullptr) { 
		/*it's from GAMEMODE or SYSTEM THINGs. LEADER PLAYER own this selector */
	}

	EffectNode->CreateSelectors(WidgetOwner, Sequence);
	
	Sequence->RunSelectorQueue([&](UDMSSequence* pSequence){

		RunSequence(pSequence);

		for (auto EI : pSequence->EIs)
		{
			EI->ChangeEIState(EDMSEIState::EIS_Default);
		}
		// ==>> 노티파이 결과로 추가적으로 생성되는 시퀀스는 이 메소드를 통해 진행.
		// 최초로 들어온 ( Root EI ) 의 After 브로드캐스트가 끝나면서 재귀적 트리 진행 종료.
	}, [&](UDMSSequence* pSequence){
			//cleanup this seq
		}
	);
	return Sequence;
}


void UDMSSeqManager::RunSequence(UDMSSequence* Sequence)
{
	DMS_LOG_SCREEN(TEXT("%s : RunSequence [%s]"), *Sequence->SourceObject->GetName(), Sequence->OriginalEffectNode->NodeKeyword == "" ? *Sequence->OriginalEffectNode->EffectDefinitions[0]->Keyword.ToString() : *Sequence->OriginalEffectNode->NodeKeyword.ToString());
	// Before Notify
	auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager();
	auto EffectHandler = UDMSCoreFunctionLibrary::GetDMSEffectHandler();

	check(NotifyManager);
	check(EffectHandler);

	Sequence->SetActive(true);

	// 이 방식 말고 받는 입장에서 알아서 생각 하게 하면 한번 뿌리는 것으로 해결 할 수 있을 듯?
	//DMS_LOG_SCREEN(TEXT("==-- BEFORE --=="));
	NotifyManager->BroadCast(Sequence, Sequence->OriginalEffectNode->bIsChainableEffect);
	Sequence->Progress = EDMSTimingFlag::T_During;

	//DMS_LOG_SCREEN(TEXT("==-- DURING --=="));
	EffectHandler->Resolve(Sequence);
	NotifyManager->BroadCast(Sequence, Sequence->OriginalEffectNode->bIsChainableEffect);
	if (Sequence->OriginalEffectNode->ChildEffect != nullptr && Sequence->OriginalEffectNode->AdvanceConditions.CheckCondition(Sequence->SourceObject, Sequence))
	{
		//DMS_LOG_SCREEN(TEXT("%s : OnNotifyReceived -> Advance"), *GetName());
		RequestCreateSequence(Sequence->SourceObject, Sequence->SourceController, Sequence->OriginalEffectNode->ChildEffect->GetEffectNode(), Sequence->Targets, Sequence->EIDatas, Sequence);
	}

	Sequence->Progress = EDMSTimingFlag::T_After;
	//DMS_LOG_SCREEN(TEXT("==-- AFTER --=="));
	NotifyManager->BroadCast(Sequence, Sequence->OriginalEffectNode->bIsChainableEffect);

	// cleanup Sequence;
}
