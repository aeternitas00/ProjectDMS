// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *	Class for objectifying "Events" that happen in the game.
 *
 *	========================================
 *	objectifying 이 맞는 표현인가?
 */

#include "ProjectDMS.h"
#include "Common/DMSCommons.h"
#include "Common/DMSCommonDelegates.h"
#include "Selector/DMSSelectorQueue.h"
#include "UObject/NoExportTypes.h"
#include "DMSSequence.generated.h"



class UDMSEffectInstance;
class UDMSDataObjectSet;
class UDMSEffectorInterface;
class UDMSEffectNode;
class UDMSConfirmWidgetBase;


/** 
 * 	========================================
 *
 *	초고 : 일반적으로 게임에서 일어나는 일들이 모두 '이펙트'의 결과이며 이 이펙트가 어떻게 진행 되는지에 대한 정보를 오브젝트화 하는 형태로 설계.
 *	게임 중 어떤 사건이 일어나면 각 게임 오브젝트들이 이 시퀀스 정보를 받아서 각자가 적절한 반응을 할 수 있게 만들기 위함. ( 컨디션 객체가 시퀀스를 받아서 체크하는 형태 )
 *	 
 *	========================================
 */
UCLASS(ClassGroup = (Sequence))
class UDMSSequence : public UObject
{
	GENERATED_BODY()

public:
	// Default Initializer
	UDMSSequence( ) { 
		Progress = EDMSTimingFlag::T_Before; //EDMSTimingFlag::T_Null;
		//bIsActive=false;
		SourceObject = nullptr;
		ParentSequence = nullptr;
		ChildSequence = nullptr;
		// 
	}

	//UPROPERTY(/*VisibleAnywhere, BlueprintReadOnly*/)
	//bool bIsActive;

	// 어떠한 이펙트가 실제로 월드에 영향을 끼치기 까지의 과정 중에 간섭할 '타이밍'은 결론적으로 발동 전, 도중, 후 세가지로 크게 나눌 수 있음.
	UPROPERTY(/*VisibleAnywhere, BlueprintReadOnly*/)
	EDMSTimingFlag Progress;

	// 이펙트 노드와 1:1 매칭.
	// 이 시퀀스에 있어 사실상의 의미절에 해당함.
	UPROPERTY(/*VisibleAnywhere, BlueprintReadOnly*/)
	UDMSEffectNode* OriginalEffectNode;

	// 미리 선택하고 발동을 해야하는 카드들을 위해?
	UPROPERTY(/*VisibleAnywhere, BlueprintReadOnly*/)
	TArray<TScriptInterface<IDMSEffectorInterface>> Targets; // 개편후 정리 / 재고 필요

	// 발동 의지를 보인 플레이어 (혹은 AI?)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* SourceController;

	// 발동 주체
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UObject* SourceObject; // Rename?

	// 이펙트 인스턴스 ( 한번의 시퀀스로 여러 타겟 영향 위해 어레이, EI하나가 타겟 하나에 부착되는 형태 )
	UPROPERTY(/*VisibleAnywhere, BlueprintReadOnly*/)
	TArray<UDMSEffectInstance*> EIs;
	
	// 시퀀스에 진행에 필요한 플레이어의 의사 결정용 위젯들의 컨테이너
	UPROPERTY(/*VisibleAnywhere, BlueprintReadOnly*/)
	FDMSSelectorQueue SelectorQueue;

	// 시퀀스 플로우에 필요한 데이터들을 보관. ( 수치 같은 것의 변화 같이 )
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UDMSDataObjectSet* EIDatas;

	// 서로 체인되어 시퀀스 트리 진행중 GC 되는것을 막아주는 역할을 하게 하는 용도 
	// ++ 상위 시퀀스와 연관된 컨디션, 이펙트 같은 것을 구현 하기 위해
	UDMSSequence* ParentSequence;
	
	UDMSSequence* ChildSequence;

	void AttachChildSequence(UDMSSequence* iSeq);

	// Noncopy setter?
	UFUNCTION(BlueprintCallable)
	void SetTarget(TArray<TScriptInterface<IDMSEffectorInterface>> iTargets){ Targets = iTargets;}

	bool SetupWidgetQueue(TArray<UDMSConfirmWidgetBase*> iWidgets);

	template<typename FuncFinished, typename FuncCanceled >
	void RunWidgetQueue(FuncFinished&& iOnSelectorFinished, FuncCanceled&& iOnSelectorCanceled);

	void OnSequenceInitiate();
	void OnSequenceFinish();

	DECLARE_MULTICAST_DELEGATE(FOnSequenceStateChanged);
	//DECLARE_DYNAMIC_DELEGATE(FOnSequenceStateChanged_Dynamic);

protected:
	FOnSequenceStateChanged OnSequenceInitiated;
	FOnSequenceStateChanged OnSequenceFinished;

public:
	template<typename FuncInitiated>
	void AddToOnSequenceInitiated_Native(FuncInitiated&& iOnSequenceInitiated);

	template<typename FuncFinished>
	void AddToOnSequenceFinished_Native(FuncFinished&& iOnSequenceFinished);

	void AddToOnSequenceFinished(const FSimpleEventSignature& OnSequenceFinished);

	friend class UDMSSeqManager;
};

