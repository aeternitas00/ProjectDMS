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
UCLASS(Blueprintable, ClassGroup = (Sequence))
class PROJECTDMSGAME_API UDMSSeqManager : public UActorComponent // or ActorComponent to attach GM
{
	GENERATED_BODY()

protected:
	void ApplySequence(UDMSSequence* Sequence);
	void CompleteSequence(UDMSSequence* Sequence);	
	void CleanupSequenceTree();
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

	UFUNCTION(BlueprintImplementableEvent)
	void OnSequenceTreeInitiated();

	UFUNCTION(BlueprintImplementableEvent)
	void OnSequenceTreeCompleted();

	// *FSeqHandle*/ ���� ������ �ڵ� ����? ( ���ڵ� �����Ҷ� �ٽ� )
	UFUNCTION(BlueprintCallable)
	UDMSSequence* RequestCreateSequence(
		UObject* SourceObject, 
		AActor* SourceController,
		UDMSEffectNode* EffectNode, 
		TArray<TScriptInterface<IDMSEffectorInterface>> Targets, 
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