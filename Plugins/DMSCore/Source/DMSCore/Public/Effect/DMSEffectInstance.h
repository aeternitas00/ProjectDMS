// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *	Effect Instance
 *
 *	========================================
 */

#include "DMSCoreIncludes.h"
#include "Effect/DMSEffectorInterface.h"
#include "Common/DMSCommonDelegates.h"
#include "Common/DMSCommons.h"
#include "Effect/DMSEffectDefinition.h"
#include "UObject/NoExportTypes.h"
#include "DMSEffectInstance.generated.h"

class ADMSPlayerController;
class UDMSEffectNode;

/**
 *	State flag of Effect instance.
 *	TODO :: make some flags and workflow for persistent effect
 */
UENUM()
enum class EDMSEIState : uint8
{
	EIS_Default UMETA(DisplayName = "Default"),
	EIS_Pending UMETA(DisplayName = "Pending to apply"), // Prevent self notifing 
	EIS_Persistent UMETA(DisplayName = "Persistent"),
	EIS_PendingKill UMETA(DisplayName = "Pending to kill"),
	//EIS_Preview UMETA(DisplayName = "Preview Dummy"),
	//...
};

/**
 * 	========================================
 *
 *	서버사이드에서 Apply Step 로직 처리를 할 때 사용 할 단순 워커
 * 
 *	========================================
 *	
 */
UCLASS()
class DMSCORE_API UDMSEffectApplyWorker : public UObject
{
	GENERATED_BODY()

private:
	TObjectPtr<UDMSEffectInstance> OwnerInstance;
	TObjectPtr<UDMSSequence> SourceSequence;

	TObjectPtr<UDMSEffectNode> ApplyingEffect;
	uint8 CurrentEDIndex;

	uint8 ExecutedOptionNum;
	FOnOptionCompleted EffectOptionCompleted;
	TObjectPtr<UDMSEffectDefinition> CurrentDef;

public:
	FOnExecuteCompleted IteratingDelegate;
	FOnApplyCompleted CompletedDelegate;
	
public:
	void SetupWorker(UDMSSequence* iSequence, UDMSEffectInstance* iEI, const FOnApplyCompleted& iOnApplyCompleted);
	
	UFUNCTION()
	void ApplyNextEffectDef(bool PrevSucceeded);

	UFUNCTION()
	void OnEffectOptionCompleted(UDMSEffectOption* CompletedOption);
};

/**
 * 	========================================
 *
 *	EffectInstance : Class for objectification of 'effect'. Supports effect lifetime management, notify response, etc.
 *
 *	EI Will be attached to each target and it'll be each EI's Outer.
 *	One EI per One Affected Object.
 *
 *	========================================
 *	Is it good enough?
 */

UCLASS(BlueprintType)
class DMSCORE_API UDMSEffectInstance : public UObject/*UActorComponent*/, public IDMSEffectorInterface
{
	GENERATED_BODY()
	
public:
	UDMSEffectInstance();
		
protected:

	/** 
	 * State flag of Effect instance.For preview or persistant things.
	 */
	UPROPERTY()
	EDMSEIState CurrentState;

	/**
	 * Effects that affect to effect. (modifying values)
	 */
	UPROPERTY()
	TArray<TObjectPtr<UDMSEffectInstance>> SubEI;

	// Hard ref for workers
	TArray<TObjectPtr<UDMSEffectApplyWorker>> ApplyWorkers;

public:

	/**
	 * Getter of CurrentState
	 */
	FORCEINLINE EDMSEIState GetCurrentState() { return CurrentState; }

	/**
	 * Setter of CurrentState
	 */
	FORCEINLINE void ChangeEIState(const EDMSEIState& NewState) { CurrentState = NewState; }

	/**
	 * Sources of effect
	 */
	TObjectPtr<AActor> SourcePlayer;
	TObjectPtr<UObject> SourceObject;

	/**
	 * Effect node that effect instance will run.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UDMSEffectNode> EffectNode;

	/**
	 * Data needs to running effect node.
	 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UDMSDataObjectSet> DataSet;

public:
	/**
	 * Apply effect with effect node.
	 * @return	Sequence						Current sequence.
	 * @return	OnApplyCompleted				Delegate executed when apply completed.
	 */
	void Apply(UDMSSequence* Sequence, const FResolveIteratingDelegate& OnApplyCompleted);

	/**
	 * Get apply target.
	 * Will be used in ED->Work function.
	 */
	IDMSEffectorInterface* GetApplyTargetInterface();

	/**
	* Get apply target.
	* Will be used in ED->Work function.
	*/
	UFUNCTION(BlueprintCallable)
	UObject* GetApplyTarget();

	/**
	 * Setting the state of EffectInstance to pending kill (it will be cleaned up at the end of the sequence tree).
	 */
	void SetToPendingKill();

	/** 
	 * Sort of setup. Getting data from sequence or other source. (ex.Setting up Owning effect.)
	 * @param	iNode
	 * @param	iSet.
	 */
	void Initialize(UDMSEffectNode* iNode, UDMSDataObjectSet* iSet = nullptr);
	
	/**
	 * Sort of setup. Getting data from sequence or other source. (ex.Setting up Owning effect.)
	 * @param	iNode
	 * @param	iSeq.
	 */
	void Initialize(UDMSEffectNode* iNode, UDMSSequence* iSeq);

	/**
	 * Create new sequence from owning datas. ( node, datas .... )
	 * @param	SourceTweak						Source object of new sequence.
	 * @param	ChainingSequence				Parent sequence of creating sequence.
	 * @return	Created sequence.
	 */
	UDMSSequence* CreateSequenceFromNode(UObject* SourceTweak, UDMSSequence* ChainingSequence);


	// =========== INTERFACE FUNCTION =========== // 
	// 
	virtual AActor* GetOwningPlayer() { return SourcePlayer; }
	virtual void AttachEffectInstance(UDMSEffectInstance* EI) override;
	virtual bool OnNotifyReceived(TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*>& ResponsedObjects, bool iChainable,UDMSSequence* Seq, UObject* SourceTweak) override;
	// 기본적으로 EI는 '어떤 효과' 그 자체를 객체화 하기 위해 만든 클래스이므로 이펙트셋을 소유한다는 개념은 조금 이상한 듯.
	//virtual UDMSEffectSet* GetOwningEffectSet(const FGameplayTag& iSetName) override { return nullptr; }
	virtual void Serialize(FArchive& Ar) override;

	//friend FArchive& operator<<(FArchive& Ar, UDMSEffectInstance*& EI);
};

