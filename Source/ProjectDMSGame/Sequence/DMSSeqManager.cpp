// Fill out your copyright notice in the Description page of Project Settings.


#include "Sequence/DMSSeqManager.h"
#include "Sequence/DMSSequence.h"
#include "Kismet/GameplayStatics.h"
#include "GameModes/DMSGameMode.h"
#include "Effect/DMSEffectHandler.h"
#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEffectDefinition.h"
#include "Effect/DMSEffectorInterface.h"
#include "Conditions/DMSConditionObject.h"
#include "Notify/DMSNotifyManager.h"
#include "Selector/DMSDecisionWidget.h"
#include "Library/DMSCoreFunctionLibrary.h"



UDMSSeqManager::UDMSSeqManager() : RootSequence(nullptr), CurrentSequence(nullptr)
{

}



UDMSSequence* UDMSSeqManager::RequestCreateSequence(
	UObject* SourceObject,
	AActor* SourceController, 
	UDMSEffectNode* EffectNode, 
	TArray<TScriptInterface<IDMSEffectorInterface>> Targets = TArray<TScriptInterface<IDMSEffectorInterface>>(),
	UDMSDataObjectSet* Datas, 
	UDMSSequence* ParentSequence
)
{
	// TODO :: Root Seq Check and make Chain tree work / Cleanup


	UDMSEffectHandler* EH = UDMSCoreFunctionLibrary::GetDMSEffectHandler();
	if (EH==nullptr) { /*DMS_LOG_DETAIL("Invalid EH");*/ return nullptr; }

	//if (EffectNode==nullptr){ DMS_LOG_SCREEN(TEXT("%s : EffectNode is Null"), *SourceObject->GetName()); return nullptr;}
	
	//FString SeqName = TEXT("Sequence")+ EffectNode->NodeTag.ToString();
	UDMSSequence* Sequence = NewObject<UDMSSequence>(this/*, FName(*SeqName)*/);
	UDMSDataObjectSet* NewData = NewObject<UDMSDataObjectSet>(Sequence);
	NewData->Inherit(Datas);
	Sequence->OriginalEffectNode = EffectNode;
	Sequence->SourceObject = SourceObject;
	Sequence->SourceController = SourceController;
	Sequence->EIDatas = NewData;
	Sequence->Targets = Targets;
	if (ParentSequence == nullptr) {
		if (RootSequence == nullptr) {
			RootSequence = Sequence;
			RootSequence->OnSequenceInitiated.AddUObject(this, &UDMSSeqManager::OnSequenceTreeInitiated);
			RootSequence->OnSequenceFinished.AddUObject(this, &UDMSSeqManager::OnSequenceTreeCompleted);
		}
		else if (CurrentSequence != nullptr)
			CurrentSequence->AttachChildSequence(Sequence);
	}
	else {
		ParentSequence->AttachChildSequence(Sequence);
	}

	//if (Sequence == Sequence->ParentSequence) {
	//	DMS_LOG_SIMPLE(TEXT("ERROR"));
	//}

	DMS_LOG_SIMPLE(TEXT("==== %s : Request Create Sequence [%s] of [%s] to [%d : %s] ===="),  *SourceObject->GetName(), *Sequence->GetName(), *EffectNode->NodeTag.ToString(), GetDepth(Sequence), Sequence->ParentSequence == nullptr ? TEXT("EmptySequence") : *Sequence->ParentSequence->GetName());

	return Sequence;
}


