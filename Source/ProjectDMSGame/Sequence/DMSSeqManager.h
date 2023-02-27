// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Common/DMSCommons.h"
#include "GameModes/DMSGameMode.h"
#include "UObject/NoExportTypes.h"
#include "DMSSeqManager.generated.h"

class UDMSEffectDefinition;
class UDMSEffectNode;
class UDMSDataObjectSet;
class UDMSSequence;
class UDMSEffectorInterface;

/**
 * Managemnet seq. flow. tossing to effect handler.
 */
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FOnSelectorFinished, UDMSDataObjectSet*, Datas);
UCLASS(ClassGroup = (Sequence))
class PROJECTDMSGAME_API UDMSSeqManager : public UObject // or ActorComponent to attach GM
{
	GENERATED_BODY()

private:
	//UPROPERTY()
	//TArray<UDMSSequence*> SequenceFlow;

protected:
	void RunSequence(UDMSSequence* Sequence);

public:
	UDMSSeqManager();

	// ������ ���� �߻��� �̰��� ����
	// Root�� ������ ���ϵ� ���� �� ������ ��� �پ��
	// leaf �������� ��� ����ǰ� root seq�� after Ÿ�̹��� ���������� ����Ǹ� Ŭ���� ����.
	UPROPERTY()
	UDMSSequence* RootSequence;

	// *FSeqHandle*/ ���� ������ �ڵ� ����? ( ���ڵ� �����Ҷ� �ٽ� )
	UFUNCTION(BlueprintCallable)
	UDMSSequence* RequestCreateSequence(
		UObject* SourceObject, 
		AActor* SourceController,
		UDMSEffectNode* EffectNode, 
		TArray<TScriptInterface<IDMSEffectorInterface>> Targets/* = TArray<TScriptInterface<IDMSEffectorInterface>>()*/, 
		UDMSDataObjectSet* Datas = nullptr, 
		UDMSSequence* ParentSequence = nullptr,
		const EDMSTimingFlag & RelationFlag = EDMSTimingFlag::T_Null
	);

	//UFUNCTION()
	//void OnSelectorCompleted(UDMSSequence* Sequence);

	FOnSelectorFinished OnSelectorFinished;

	friend class UDMSSequence;
};

//DEPRECATED
//#define GetDMSSequenceManager() ( GetDMSGameMode()!=nullptr ? GetDMSGameMode()->GetSeqManager() : nullptr )