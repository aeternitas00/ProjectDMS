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
#include "Effect/DMSEffectDefinition.h"
#include "Effect/DMSEffectorInterface.h"
#include "Common/DMSCommons.h"
#include "UObject/NoExportTypes.h"
#include "DMSEffectInstance.generated.h"

class ADMSPlayerController;

/**
 *	State flag of Effect instance.
 *	TODO :: make some flags and workflow for persistent effect
 */
UENUM()
enum class EDMSEIState : uint8
{
	EIS_Default UMETA(DisplayName = "Default"),
	EIS_Pending UMETA(DisplayName = "Pending to apply"),
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
	FORCEINLINE void Apply();

	// Sort of setup. Getting data from sequence or other source. ( ex. Setting up Owning effect. )
	FORCEINLINE void Initialize(UDMSEffectNode* iNode, UDMSDataObjectSet* iSet = nullptr);
	FORCEINLINE void Initialize(UDMSEffectNode* iNode, UDMSSequence* iSeq);

	FORCEINLINE void ChangeEIState(const EDMSEIState& NewState) { CurrentState = NewState; }

	// =========== INTERFACE FUNCTION =========== // 
	virtual UObject* GetObject() override { return this; } 
	virtual void AttachEffectInstance(UDMSEffectInstance* EI) override;
	virtual void OnNotifyReceived(bool iChainable,UDMSSequence* Seq, UObject* SourceTweak) override;
	// 기본적으로 EI는 '어떤 효과' 그 자체를 객체화 하기 위해 만든 클래스이므로 이펙트셋을 소유한다는 개념은 조금 이상한 듯.
	virtual UDMSEffectSet* GetOwningEffectSet(const FName& iSetName) override { return nullptr; }
};