void UDMSSeqManager::RunSequence(UDMSSequence* iSeq)
{
	DMS_LOG_SIMPLE(TEXT("==== %s : RUN SEQUENCE ===="), *iSeq->GetName());

	auto WidgetOwner = Cast<APlayerController>(iSeq->SourceController);

	if (WidgetOwner == nullptr) {
		//	it's from GAMEMODE or SYSTEM THINGs. LEADER PLAYER own this selector 
		//	추후 멀티 플레이 환경에서 어떻게 할 지 생각해보기.
	}

	CurrentSequence = iSeq;
	CurrentSequence->OnSequenceInitiate();

	// ====== Decision Making Step ====== //
	// ( ex. User Choose target, ... )

	TArray<UDMSConfirmWidgetBase*> Widgets;
	//if (!iSeq->OriginalEffectNode->bForced && DefaultYNWidget != nullptr)
	//	Widgets.Add(NewObject<UDMSConfirmWidgetBase>(this, DefaultYNWidget.Get()));
	Widgets.Append(TArray<UDMSConfirmWidgetBase*>(iSeq->OriginalEffectNode->CreateDecisionWidgets(WidgetOwner)));
	if (!iSeq->SetupWidgetQueue(Widgets))
	{
		DMS_LOG_SIMPLE(TEXT("Can't setup Decision selector"));
		CompleteSequence(iSeq);
		return;
	}

	iSeq->RunWidgetQueue(
	[&, WidgetOwner](UDMSSequence* pSequence) {
		// Decision confirmed or no decision widget

		// Create EI first for preview.
		UDMSCoreFunctionLibrary::GetDMSEffectHandler()->CreateEffectInstance(pSequence, pSequence->OriginalEffectNode);

		// ====== Effect Data Selection Step ====== //
		// ( ex. User set value of effect's damage amount , ... )
		if (!pSequence->SetupWidgetQueue(TArray<UDMSConfirmWidgetBase*>(pSequence->OriginalEffectNode->CreateSelectors(WidgetOwner))))
		{
			DMS_LOG_SIMPLE(TEXT("Can't setup Effect selector"));
			CompleteSequence(pSequence);
			return;
		}

		pSequence->RunWidgetQueue(
		[this](UDMSSequence* pSequenceN) {
			// Selection completed

			// Run sequence ( Notifying steps and apply )
			DMS_LOG_SIMPLE(TEXT("==== %s : EI Data Selection Completed  ===="),*pSequenceN->GetName());
			ApplySequence(pSequenceN);

			for (auto EI : pSequenceN->EIs)
			{
				EI->ChangeEIState(EDMSEIState::EIS_Default);
			}
		}, 
		[this](UDMSSequence* pSequenceN) {
			// Selection Canceled
			DMS_LOG_SIMPLE(TEXT("Selection Canceled"));
			CompleteSequence(pSequenceN);
			// Cleanup this seq
		} // Runwidgetqueue end.
		);
	}, 

	[this](UDMSSequence* pSequence) {
		// Decision canceled
		DMS_LOG_SIMPLE(TEXT("Decision canceled"));
		// Cleanup this seq
		CompleteSequence(pSequence);
		//...
	}
	);

}

int UDMSSeqManager::GetDepth(UDMSSequence* iSeq) {
	if (iSeq == nullptr) return -1;
	if (iSeq == RootSequence) return 0;
	return GetDepth(iSeq->ParentSequence)+1;
}

