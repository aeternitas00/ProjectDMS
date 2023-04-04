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
 *	objectifying �� �´� ǥ���ΰ�?
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
 *	�ʰ� : �Ϲ������� ���ӿ��� �Ͼ�� �ϵ��� ��� '����Ʈ'�� ����̸� �� ����Ʈ�� ��� ���� �Ǵ����� ���� ������ ������Ʈȭ �ϴ� ���·� ����.
 *	���� �� � ����� �Ͼ�� �� ���� ������Ʈ���� �� ������ ������ �޾Ƽ� ���ڰ� ������ ������ �� �� �ְ� ����� ����. ( ����� ��ü�� �������� �޾Ƽ� üũ�ϴ� ���� )
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

	// ��� ����Ʈ�� ������ ���忡 ������ ��ġ�� ������ ���� �߿� ������ 'Ÿ�̹�'�� ��������� �ߵ� ��, ����, �� �������� ũ�� ���� �� ����.
	UPROPERTY(/*VisibleAnywhere, BlueprintReadOnly*/)
	EDMSTimingFlag Progress;

	// ����Ʈ ���� 1:1 ��Ī.
	// �� �������� �־� ��ǻ��� �ǹ����� �ش���.
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
	
	// �������� ���࿡ �ʿ��� �÷��̾��� �ǻ� ������ �������� �����̳�
	UPROPERTY(/*VisibleAnywhere, BlueprintReadOnly*/)
	FDMSSelectorQueue SelectorQueue;

	// ������ �÷ο쿡 �ʿ��� �����͵��� ����. ( ��ġ ���� ���� ��ȭ ���� )
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UDMSDataObjectSet* EIDatas;

	// ���� ü�εǾ� ������ Ʈ�� ������ GC �Ǵ°��� �����ִ� ������ �ϰ� �ϴ� �뵵 
	// ++ ���� �������� ������ �����, ����Ʈ ���� ���� ���� �ϱ� ����
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

