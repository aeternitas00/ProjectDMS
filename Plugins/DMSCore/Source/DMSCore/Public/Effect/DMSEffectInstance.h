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
class UDMSEIManagerComponent;

/**
 *	State flag of Effect instance.
 *	TODO :: make some flags and workflow for persistent effect
 */
UENUM(BlueprintType, Meta = (Bitflags,UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EDMSAEState : uint8
{
	AES_None			= 0x00			UMETA(Hidden),
	AES_NotifyClosed	= 0x01			UMETA(DisplayName = "Don't receive notify"), 
	AES_Persistent		= 0x01 << 1		UMETA(DisplayName = "Persistent"),
	//EIS_PendingKill		= 0x01 << 2		UMETA(DisplayName = "Pending to kill"),
	AES_MAX				= MAX_uint8		UMETA(Hidden),
	//...
};

ENUM_CLASS_FLAGS(EDMSAEState)

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
	TObjectPtr<ADMSActiveEffect> OwnerInstance;
	TObjectPtr<ADMSSequence> SourceSequence;

	TObjectPtr<UDMSEffectNode> ApplyingEffect;
	uint8 CurrentEDIndex;

	uint8 ExecutedOptionNum;
	FOnOptionCompleted EffectOptionCompleted;
	TObjectPtr<UDMSEffectDefinition> CurrentDef;

public:
	FOnExecuteCompleted IteratingDelegate;
	FOnApplyCompleted CompletedDelegate;
	
public:
	void SetupWorker(ADMSSequence* iSequence, ADMSActiveEffect* iEI, const FOnApplyCompleted& iOnApplyCompleted);
	
	UFUNCTION()
	void ApplyNextEffectDef(bool PrevSucceeded);

	UFUNCTION()
	void OnEffectOptionCompleted(UDMSEffectOption* CompletedOption);
};

/**
 * 	========================================
 *
 *	Active effect : Class for objectification of 'effect'. Supports effect lifetime management, notify response, etc.
 *
 *	AE Will be attached to each target and it'll be each AE's Owner.
 *	One EI per One Affected Object.
 *
 *	========================================
 *	Is it good enough?
 */

UCLASS(BlueprintType)
class DMSCORE_API ADMSActiveEffect : /*public ADMSSpawnableBase*/public AInfo, public IDMSEffectorInterface
{
	GENERATED_BODY()
	
public:
	ADMSActiveEffect();
		
protected:

	/** 
	 * State flag of Active Effect.For preview or persistant things.
	 * [replicates for clients' ui]
	 */
	UPROPERTY(Replicated)
	EDMSAEState CurrentState;

	/**
	 * Managing attributes for active effect
	 * [replicates for clients' ui]
	 */
	UPROPERTY(Replicated)
	TObjectPtr<UDMSAttributeComponent> AttributeComponent;

	/**
	* Managing attributes for active effect
	* [replicates for clients' ui]
	*/
	UPROPERTY(Replicated)
	TObjectPtr<UDMSEIManagerComponent> EffectManagerComponent;

	// Hard ref for workers
	TArray<TObjectPtr<UDMSEffectApplyWorker>> ApplyWorkers;

	FSimpleMulticastDelegate OnApplyComplete_Native;

public:

	/**
	 * Getter of CurrentState
	 */
	FORCEINLINE EDMSAEState GetCurrentState() { return CurrentState; }

	/**
	 * Setter of CurrentState
	 */
	FORCEINLINE void ToggleEIState(const EDMSAEState& NewState) { CurrentState ^= NewState; }

	/**
	 * Sources of effect
	 */
	UPROPERTY()
	TObjectPtr<AActor> SourcePlayer;
	UPROPERTY()
	TObjectPtr<AActor> SourceObject;

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
	void Apply(ADMSSequence* Sequence, const FResolveIteratingDelegate& OnApplyCompleted);

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
	AActor* GetApplyTarget();

	/**
	 * On effect apply complete
	 */
	virtual void OnApplyComplete();

	//void OnSequenceComplete();

	virtual void Initialize(UDMSEffectNode* iNode,const EDMSAEState& InitialState);
	/** 
	 * Sort of setup. Getting data from sequence or other source. (ex.Setting up Owning effect.)
	 * @param	iSet.
	 */
	void SetupDatas(UDMSDataObjectSet* iSet=nullptr);
	
	/**
	 * Sort of setup. Getting data from sequence or other source. (ex.Setting up Owning effect.)
	 * @param	iNode
	 * @param	iSeq.
	 */
	void InheritSequenceDatas(ADMSSequence* iSeq);

	/**
	 * Create new sequence from owning datas. ( node, datas .... )
	 * @param	SourceTweak						Source object of new sequence.
	 * @param	ChainingSequence				Parent sequence of creating sequence.
	 * @return	Created sequence.
	 */
	ADMSSequence* CreateApplyingSequence(AActor* SourceTweak, ADMSSequence* ChainingSequence);

	/**
	 * Handle notifing data
	 * @param	iChainable						Flag whether the user can chain on this effect. (forced trigger effects will ignore this and execute the notification)
	 * @param	Seq								Current sequence ( notifying info )
	 * @param	SourceTweak						Indicating who the active effect responsed to when triggered (typically the owner of the AE).
	 * @out		ResponsedObjects				When the response is successful, this AE and the owner (varies depending on Source Tweak) are registered in the parameter map. 
												This is done to inform external objects using notification (typically a notify manager) 
												about which AE responsed to which game actor.
	 * @return	true if Response success.
	 */
	bool ReceiveNotify(TMultiMap<TScriptInterface<IDMSEffectorInterface>, ADMSActiveEffect*>& ResponsedObjects, bool iChainable,ADMSSequence* Seq, AActor* SourceTweak=nullptr);

	/**
	 *	Add a param lambda expression to the native delegate to be executed after this active effect has been applied.
	 *	@param	iOnSequenceFinished				lambda expression to be added.
	 */
	template<typename FuncFinished>
	void AddToOnApplyComplete_Native(FuncFinished&& iOnSequenceFinished);

	/**
	 *	Safe detaching from its owner.
	 */
	void DetachFromOwner();

	FSimpleMulticastDelegate OnDetach;

	/**
	 *	Safely clean up the workers used in AE applying.
	 */
	void CleanupWorker(UDMSEffectApplyWorker* Worker);

	// =========== INTERFACE FUNCTION =========== // 
	
	virtual AActor* GetOwningPlayer_Implementation() { return SourcePlayer; }
	
	virtual void Serialize(FArchive& Ar) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

template<typename FuncFinished>
void ADMSActiveEffect::AddToOnApplyComplete_Native(FuncFinished&& iOnSequenceFinished)
{
	OnApplyComplete_Native.AddLambda(iOnSequenceFinished);
}