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

	// 상호 발동형 이펙트를 통해 체인 내에 같은 EI가 Reapply 될 경우 꼬일 가능성 존재하므로 흐름 단위인 '시퀀스'를 키로 잡음.
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
	// 기본적으로 EI는 '어떤 효과' 그 자체를 객체화 하기 위해 만든 클래스이므로 이펙트셋을 소유한다는 개념은 조금 이상한 듯.
	virtual UDMSEffectSet* GetOwningEffectSet(const FName& iSetName) override { return nullptr; }
};

