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
 *	EffectInstance : ~~
 * 
 *	Will be attached to each target
 *	One EI per One Affected Object
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
	EDMSEIState CurrentState;

	UPROPERTY()
	TArray<UDMSEffectInstance*> SubEI;
public:
	AActor* SourceController;
	UObject* SourceObject;

	UPROPERTY()
	UDMSDataObjectSet* DataSet; 
	UPROPERTY()
	UDMSEffectNode* EffectNode;

public:
	FORCEINLINE void Apply();
	FORCEINLINE void Initialize(UDMSEffectNode* iNode, UDMSDataObjectSet* iSet =nullptr);
	FORCEINLINE void Initialize(UDMSEffectNode* iNode, UDMSSequence* iSeq);
	FORCEINLINE void ChangeEIState(const EDMSEIState& NewState) { CurrentState = NewState; }

	virtual UObject* GetObject() override { return this; } 
	virtual void AttachEffectInstance(UDMSEffectInstance* EI) override;
	virtual void OnNotifyReceived(UDMSSequence* Seq, UObject* SourceTweak) override;
	// �⺻������ EI�� '� ȿ��' �� ��ü�� ��üȭ �ϱ� ���� ���� Ŭ�����̹Ƿ� ����Ʈ���� �����Ѵٴ� ������ ���� �̻��� ��.
	virtual UDMSEffectSet* GetOwningEffectSet(const FName& iSetName) override { return nullptr; }
};

