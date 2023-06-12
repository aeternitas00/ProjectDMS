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
	SS_Ignored UMETA(DisplayName = "Ignored")
};

DECLARE_DYNAMIC_DELEGATE(FOnSequenceStateChanged_Signature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSequenceStateChanged_Dynamic);

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

protected:

	// === COMMENT TO REFERENCES OF SOURCE === //
	// Originally, IDMSEffectorInterface should be used.
	// But due to the inconvenience of TScriptInterface, it has been implemented using pointers of UObject-based classes instead.

	/**
	 * The player or actor that triggers the sequence.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AActor> SourcePlayer;

	/**
	 * The object that triggers the sequence.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UObject> SourceObject; 

	/**
	 * Explicit targets of this sequence. Override EffectNode's preset target flag.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TScriptInterface<IDMSEffectorInterface>> Targets;

public:
	// Default Initializer
	UDMSSequence();

	/**
	 * Current state of this sequence.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsPreviewSequence;

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
	 * ���� ü�εǾ� ������ Ʈ�� ������ GC �Ǵ°��� �����ִ� ������ �ϰ� �ϴ� �뵵
	 * ++ ���� �������� ������ �����, ����Ʈ ���� ���� ���� �ϱ� ����
	 */
	TObjectPtr<UDMSSequence> ParentSequence;
	TObjectPtr<UDMSSequence> ChildSequence;

	UFUNCTION(BlueprintCallable)
	UObject* GetSourceObject(bool ForceGetOriginal = false);

	UFUNCTION(BlueprintCallable)
	AActor* GetSourcePlayer(bool ForceGetOriginal = false);

	UFUNCTION(BlueprintCallable)
	TArray<TScriptInterface<IDMSEffectorInterface>> GetTargets();

	UFUNCTION(BlueprintCallable)
	bool SetSourceObject(UObject* NewSourceObject);

	UFUNCTION(BlueprintCallable)
	bool SetSourcePlayer(AActor* NewSourcePlayer);

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
	// ����Ʈ ��忡�� ���� ����Ʈ ���س��� ����?
	void InitializeSteps(const TArray<TSubclassOf<UDMSSequenceStep>>& StepClasses);

	void RunStepQueue();

	FGameplayTagContainer GenerateTagContainer();

protected:	
	TArray<TObjectPtr<UDMSSequenceStep>> InstancedSteps;
	
	friend class UDMSSequenceStep;
	friend class UDMSSeqManager;
};


template<typename FuncFinished, typename FuncCanceled >
void UDMSSequence::RunWidgetQueue(FuncFinished&& iOnSelectorFinished, FuncCanceled&& iOnSelectorCanceled)
{
	SelectorQueue.RunSelectors(
		std::forward<FuncFinished&&>(iOnSelectorFinished),
		std::forward<FuncCanceled&&>(iOnSelectorCanceled)
	);
}

template<typename FuncInitiated>
void UDMSSequence::AddToOnSequenceInitiated_Native(FuncInitiated&& iOnSequenceInitiated)
{
	OnSequenceFinished.AddLambda(iOnSequenceInitiated);
}

template<typename FuncFinished>
void UDMSSequence::AddToOnSequenceFinished_Native(FuncFinished&& iOnSequenceFinished)
{
	DMS_LOG_SIMPLE(TEXT("==== %s : ADD TO SEQ FINISHIED ===="), *GetName());

	if (OnSequenceFinished.IsBound()) {
		DMS_LOG_SIMPLE(TEXT("==== %s : SEQ FINISHIED HAS MUILTIPLE DELEGATES ===="), *GetName());
	}
	OnSequenceFinished.AddLambda(iOnSequenceFinished);

}
