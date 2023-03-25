// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *	"Handling Sequences"
 *
 *	========================================
 */

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
class UDMSDecisionWidget;

/**
 *	========================================
 * 
 *	Managemnet seq. flow. tossing to effect handler.
 * 
 *	========================================
 */
UCLASS(ClassGroup = (Sequence))
class PROJECTDMSGAME_API UDMSSeqManager : public UObject // or ActorComponent to attach GM
{
	GENERATED_BODY()

private:
	//UPROPERTY()
	//TArray<UDMSSequence*> SequenceFlow;

protected:
	void ApplySequence(UDMSSequence* Sequence);
	void CompleteSequence(UDMSSequence* Sequence);
public:
	UDMSSeqManager();

	// ������ ���� �߻��� �̰��� ����
	// Root�� ������ ���ϵ� ���� �� ������ ��� �پ��
	// leaf �������� ��� ����ǰ� root seq�� after Ÿ�̹��� ���������� ����Ǹ� Ŭ���� ����.
	UPROPERTY()
	UDMSSequence* RootSequence;

	UPROPERTY()
	UDMSSequence* CurrentSequence;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UDMSDecisionWidget> DefaultYNWidget;

	void CleanupSequenceTree();

	// *FSeqHandle*/ ���� ������ �ڵ� ����? ( ���ڵ� �����Ҷ� �ٽ� )
	UFUNCTION(BlueprintCallable)
	UDMSSequence* RequestCreateSequence(
		UObject* SourceObject, 
		AActor* SourceController,
		UDMSEffectNode* EffectNode, 
		TArray<TScriptInterface<IDMSEffectorInterface>> Targets/* = TArray<TScriptInterface<IDMSEffectorInterface>>()*/, 
		UDMSDataObjectSet* Datas = nullptr, 
		UDMSSequence* ParentSequence = nullptr
	);

	UFUNCTION(BlueprintCallable)
	void RunSequence(UDMSSequence* iSeq);
	
	int GetDepth(UDMSSequence* iSeq);
	DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FOnSelectorFinished, UDMSDataObjectSet*, Datas);

	FOnSelectorFinished OnSelectorFinished;

	friend class UDMSSequence;
};

//DEPRECATED
//#define GetDMSSequenceManager() ( GetDMSGameMode()!=nullptr ? GetDMSGameMode()->GetSeqManager() : nullptr )