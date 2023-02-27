// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Common/DMSCommons.h"
#include "Selector/DMSSelectorQueue.h"
#include "UObject/NoExportTypes.h"
#include "DMSSequence.generated.h"

class UDMSEffectInstance;
class UDMSDataObjectSet;
class UDMSEffectorInterface;
class UDMSEffectNode;
class UDMSEffectElementSelectorWidget;

UCLASS(ClassGroup = (Sequence))
class UDMSSequence : public UObject
{
	GENERATED_BODY()

public:
	//static const uint16 IDX_NONE = 0xFFFF;

	// Default Initializer
	UDMSSequence( ) { 
		Progress = EDMSTimingFlag::T_Before; //EDMSTimingFlag::T_Null;
		bIsActive=false;
		SourceObject = nullptr;
		ParentNode = nullptr;
		BeforeNode = nullptr;
		DuringNode = nullptr;
		AfterNode= nullptr;
		// 
	}

	UPROPERTY(/*VisibleAnywhere, BlueprintReadOnly*/)
	bool bIsActive;

	// B-D-A 3단계 구성
	UPROPERTY(/*VisibleAnywhere, BlueprintReadOnly*/)
	EDMSTimingFlag Progress;

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
	
	UPROPERTY(/*VisibleAnywhere, BlueprintReadOnly*/)
	FDMSSelectorQueue SelectorQueue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UDMSDataObjectSet* EIDatas;

	// 서로 체인되어 시퀀스 트리 진행중 GC 되는것을 막아주는 역할을 하게 하는 용도 
	// ++ 상위 시퀀스와 연관된 컨디션, 이펙트 같은 것을 구현 하기 위해
	UPROPERTY()
	UDMSSequence* ParentNode;
	UPROPERTY()
	UDMSSequence* BeforeNode;
	UPROPERTY()
	UDMSSequence* DuringNode;
	UPROPERTY()
	UDMSSequence* AfterNode;

	FORCEINLINE void SetActive(const bool& iAct) { bIsActive = iAct;}
	FORCEINLINE void AddToSelectorQueue(UDMSEffectElementSelectorWidget* iWidget) { SelectorQueue.AddSelector(iWidget); }
	void InitializeSelectorQueue(){ SelectorQueue.Initialize(this); }

	template<typename FuncFinished, typename FuncCanceled >
	void RunSelectorQueue(FuncFinished&& iOnSelectorFinished, FuncCanceled&& iOnSelectorCanceled);
};

