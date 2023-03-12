// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *	"Definition of Effect" for Project DMS
 *
 * =========================================
 */

#include "ProjectDMS.h"
#include "UObject/NoExportTypes.h"
#include "Sequence/DMSSeqManager.h"
#include "Common/DMSConditionContainer.h"
#include "Effect/DMSEffectElementSelectorWidget.h"
#include "DMSEffectDefinition.generated.h"




class UDMSDataObjectSet;

/**
 *
 * 	========================================
 *
 *	Effect Definition :: Minimal functor-like Object. Definition of what 'Effect' should do.
 *	This class represents a single line or word of effect text. ( "Deal 3 damage" )
 *	It is designed to be instanced in "EffectNode".
 *
 * =========================================
 *
 *	BP에서 기본적인 것들 외의 커스텀 이펙트 원형 생성 가능하게
 *	Effect가 가져야할 기본적인 값 들은 프로퍼티로, 수치, 타겟등 유저 조작과 연계된 부분은 셀렉터를 사용하게 함.
 *	셀렉터가 없을 경우 -> 수치가 범위 기반이라면 랜덤 값, 타겟은 시전자가 되도록 구현
 *	Ex) BP_EffectTag_DealDamage ?
 * 
 */
UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew, Abstract, ClassGroup = (Effect))
class UDMSEffectDefinition : public UObject
{
	GENERATED_BODY()

public:
	// Discuss
	UPROPERTY()
	FName Keyword; // 단어 단위이므로 fstring 대신 fname

public:
	UDMSEffectDefinition() { /* Keyword = New Name */ };

	// 실행할 함수의 중복을 줄이기 위해 CardDefinition - DMSEffect에 실행부인 (Work)를 두는 것으로 하였음.
	// 이것은 카드 종류 하나에 실행부 하나만을 두기 까지 압축하는 것을 의도
	// 성공 여부 파악, 체인 확장을 위한 핸들 리턴?
	UFUNCTION(BlueprintNativeEvent)
	void Work(UDMSEffectInstance* iEI); // temp
	virtual void Work_Implementation(UDMSEffectInstance* iEI) {};

	// ===== For selector features ===== //
	// 셀렉터를 쓰는 디피니션을 차일드 클래스로 하나 빼는게 좋을까?
	// 리스트 구할 방법을 미리 해놓는 식으로.. TMap<Name, ENum ( Numeric, Object, String ...) > CandidatesFlag 같이...

	// SelectorDefinition??????
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CardEffect)
	TArray<UDMSEffectElementSelectorWidget*> Selectors; // sort of option

	// NOTE ) 셀렉터한테 "Name,Type"의 명세를 넣어주는게 더 좋은 설계인가?
	UFUNCTION(BlueprintNativeEvent)
	bool GetCandidates(UDMSSequence* iSeq, TArray<UDMSDataObject*>& outDataObj); // temp
	virtual bool GetCandidates_Implementation(UDMSSequence* iSeq, TArray<UDMSDataObject*>& outDataObj) { return false; };

	void CreateSelectors_(APlayerController* WidgetOwner, UDMSSequence* inSequence);
};


/**
 *
 * 	========================================
 *
 *	Effect Node :: Customizable effect definition container.
 *	This class represents multiple line of effect text. 
 *	( ex. Deal 3 Damage and heal self that much. Then, if target HP is lower than ~ : Deal 2 Damage. )
 *	Pseudo tree architecture for sub-effect and branchs.
 *
 *	=========================================
 *	
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Effect), DefaultToInstanced, EditInlineNew, meta = (DisplayName = "Effect Node Base"))
class PROJECTDMSGAME_API UDMSEffectNode : public UObject
{
	GENERATED_BODY()

public:
	UDMSEffectNode():NodeKeyword(""),PresetTargetFlag(EDMSPresetTargetFlag::PTF_Self), bIsChainableEffect(true), bForced(false) {}
	// 이펙트 노드에 대한 대표 키워드
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = EffectNode)
	FName NodeKeyword;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = EffectNode)
	EDMSPresetTargetFlag PresetTargetFlag;

	// 노드 실행시 발생할 코스트 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = EffectNode)
	UDMSEffectSet* EffectCost;

	// Effect's activatable timing
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = EffectNode)
	FDMSConditionContainer Conditions;

	// Actual effects that activatable in that timing
	// Works in order to 0~n
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = EffectNode, Instanced)
	TArray<UDMSEffectDefinition*> EffectDefinitions;

	// Effect's activatable timing
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = EffectNode)
	FDMSConditionContainer AdvanceConditions;

	// 이펙트 발동 후 조건 체크를 한번 더 한다음 발동하는 서브 이펙트에 대한 구현?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = EffectNode)
	UDMSEffectNodeWrapper* ChildEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = EffectNode)
	bool bIsChainableEffect;

	// 효과 발동에 대한 선택권이 있는 효과인가? == true : 조건에 맞으면 강제 발동 / false : 조건에 맞으면 효과 발동에 대한 Y/N UI 띄우는 형태로
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = EffectNode)
	bool bForced;

	bool IsContainKeyword(const FName& iKeyword);
	void CreateSelectors(APlayerController* WidgetOwner, UDMSSequence* inSequence);
	//void RunSelectors(APlayerController* WidgetOwner, UDMSSequence* inSequence, void (UDMSSeqManager::*OnSelectorsFinished)(UDMSSequence*));
};

/** 
 * 	========================================
 *
 *	Effect Node Wrapper :: Wrapper for exposing node to Blueprint in various ways.
 *
 *	=========================================
 */
