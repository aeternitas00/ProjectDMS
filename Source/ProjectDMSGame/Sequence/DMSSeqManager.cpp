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
#include "Selector/DMSDecisionWidget.h"
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
	auto WidgetOwner = Cast<APlayerController>(SourceController);	
	
	if (WidgetOwner == nullptr) {
		//	it's from GAMEMODE or SYSTEM THINGs. LEADER PLAYER own this selector 
		//	���� ��Ƽ �÷��� ȯ�濡�� ��� �� �� �����غ���.
	}

	// ====== Decision Making Step ====== //
	// ( ex. User Choose target, ... )

	TArray<UDMSConfirmWidgetBase*> Widgets;
	if (!EffectNode->bForced && DefaultYNWidget != nullptr) 
		Widgets.Add(NewObject<UDMSConfirmWidgetBase>(this, DefaultYNWidget.Get()));
	Widgets.Append(TArray<UDMSConfirmWidgetBase*>(EffectNode->CreateDecisionWidgets()));
	Sequence->InitializeWidgetQueue(Widgets,WidgetOwner);
	Sequence->RunWidgetQueue([&,WidgetOwner](UDMSSequence* pSequence) {

		// Decision confirmed or no decision widget

		// Create EI first for preview.
		UDMSCoreFunctionLibrary::GetDMSEffectHandler()->CreateEffectInstance(pSequence, pSequence->OriginalEffectNode);

		// ====== Effect Data Selection Step ====== //
		// ( ex. User set value of effect's damage amount , ... )
		pSequence->InitializeWidgetQueue(TArray<UDMSConfirmWidgetBase*>(pSequence->OriginalEffectNode->CreateSelectors()), WidgetOwner);
		pSequence->RunWidgetQueue([this](UDMSSequence* pSequenceN){
			// Selection completed

			// Run sequence ( Notifying steps and apply )
			RunSequence(pSequenceN);

			for (auto EI : pSequenceN->EIs)
			{
				EI->ChangeEIState(EDMSEIState::EIS_Default);
			}
			// ==>> ��Ƽ���� ����� �߰������� �����Ǵ� �������� �� �޼ҵ带 ���� ����.
			// ���ʷ� ���� ( Root EI ) �� After ��ε�ĳ��Ʈ�� �����鼭 ����� Ʈ�� ���� ����.
		}, [this](UDMSSequence* pSequenceN){
			// Selection Canceled

			// Cleanup this seq
			CleanupSequenceTree();
		}
		);
	}, [this](UDMSSequence* pSequence) {
		// Decision canceled

		// Cleanup this seq
		CleanupSequenceTree();
		//...
	}
	);
	return Sequence;
}

void UDMSSeqManager::CleanupSequenceTree()
{
	//...
	RootSequence = nullptr;
	//...
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

	// �� ��� ���� �޴� ���忡�� �˾Ƽ� ���� �ϰ� �ϸ� �ѹ� �Ѹ��� ������ �ذ� �� �� ���� ��?
	//DMS_LOG_SCREEN(TEXT("==-- BEFORE --=="));
	NotifyManager->BroadCast(Sequence, Sequence->OriginalEffectNode->bIsChainableEffect);
	Sequence->Progress = EDMSTimingFlag::T_During;

	//DMS_LOG_SCREEN(TEXT("==-- DURING --=="));
	EffectHandler->Resolve(Sequence);
	NotifyManager->BroadCast(Sequence, Sequence->OriginalEffectNode->bIsChainableEffect);
	// if (Sequence->OriginalEffectNode->ChildEffect != nullptr && Sequence->OriginalEffectNode->ChildEffect->Conditions_->CheckCondition(SourceTweak, Seq))
	if (Sequence->OriginalEffectNode->ChildEffect != nullptr && Sequence->OriginalEffectNode->AdvanceConditions_->CheckCondition(Sequence->SourceObject, Sequence))
	{
		//DMS_LOG_SCREEN(TEXT("%s : OnNotifyReceived -> Advance"), *GetName());
		RequestCreateSequence(Sequence->SourceObject, Sequence->SourceController, Sequence->OriginalEffectNode->ChildEffect->GetEffectNode(), Sequence->Targets, Sequence->EIDatas, Sequence);
	}

	Sequence->Progress = EDMSTimingFlag::T_After;
	//DMS_LOG_SCREEN(TEXT("==-- AFTER --=="));
	NotifyManager->BroadCast(Sequence, Sequence->OriginalEffectNode->bIsChainableEffect);

	// cleanup Sequence;
}
