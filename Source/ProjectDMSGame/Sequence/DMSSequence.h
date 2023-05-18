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


class UDMSSequenceStep;
class UDMSEffectInstance;
class UDMSDataObjectSet;
class UDMSEffectorInterface;
class UDMSEffectNode;
class UDMSConfirmWidgetBase;

UENUM(/*BlueprintType*/)
enum class EDMSSequenceState : uint8
{
	SS_Default UMETA(DisplayName = "Default"),
	SS_Canceled UMETA(DisplayName = "Canceled"),
	SS_Ignored UMETA(DisplayName = "After")
};

DECLARE_DYNAMIC_DELEGATE(FOnSequenceStateChanged_Signature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSequenceStateChanged_Dynamic);

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
		Progress = EDMSTimingFlag::T_Decision; //EDMSTimingFlag::T_Null;
		SequenceState = EDMSSequenceState::SS_Default;
		SourceObject = nullptr;
		ParentSequence = nullptr;
		ChildSequence = nullptr;
		// 
	}

	/**
	 * Current state of this sequence.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EDMSSequenceState SequenceState;

	/**
	 * Current timing of this sequence. DEPRECATED :: [Step]
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EDMSTimingFlag Progress;


	/**
	 * Effect that will be applied when the 'sequence is applied'.
	 * Sequence : EffectNode = 1 : 1 / Corresponds to the actual meaning of this sequence.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDMSEffectNode> OriginalEffectNode;

	/**
	 * Explicit targets of this sequence. Override EffectNode's preset target flag.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TScriptInterface<IDMSEffectorInterface>> Targets;

	/**
	 * The player or actor that triggers the sequence.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AActor> SourcePlayer;

	/**
	 * The object that triggers the sequence.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UObject> SourceObject; // IDMSEffectorInterface

	/**
	 * Effect instances (One EI attached to One target)
	 */
	UPROPERTY(/*VisibleAnywhere, BlueprintReadOnly*/)
	TArray<TObjectPtr<UDMSEffectInstance>> EIs;
	
	/**
	 * Container for widgets used by the player to make decisions during the progress of a sequence.
	 */
	UPROPERTY(/*VisibleAnywhere, BlueprintReadOnly*/)
	FDMSSelectorQueue SelectorQueue;

	/** 
	 * Data needed for sequence flow, such as 'Damage' by numerical values.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDMSDataObjectSet> EIDatas;

	/** 
	 * 서로 체인되어 시퀀스 트리 진행중 GC 되는것을 막아주는 역할을 하게 하는 용도
	 * ++ 상위 시퀀스와 연관된 컨디션, 이펙트 같은 것을 구현 하기 위해
	 */
	TObjectPtr<UDMSSequence> ParentSequence;
	TObjectPtr<UDMSSequence> ChildSequence;

	/**
	 * Attach child sequence.
	 * @param	iSeq						Attaching sequence.
	 */
	void AttachChildSequence(UDMSSequence* iSeq);

	/**
	 * Setter of Targets.
	 * @param	iTargets					New targets.
	 */
	UFUNCTION(BlueprintCallable)
	void SetTarget(TArray<TScriptInterface<IDMSEffectorInterface>> iTargets){ Targets = iTargets;}

	/**
	 * Setup owning widget queue with param widgets.
	 * @param	iWidgets					Widgets going to used in widget queue.
	 * @return	ture if setup was successful.
	 */
	bool SetupWidgetQueue(TArray<UDMSConfirmWidgetBase*> iWidgets);

	/**
	 * Run owning widget queue.
	 * @param	iOnSelectorFinished			Lambda parameter to be executed when the widget queue is successfully completed.
	 * @param	iOnSelectorCanceled			Lambda parameter to be executed when the widget queue is canceled.
	 */
	template<typename FuncFinished, typename FuncCanceled >
	void RunWidgetQueue(FuncFinished&& iOnSelectorFinished, FuncCanceled&& iOnSelectorCanceled);

	/**
	 * Executed when sequence is initiated.
	 */
	void OnSequenceInitiate();

	/**
	 * Executed when sequence is finished.
	 */
	void OnSequenceFinish();

	APlayerController* GetWidgetOwner();

	// ================================ //
	//		Delegates and binders.
	// ================================ //
protected:
	FSimpleMulticastEventSignature OnSequenceInitiated;
	FSimpleMulticastEventSignature OnSequenceFinished;

	FOnSequenceStateChanged_Dynamic OnSequenceInitiated_Dynamic;
	FOnSequenceStateChanged_Dynamic OnSequenceFinished_Dynamic;

public:
	template<typename FuncInitiated>
	void AddToOnSequenceInitiated_Native(FuncInitiated&& iOnSequenceInitiated);

	template<typename FuncFinished>
	void AddToOnSequenceFinished_Native(FuncFinished&& iOnSequenceFinished);

	UFUNCTION(BlueprintCallable)
	void AddToOnSequenceInitiated(const FOnSequenceStateChanged_Signature& iOnSequenceInitiated);

	UFUNCTION(BlueprintCallable)
	void AddToOnSequenceFinished(const FOnSequenceStateChanged_Signature& OnSequenceFinished);


// STEP 
public:
	TObjectPtr<UDMSSequenceStep> CurrentStep;
	// 이펙트 노드에서 스텝 리스트 정해놓고 생성?
	void InitializeSteps(const TArray<TSubclassOf<UDMSSequenceStep>>& StepClasses);

	void RunStepQueue();

	FGameplayTagContainer GenerateTagContainer();

protected:	
	TArray<TObjectPtr<UDMSSequenceStep>> InstancedSteps;
	
	friend class UDMSSequenceStep;
	friend class UDMSSeqManager;
};

