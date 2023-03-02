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
 *	BP���� �⺻���� �͵� ���� Ŀ���� ����Ʈ ���� ���� �����ϰ�
 *	Effect�� �������� �⺻���� �� ���� ������Ƽ��, ��ġ, Ÿ�ٵ� ���� ���۰� ����� �κ��� �����͸� ����ϰ� ��.
 *	�����Ͱ� ���� ��� -> ��ġ�� ���� ����̶�� ���� ��, Ÿ���� �����ڰ� �ǵ��� ����
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
	FName Keyword; // �ܾ� �����̹Ƿ� fstring ��� fname

public:
	UDMSEffectDefinition() { /* Keyword = New Name */ };

	// SelectorDefinition??????
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CardEffect)
	TArray<UDMSEffectElementSelectorWidget*> Selectors; // sort of option

	// ������ �Լ��� �ߺ��� ���̱� ���� CardDefinition - DMSEffect�� ������� (Work)�� �δ� ������ �Ͽ���.
	// �̰��� ī�� ���� �ϳ��� ����� �ϳ����� �α� ���� �����ϴ� ���� �ǵ�
	// ���� ���� �ľ�, ü�� Ȯ���� ���� �ڵ� ����?
	UFUNCTION(BlueprintNativeEvent)
	void Work(UDMSEffectInstance* iEI); // temp

	UFUNCTION(BlueprintNativeEvent)
	bool GetCandidates(UDMSDataObject*& outDataObj); // temp

	void CreateSelectors_(APlayerController* WidgetOwner, UDMSSequence* inSequence);

	virtual void Work_Implementation(UDMSEffectInstance* iEI) {};
	virtual bool GetCandidates_Implementation(UDMSDataObject*& outDataObj) { return false; };

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
 * =========================================
 *
 */
UCLASS(/*HideDropdown,*/ Blueprintable, BlueprintType, ClassGroup = (Effect), DefaultToInstanced, EditInlineNew, meta = (DisplayName = "Effect Node Base"))
class PROJECTDMSGAME_API UDMSEffectNode : public UObject
{
	GENERATED_BODY()

public:
	UDMSEffectNode():NodeKeyword(""),PresetTargetFlag(EDMSPresetTargetFlag::PTF_Self), bIsChainableEffect(true) {}
	// ����Ʈ ��忡 ���� ��ǥ Ű����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = EffectNode)
	FName NodeKeyword;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = EffectNode)
	EDMSPresetTargetFlag PresetTargetFlag;

	// ��� ����� �߻��� �ڽ�Ʈ 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = EffectNode)
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

	// ����Ʈ �ߵ� �� ���� üũ�� �ѹ� �� �Ѵ��� �ߵ��ϴ� ���� ����Ʈ�� ���� ����?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = EffectNode)
	UDMSEffectNodeWrapper* ChildEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = EffectNode)
	bool bIsChainableEffect;

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
UCLASS(Abstract, Const, DefaultToInstanced, EditInlineNew, ClassGroup = (Condition))
class PROJECTDMSGAME_API UDMSEffectNodeWrapper : public UObject
{
	GENERATED_BODY()

public:
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
	UPROPERTY(EditDefaultsOnly, Category = Effect)
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

