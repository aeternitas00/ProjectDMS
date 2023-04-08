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
#include "Common/DMSCommonDelegates.h"
#include "Selector/DMSEffectElementSelectorWidget.h"
#include "DMSEffectDefinition.generated.h"

class UDMSDecisionWidget;
class UDMSDataObjectSet;
class UDMSSequence;
class UDMSConditionCombiner;

/**
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
 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnWorkCompleted, UDMSSequence*, SourceSequence);

UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew, Abstract, ClassGroup = (Effect))
class UDMSEffectDefinition : public UObject
{
	GENERATED_BODY()

public:
	// Discuss
	// TO GPTag? ( ������ �±� ������ ���� �ϴ� ����� �� �ŷ����̶�� ������. )
	//UPROPERTY()
	//FName Keyword; // �ܾ� �����̹Ƿ� fstring ��� fname

	// TEST
	UPROPERTY()
	FGameplayTag EffectTag;

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	FGameplayTagContainer GetEffectTags();
	virtual FGameplayTagContainer GetEffectTags_Implementation(){return FGameplayTagContainer(EffectTag);}
public:
	UDMSEffectDefinition(): bIsUsingSelector(false), bHasPairedSelector(false){}


	// ������ �Լ��� �ߺ��� ���̱� ���� CardDefinition - DMSEffect�� ������� (Work)�� �δ� ������ �Ͽ���.
	// �̰��� ī�� ���� �ϳ��� ����� �ϳ����� �α� ���� �����ϴ� ���� �ǵ�
	// ���� ���� �ľ�, ü�� Ȯ���� ���� �ڵ� ����?
	UFUNCTION(BlueprintNativeEvent)
	void Work(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnWorkCompleted& OnWorkCompleted); // temp
	virtual void Work_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnWorkCompleted& OnWorkCompleted){ OnWorkCompleted.ExecuteIfBound(SourceSequence); }

	// ===== For selector features ===== //

	UFUNCTION(BlueprintNativeEvent)
	bool GetCandidates(UDMSSequence* iSeq, TArray<UDMSDataObject*>& outDataObj);
	virtual bool GetCandidates_Implementation(UDMSSequence* iSeq, TArray<UDMSDataObject*>& outDataObj) { return false; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect, meta = (EditCondition = "bHasPairedSelector", EditConditionHides))
	bool bIsUsingSelector;

	// dynamic referencer for tweaks
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bIsUsingSelector", EditConditionHides))
	FGameplayTag ReferenceDataKey;

	UPROPERTY(VisibleAnywhere, meta = (EditCondition = false, EditConditionHides))
	bool bHasPairedSelector;

	UFUNCTION(BlueprintNativeEvent)
	TSubclassOf<UDMSEffectElementSelectorWidget> GetPairedSelector();
	virtual TSubclassOf<UDMSEffectElementSelectorWidget> GetPairedSelector_Implementation(){return nullptr;}

	UDMSEffectElementSelectorWidget* CreatePairedSelector(APlayerController* WidgetOwner);
	virtual void InitializePairedSelector(UDMSEffectElementSelectorWidget* WidgetInstance){}
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
	UDMSEffectNode(): bForced(false), PresetTargetFlag(EDMSPresetTargetFlag::PTF_Self), bIsChainableEffect(true) {
		Conditions = CreateDefaultSubobject<UDMSConditionCombiner>("Conditions");
	}
	
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = EffectNode)
	//FName NodeKeyword;

	// ����Ʈ ��忡 ���� ��ǥ Ű����. ����ξ ��� ����. ��ǥ Ű���带 ���� ��Ƽ���� �÷ο��� �������� �����ɼ��� ����ϰ� ���� ��� ���.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = EffectNode)
	FGameplayTag NodeTag;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = EffectNode)
	//FGameplayTagContainer NodeTags;

	bool ExecuteTagQuery(const FGameplayTagQuery& EffectTagQuery);

//=================== Conditions and timing that check before activate main effect ===================//

	// Effect's activatable timing.

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = EffectNode)
	UDMSConditionCombiner* Conditions;

	// Has a choice about triggering the effect? == true : Forced trigger when meet the conditions. / false : Can choose Y/N of trigger.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = EffectNode)
	bool bForced;

	// Effect's Cost. It's different with cost of playing card.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = EffectNode)
	UDMSEffectSet* EffectCost;

//=================== Pre-activate ( Decision step ) ===================//

	// Classes of decision widget what this effect will use.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = EffectNode)
	TArray<TSubclassOf<UDMSDecisionWidget>> DecisionWidgetClasses;

	// Create decision widget with "DecisionWidgetClasses".
	TArray<UDMSDecisionWidget*> CreateDecisionWidgets(APlayerController* WidgetOwner);

	// Implements on BP. How to initializing decision widget's candidate or search range. 
	// Param's order follows "DecisionWidgetClasses" property's one.
	UFUNCTION(BlueprintNativeEvent)
	void InitializeDecisionWidget(const TArray<UDMSDecisionWidget*>& iWidget);
	virtual void InitializeDecisionWidget_Implementation(const TArray<UDMSDecisionWidget*>& iWidget) {};


//=================== Main effect ===================//

	// Actual effects that activatable in that timing
	// Works in order to 0~n
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = EffectNode, Instanced)
	TArray<UDMSEffectDefinition*> EffectDefinitions;

	// Flag for the effect's predefined target.
	// ex ) effect that Add some resource to "Actvivated player". 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = EffectNode)
	EDMSPresetTargetFlag PresetTargetFlag;

	// Using with "PresetTargetFlag" property. 
	// Use this when effect has to set targets with runtime data ( Sequence ).
	UFUNCTION(BlueprintNativeEvent)
	TArray<TScriptInterface<IDMSEffectorInterface>> GenerateTarget(UDMSSequence* iSequence);
	virtual TArray<TScriptInterface<IDMSEffectorInterface>> GenerateTarget_Implementation(UDMSSequence* iSequence);
	
	UFUNCTION(BlueprintCallable)
	static TArray<TScriptInterface<IDMSEffectorInterface>> GeneratePresetTarget(UDMSEffectNode* Node, UDMSSequence* iSequence);

	UFUNCTION(BlueprintNativeEvent)
	TArray<TScriptInterface<IDMSEffectorInterface>> GenerateApplyTarget(UDMSSequence* iSequence);
	virtual TArray<TScriptInterface<IDMSEffectorInterface>> GenerateApplyTarget_Implementation(UDMSSequence* iSequence);


	// Create paired selector widget from "EffectDefinitions".
	TArray<UDMSEffectElementSelectorWidget*> CreateSelectors(APlayerController* WidgetOwner);

//=================== Child effect ===================//

	// Conditions to advance to child effect.
	// DO WE ACTUALLY NEED THIS?
	// BETTER USE CHILD EFFECT's CONDITION?
	//UPROPERTY()
	//UDMSConditionContainer* AdvanceConditions;

	// Effect's child(sub) effect
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = EffectNode)
	UDMSEffectNodeWrapper* ChildEffect;

	// Flag of "Is this effect has chainable window?".
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = EffectNode)
	bool bIsChainableEffect;
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
	//virtual bool IsContainKeyword(const FName& iKeyword) {return false;}
	virtual void CreateSelectors(APlayerController* WidgetOwner){}
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
	//bool IsContainKeyword(const FName& iKeyword){return EffectNode->IsContainKeyword(iKeyword);}
	virtual void CreateSelectors(APlayerController* WidgetOwner){ EffectNode->CreateSelectors(WidgetOwner); }
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
	//bool IsContainKeyword(const FName& iKeyword) { return EffectNode.GetDefaultObject()->IsContainKeyword(iKeyword); }
	virtual void CreateSelectors(APlayerController* WidgetOwner){ EffectNode.GetDefaultObject()->CreateSelectors(WidgetOwner); }
	//virtual void RunSelectors(APlayerController* WidgetOwner, UDMSSequence* inSequence, void (UDMSSeqManager::*OnSelectorsFinished)(UDMSSequence*)) { EffectNode.GetDefaultObject()->RunSelectors(WidgetOwner, inSequence,OnSelectorsFinished); }
};

/**
 * 	========================================
 *
 *	Effect Set :: 
 *	This class represents whole effect text.
 *	(ex.	1) [EffectNode 1 Desc]
 *			2) [EffectNode 2 Desc]
 *			...					)
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
	//TArray<UDMSEffectNodeWrapper*> SearchEffects(const FGameplayTag& iKeyword) { return TArray<UDMSEffectNodeWrapper*>(); }
	//TArray<UDMSEffectNode*> SearchEffects(UDMSSequence* iSeq);

	//bool IsContainKeyword(const FName& iKeyword);
	bool ExecuteTagQuery(const FGameplayTagQuery& EffectTagQuery);
};


