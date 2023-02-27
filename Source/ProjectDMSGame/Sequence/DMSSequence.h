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

	// B-D-A 3�ܰ� ����
	UPROPERTY(/*VisibleAnywhere, BlueprintReadOnly*/)
	EDMSTimingFlag Progress;

	UPROPERTY(/*VisibleAnywhere, BlueprintReadOnly*/)
	UDMSEffectNode* OriginalEffectNode;

	// �̸� �����ϰ� �ߵ��� �ؾ��ϴ� ī����� ����?
	UPROPERTY(/*VisibleAnywhere, BlueprintReadOnly*/)
	TArray<TScriptInterface<IDMSEffectorInterface>> Targets; // ������ ���� / ��� �ʿ�

	// �ߵ� ������ ���� �÷��̾� (Ȥ�� AI?)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* SourceController;

	// �ߵ� ��ü
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UObject* SourceObject; // Rename?

	// ����Ʈ �ν��Ͻ� ( �ѹ��� �������� ���� Ÿ�� ���� ���� ���, EI�ϳ��� Ÿ�� �ϳ��� �����Ǵ� ���� )
	UPROPERTY(/*VisibleAnywhere, BlueprintReadOnly*/)
	TArray<UDMSEffectInstance*> EIs;
	
	UPROPERTY(/*VisibleAnywhere, BlueprintReadOnly*/)
	FDMSSelectorQueue SelectorQueue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UDMSDataObjectSet* EIDatas;

	// ���� ü�εǾ� ������ Ʈ�� ������ GC �Ǵ°��� �����ִ� ������ �ϰ� �ϴ� �뵵 
	// ++ ���� �������� ������ �����, ����Ʈ ���� ���� ���� �ϱ� ����
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