void UDMSSeqManager::ApplySequence(UDMSSequence* Sequence)
{
	DMS_LOG_SIMPLE(TEXT("==== %s : Apply Sequence [ Depth : %d ] ===="), *Sequence->GetName(), GetDepth(Sequence));

	DMS_LOG_SCREEN(TEXT("%s : ApplySequence [%s]"), *Sequence->SourceObject->GetName(), 
	Sequence->OriginalEffectNode->NodeTag == FGameplayTag::EmptyTag ? 
	*Sequence->OriginalEffectNode->EffectDefinitions[0]->EffectTag.ToString() :
	*Sequence->OriginalEffectNode->NodeTag.ToString());
	// Before Notify

	auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager();
	auto EffectHandler = UDMSCoreFunctionLibrary::GetDMSEffectHandler();
	check(NotifyManager);
	check(EffectHandler);

	//Sequence->SetActive(true);

	// 이 방식 말고 받는 입장에서 알아서 생각 하게 하면 한번 뿌리는 것으로 해결 할 수 있을 듯?

	DMS_LOG_SCREEN(TEXT("==-- BEFORE [ Depth : %d ] --=="),GetDepth(Sequence));
	// 'Before Timing' broadcast starts.
	NotifyManager->BroadCast(Sequence, 
	[=, BeforeSequence=Sequence]() {
			// ==== ON BEFORE TIMING RESPONSE ENDED ====
		DMS_LOG_SIMPLE(TEXT("==== %s : ON BEFORE TIMING RESPONSE ENDED [ Depth : %d ] ===="), *BeforeSequence->GetName(), GetDepth(BeforeSequence));
		// Proceed to 'During Timing'
		BeforeSequence->Progress = EDMSTimingFlag::T_During;

		auto SeqManager = UDMSCoreFunctionLibrary::GetDMSSequenceManager();
		DMS_LOG_SCREEN(TEXT("==-- DURING [ Depth : %d ] --=="), SeqManager->GetDepth(BeforeSequence));
		
		// Resolve First
		DMS_LOG_SIMPLE(TEXT("==== %s : RESOLVE START ===="), *BeforeSequence->GetName());

		EffectHandler->Resolve(BeforeSequence,
		[=, DuringSequence=BeforeSequence]() {
				// ==== ON RESOLVE COMPLETED ====

			DMS_LOG_SIMPLE(TEXT("==== %s : ON RESOLVE COMPLETED [ Depth : %d ] ===="), *DuringSequence->GetName(), SeqManager->GetDepth(DuringSequence));
			// 'During Timing' broadcast starts.
			auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager();
			NotifyManager->BroadCast(BeforeSequence, 
			[=, DurningSequence = BeforeSequence]() __declspec(noinline) {
					// ==== ON DURING TIMING RESPONSE ENDED ====
				DMS_LOG_SIMPLE(TEXT("==== %s : ON DURING TIMING RESPONSE ENDED [ Depth : %d ] ===="), *DurningSequence->GetName(), SeqManager->GetDepth(DurningSequence));
				// Proceed to 'After Timing'
				DurningSequence->Progress = EDMSTimingFlag::T_After;

				DMS_LOG_SCREEN(TEXT("==-- AFTER [ Depth : %d ] --=="), SeqManager->GetDepth(DurningSequence));

				// 'After Timing' broadcast starts.
				NotifyManager->BroadCast(DurningSequence, 
				[=, AfterSequence = DurningSequence]() __declspec(noinline) {
						// ==== ON AFTER TIMING RESPONSE ENDED ====
					DMS_LOG_SIMPLE(TEXT("==== %s : ON AFTER TIMING RESPONSE ENDED [ Depth : %d ] ===="), *AfterSequence->GetName(), SeqManager->GetDepth(AfterSequence));
					
					// Run child effect if exist.
					if (AfterSequence->SequenceState!=EDMSSequenceState::SS_Canceled &&
						AfterSequence->OriginalEffectNode->ChildEffect != nullptr &&
						AfterSequence->OriginalEffectNode->ChildEffect->GetEffectNode()->Conditions->CheckCondition(AfterSequence->SourceObject, AfterSequence))	{

						// Proceed to run child effect sequence.
						DMS_LOG_SIMPLE(TEXT("==== %s : OnNotifyReceived -> Advance ===="), *AfterSequence->GetName());
						auto ChildNode = AfterSequence->OriginalEffectNode->ChildEffect->GetEffectNode();
						// follows parents data. 
						auto NewSeq = RequestCreateSequence(AfterSequence->SourceObject, AfterSequence->SourceController, ChildNode,
							TArray<TScriptInterface<IDMSEffectorInterface>>(), AfterSequence->EIDatas, AfterSequence);
				
						// Set delegates when child effect sequence completed.
						NewSeq->AddToOnSequenceFinished_Native(
						[=, ParentSequence= AfterSequence]() __declspec(noinline) {
								// ==== ON CHILD EFFECT SEQUENCE COMPLETED ====
							DMS_LOG_SIMPLE(TEXT("==== %s : ON CHILD EFFECT SEQUENCE COMPLETED [ Depth : %d ] ==== "), *ParentSequence->GetName(), SeqManager->GetDepth(ParentSequence));

							auto NotifyManager = UDMSCoreFunctionLibrary::GetDMSNotifyManager();
					
							// Resume parent sequence closing
							SeqManager->CompleteSequence(ParentSequence);
						});

						// Run setuped child effect sequence.
						SeqManager->RunSequence(NewSeq);
					}
					else //Complete this sequence.
						SeqManager->CompleteSequence(AfterSequence);
				});
			});
			
		});
	});

}

void UDMSSeqManager::CompleteSequence(UDMSSequence* Sequence)
{
	DMS_LOG_SIMPLE(TEXT("==== %s : Complete Sequence ===="), *Sequence->GetName());

	// 이렇게 분기 안걸어도 되는데 혹시 몰라서 일단.
	if (CurrentSequence == RootSequence) {
		CurrentSequence = nullptr;
		RootSequence = nullptr;
	}
	else
		CurrentSequence = Sequence->ParentSequence;

	Sequence->OnSequenceFinish();

}


void UDMSSeqManager::CleanupSequenceTree()
{
	//...
	RootSequence = nullptr;
	// Else will be GCed.
	//...
}