UCLASS(Abstract,BlueprintType, Const, DefaultToInstanced, EditInlineNew, ClassGroup = (Condition))
class PROJECTDMSGAME_API UDMSEffectNodeWrapper : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure,BlueprintCallable, meta = (DisplayName = "Get Effect Node"))
	UDMSEffectNode* GetEffectNodeBP() { return GetEffectNode(); }
	virtual UDMSEffectNode* GetEffectNode(){return nullptr;}
	virtual bool IsContainKeyword(const FName& iKeyword) {return false;}
	virtual void CreateSelectors(APlayerController* WidgetOwner, UDMSSequence* inSequence){}
	//virtual void RunSelectors(APlayerController* WidgetOwner, UDMSSequence* inSequence, void (UDMSSeqManager::*OnSelectorsFinished)(UDMSSequence*)) {}
};

/**
 * 	========================================
 *
 *	Wrapper_Manual :: Instancing EffectNode while in outer definition.
 *	ex ) When a node using Deal X Damage Effectdef. ( X is property of Effect def ).
 *	It'll make reusability of Effect def.
 * 
 *	=========================================
 */
UCLASS(BlueprintType, ClassGroup = (Effect), meta = (DisplayName = "Make Effect Node in def"))
class PROJECTDMSGAME_API UDMSEffectNodeWrapper_Manual : public UDMSEffectNodeWrapper
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Instanced, Category = Effect)
	UDMSEffectNode* EffectNode;
	virtual UDMSEffectNode* GetEffectNode() { return EffectNode; }
	bool IsContainKeyword(const FName& iKeyword){return EffectNode->IsContainKeyword(iKeyword);}
	virtual void CreateSelectors(APlayerController* WidgetOwner, UDMSSequence* inSequence){ EffectNode->CreateSelectors(WidgetOwner, inSequence); }
	//virtual void RunSelectors(APlayerController* WidgetOwner, UDMSSequence* inSequence, void (UDMSSeqManager::*OnSelectorsFinished)(UDMSSequence*)) { EffectNode->RunSelectors(WidgetOwner, inSequence,OnSelectorsFinished);}
};

/**
 * 	========================================
 *
 *	Wrapper_Preset :: Use TSubclassOf<Node> and CDO.
 *	For Non-variable static Effects.
 *
 *	=========================================
 */
UCLASS(BlueprintType, ClassGroup = (Effect), meta = (DisplayName = "Use BP Effect Node"))
class PROJECTDMSGAME_API UDMSEffectNodeWrapper_Preset : public UDMSEffectNodeWrapper
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = Effect, meta = (DisplayName = "Effect Node Class"))
	TSubclassOf<UDMSEffectNode> EffectNode; 
	virtual UDMSEffectNode* GetEffectNode() { return EffectNode.GetDefaultObject(); }
	bool IsContainKeyword(const FName& iKeyword) { return EffectNode.GetDefaultObject()->IsContainKeyword(iKeyword); }
	virtual void CreateSelectors(APlayerController* WidgetOwner, UDMSSequence* inSequence){ EffectNode.GetDefaultObject()->CreateSelectors(WidgetOwner, inSequence); }
	//virtual void RunSelectors(APlayerController* WidgetOwner, UDMSSequence* inSequence, void (UDMSSeqManager::*OnSelectorsFinished)(UDMSSequence*)) { EffectNode.GetDefaultObject()->RunSelectors(WidgetOwner, inSequence,OnSelectorsFinished); }
};

/**
 * 	========================================
 *
 *	Effect Set :: 
 *	This class represents whole effect text.
 *	(ex.	1) [EffectNode 1 Desc]
			2) [EffectNode 2 Desc]
			...						)
 * 
 * =========================================
 */
UCLASS(BlueprintType, ClassGroup = (Effect), DefaultToInstanced, EditInlineNew, meta = (DisplayName = "Make Custom EffectSet"))
class PROJECTDMSGAME_API UDMSEffectSet : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CardEffect, Instanced)
	TArray<UDMSEffectNodeWrapper*> EffectNodes;

	//UFUNCTION()
	TArray<UDMSEffectNodeWrapper*> SearchEffects(const FName& iKeyword) { return TArray<UDMSEffectNodeWrapper*>(); }
	//TArray<UDMSEffectNode*> SearchEffects(UDMSSequence* iSeq);

	bool IsContainKeyword(const FName& iKeyword);
};

