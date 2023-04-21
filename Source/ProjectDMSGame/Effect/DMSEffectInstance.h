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

#include "ProjectDMS.h"
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
	//...
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
class PROJECTDMSGAME_API UDMSEffectInstance : public UObject , public IDMSEffectorInterface
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

	struct FApplyDelegateCounter {
		FOnApplyCompleted CompletedDelegate;
		uint8 Index;
	};

	FOnWorkCompleted IteratingDelegate;

	/**
	 * ��ȣ �ߵ��� ����Ʈ�� ���� ü�� ���� ���� EI�� Reapply �� ��� ���� ���ɼ� �����ϹǷ� �帧 ������ '������'�� Ű�� ����.
	 */
	TMap<TObjectPtr<UDMSSequence>, FApplyDelegateCounter> OnApplyCompletedMap;

	/**
	 * Function for chained delegates. Will be bound to IteratingDelegate.
	 * @param	SourceSequence					Current applying sequence.
	 */
	UFUNCTION()
	void ApplyNextEffectDefinition(UDMSSequence* SourceSequence);

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
	TObjectPtr<AActor> SourceController;
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
	 * Apply effect to Outer.
	 * @return	Sequence						Current sequence.
	 * @return	OnApplyCompleted				Delegate executed when apply completed.
	 */
	FORCEINLINE void Apply(UDMSSequence* Sequence,const FResolveIteratingDelegate& OnApplyCompleted);

	/** 
	 * Sort of setup. Getting data from sequence or other source. (ex.Setting up Owning effect.)
	 * @param	iNode
	 * @param	iSet.
	 */
	FORCEINLINE void Initialize(UDMSEffectNode* iNode, UDMSDataObjectSet* iSet = nullptr);
	
	/**
	 * Sort of setup. Getting data from sequence or other source. (ex.Setting up Owning effect.)
	 * @param	iNode
	 * @param	iSeq.
	 */
	FORCEINLINE void Initialize(UDMSEffectNode* iNode, UDMSSequence* iSeq);

	/**
	 * Create new sequence from owning datas. ( node, datas .... )
	 * @param	SourceTweak						Source object of new sequence.
	 * @param	ChainingSequence				Parent sequence of creating sequence.
	 * @return	Created sequence.
	 */
	UDMSSequence* CreateSequenceFromNode(UObject* SourceTweak, UDMSSequence* ChainingSequence);

	// =========== INTERFACE FUNCTION =========== // 
	virtual UObject* GetObject() override { return this; } 
	virtual AActor* GetOwningPlayer() { return SourceController; }
	virtual void AttachEffectInstance(UDMSEffectInstance* EI) override;
	virtual bool OnNotifyReceived(TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*>& ResponsedObjects, bool iChainable,UDMSSequence* Seq, UObject* SourceTweak) override;
	// �⺻������ EI�� '� ȿ��' �� ��ü�� ��üȭ �ϱ� ���� ���� Ŭ�����̹Ƿ� ����Ʈ���� �����Ѵٴ� ������ ���� �̻��� ��.
	virtual UDMSEffectSet* GetOwningEffectSet(const FName& iSetName) override { return nullptr; }

	virtual void Serialize(FArchive& Ar) override;

	//friend FArchive& operator<<(FArchive& Ar, UDMSEffectInstance*& EI);
};

