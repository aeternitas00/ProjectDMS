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
	const EDMSTimingFlag& RelationFlag
)
{
	// TODO :: Root Seq Check and make Chain tree work / Cleanup

	DMS_LOG_SCREEN(TEXT("%s : RequestCreateSequence"), *SourceObject->GetName());

	UDMSEffectHandler* EH = UDMSCoreFunctionLibrary::GetDMSEffectHandler();
	if (EH==nullptr) { /*DMS_LOG("Invalid EH");*/ return nullptr; }

	if (EffectNode==nullptr){ DMS_LOG_SCREEN(TEXT("%s : EffectNode is Null"), *SourceObject->GetName()); return nullptr;}
	
	UDMSDataObjectSet* NewData = NewObject<UDMSDataObjectSet>();
	NewData->Inherit(Datas);

	// ������ ����
	// �� �ܰ� ( �����Ϳ��� ĵ�� ���� ) ���� ��ҽ� ������ ���� �ܰ�� ���� X
	// �����Ͱ� ���ų� �Ϸ� �Ǹ� DataSet �����ϰ� 
	// DataSet & EN �� �̿��Ͽ� EI�� ����.

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

	// Lambda with capture doesn't work with simple conversion to function pointer.
	// Using exact signature to param for a while. fix this when find proper way  
	auto WidgetOwner = Cast<APlayerController>(SourceController);

	EffectNode->CreateSelectors(WidgetOwner, Sequence);
	
	Sequence->RunSelectorQueue([&](UDMSSequence* Sequence){
		RunSequence(Sequence);
		// �����ߴٰ� ����
		// RunSequence�� �Ѿ. ( RunSequence���� ��Ƽ���� ���� ���� )
		for (auto EI : Sequence->EIs)
		{
			EI->ChangeEIState(EDMSEIState::EIS_Default);
		}
		// ==>> ��Ƽ���� ����� �߰������� �����Ǵ� �������� �� �޼ҵ带 ���� ����.
		// ���ʷ� ���� ( Root EI ) �� After ��ε�ĳ��Ʈ�� �����鼭 ����� Ʈ�� ���� ����.
	}, [&](UDMSSequence* Sequence){});
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

	
	if (Sequence->OriginalEffectNode->bIsChainableEffect)
	{
		// �� ��� ���� �޴� ���忡�� �˾Ƽ� ���� �ϰ� �ϸ� �ѹ� �Ѹ��� ������ �ذ� �� �� ���� ��?
		DMS_LOG_SCREEN(TEXT("==-- BEFORE --=="));
		NotifyManager->BroadCast(Sequence);
		Sequence->Progress = EDMSTimingFlag::T_During;

		DMS_LOG_SCREEN(TEXT("==-- DURING --=="));
		EffectHandler->Resolve(Sequence);
		NotifyManager->BroadCast(Sequence);

		Sequence->Progress = EDMSTimingFlag::T_After;
		DMS_LOG_SCREEN(TEXT("==-- AFTER --=="));
		NotifyManager->BroadCast(Sequence);
	}
	else 
	{
		DMS_LOG_SCREEN(TEXT("==-- NOT CHAINABLE --=="));
		Sequence->Progress = EDMSTimingFlag::T_After;
		EffectHandler->Resolve(Sequence);
	}
}

// Global&Static GetSeqManager()