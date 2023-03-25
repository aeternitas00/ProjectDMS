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

	// 시퀀스 최초 발생시 이곳에 저장
	// Root이 있으면 차일드 노드는 이 밑으로 계속 붙어나감
	// leaf 시퀀스가 모두 종료되고 root seq의 after 타이밍이 최종적으로 종료되면 클린업 실행.
	UPROPERTY()
	UDMSSequence* RootSequence;

	UPROPERTY()
	UDMSSequence* CurrentSequence;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UDMSDecisionWidget> DefaultYNWidget;

	void CleanupSequenceTree();

	// *FSeqHandle*/ 같은 시퀀스 핸들 구현? ( 넷코드 생각할때 다시 )
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