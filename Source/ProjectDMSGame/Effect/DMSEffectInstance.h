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
	EIS_Pending UMETA(DisplayName = "Pending to apply"),
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


UCLASS()
class PROJECTDMSGAME_API UDMSEffectInstance : public UObject , public IDMSEffectorInterface
{
	GENERATED_BODY()
	
public:
	UDMSEffectInstance():CurrentState(EDMSEIState::EIS_Default){}
		
protected:
	// State flag of Effect instance. For preview or persistant things.
	EDMSEIState CurrentState;

	// Effects that affect to effect. ( modifying values )
	//UPROPERTY()
	TArray<UDMSEffectInstance*> SubEI;

	struct FApplyDelegateCounter {
		FOnApplyCompleted CompletedDelegate;
		uint8 Index;
		FOnWorkCompleted IteratingDelegate;
	};

	// ��ȣ �ߵ��� ����Ʈ�� ���� ü�� ���� ���� EI�� Reapply �� ��� ���� ���ɼ� �����ϹǷ� �帧 ������ '������'�� Ű�� ����.
	TMap<UDMSSequence*, FApplyDelegateCounter> OnApplyCompletedMap;

public:
	// Sources of effect
	AActor* SourceController;
	UObject* SourceObject;

	// Effect node that effect instance will run.
	UPROPERTY()
	UDMSEffectNode* EffectNode;

	// Data needs to running effect node.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UDMSDataObjectSet* DataSet; 

public:
	// Apply effect to Outer.

	FORCEINLINE void Apply(UDMSSequence* Sequence,const FResolveIteratingDelegate& OnApplyCompleted);

	// Sort of setup. Getting data from sequence or other source. ( ex. Setting up Owning effect. )
	FORCEINLINE void Initialize(UDMSEffectNode* iNode, UDMSDataObjectSet* iSet = nullptr);
	FORCEINLINE void Initialize(UDMSEffectNode* iNode, UDMSSequence* iSeq);

	FORCEINLINE void ChangeEIState(const EDMSEIState& NewState) { CurrentState = NewState; }

	UDMSSequence* CreateSequenceFromNode(UObject* SourceTweak, UDMSSequence* ChainingSequence);

	UFUNCTION()
	void ApplyNextEffectDefinition(UDMSSequence* SourceSequence);

	// =========== INTERFACE FUNCTION =========== // 
	virtual UObject* GetObject() override { return this; } 
	virtual AActor* GetOwningPlayer() { return SourceController; }
	virtual void AttachEffectInstance(UDMSEffectInstance* EI) override;
	virtual bool OnNotifyReceived(TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*>& ResponsedObjects, bool iChainable,UDMSSequence* Seq, UObject* SourceTweak) override;
	// �⺻������ EI�� '� ȿ��' �� ��ü�� ��üȭ �ϱ� ���� ���� Ŭ�����̹Ƿ� ����Ʈ���� �����Ѵٴ� ������ ���� �̻��� ��.
	virtual UDMSEffectSet* GetOwningEffectSet(const FName& iSetName) override { return nullptr; }
};

