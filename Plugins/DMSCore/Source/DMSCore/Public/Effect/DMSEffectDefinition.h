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

#include "DMSCoreIncludes.h"
#include "UObject/NoExportTypes.h"
#include "Sequence/DMSSeqManager.h"
#include "Common/DMSCommonDelegates.h"
#include "Common/DMSTargetGenerator.h"
#include "Selector/DMSEffectElementSelectorWidget.h"
#include "DMSEffectDefinition.generated.h"

class UDMSEffectOption;
class UDMSSequenceStep;
class UDMSDecisionWidget;
class UDMSDataObjectSet;
class UDMSSequence;
class UDMSConditionCombiner;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnOptionCompleted,UDMSEffectOption*,CompletedOption);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnExecuteCompleted, bool, Successed);



/**
 * 	========================================
 *
 *	Effect Definition :: Minimal functor-like Object. Definition of what 'Effect' should do.
 *	This class represents a single line or word of effect text. ( "Deal 3 damage" )
 *	It is designed to be instanced in "EffectNode".
 *
 * =========================================
 */
UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew, Abstract, ClassGroup = (Effect))
class DMSCORE_API UDMSEffectDefinition : public UObject
{
	GENERATED_BODY()

public:

	/**
	 * Effect's tag. commonly used to identify the effect.
	 * Can't be null.
	 */
	UPROPERTY()
	FGameplayTag EffectTag;

	/**
	 * Get effect's tag.
	 * @return	Return EffectTag with additional tags for further identification in each child class.
	 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	FGameplayTagContainer GetEffectTags();

	virtual FGameplayTagContainer GetEffectTags_Implementation(){return FGameplayTagContainer(EffectTag);}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	TArray<TObjectPtr<UDMSEffectOption>> EffectOptions;

public:
	UDMSEffectDefinition()/*: bIsUsingSelector(false)*/{
	/*	EffectOptions = CreateDefaultSubobject<UDMSEffectOptionSet>("EffectOptions");*/
	}

	// 실행할 함수의 중복을 줄이기 위해 CardDefinition - DMSEffect에 실행부인 (Work)를 두는 것으로 하였음.
	// 이것은 카드 종류 하나에 실행부 하나만을 두기 까지 압축하는 것을 의도
	// 성공 여부 파악, 체인 확장을 위한 핸들 리턴?
	
	void ExecuteEffectOptions(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnOptionCompleted& OnOptionCompleted);
	void ExecuteEffectDefinition(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnExecuteCompleted& OnExecuteCompleted);

	/**
	 * Execution part of the this effect.
	 * @param	SourceSequence					Target sequence.
	 * @param	iEi								Source effect instance
	 * @param	OnWorkCompleted					Delegate excuted when work comple
	 * @return	Return EffectTag with additional tags for further identification in each child class.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void Work(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnExecuteCompleted& OnWorkCompleted); // temp
	virtual void Work_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnExecuteCompleted& OnWorkCompleted){ OnWorkCompleted.ExecuteIfBound(true); }

	/**
	 * Predict whether work will succeed or fail
	 * @param	SourceSequence					Target sequence.
	 * @param	iEi								Source effect instance
	 * @return	
	 */
	UFUNCTION(BlueprintNativeEvent)
	bool Predict(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI); // temp
	virtual bool Predict_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI) { return true; }

	//UFUNCTION(BlueprintNativeEvent)
	//AActor* GetPlayerFocusTarget(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI);
	//virtual AActor* GetPlayerFocusTarget_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI);

	// ====================== //
	//		For selector
	// ====================== //

	/**
	 * Is this effect using selector?
	 */
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	//bool bIsUsingSelector;
	

	//virtual void Serialize(FArchive& Ar) override;
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
class DMSCORE_API UDMSEffectNode : public UObject
{
	GENERATED_BODY()

public:
	UDMSEffectNode();
	

	/**
	 * 이펙트 노드에 대한 대표 키워드. 비워두어도 상관 없음. 
	 * 대표 키워드를 통한 노티파이 플로우의 세부적인 쿼리옵션을 사용하고 싶을 경우 사용.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	FGameplayTag NodeTag;

	/**
	 * It returns a container that consolidates all the tags associated with the node like EffectDefinition's one (e.g., tags for the target attribute in ED_ModAtt). 
	 */
	FGameplayTagContainer GenerateTagContainer();

	/**
	 * Execute param tag query to generated node tag container.
	 * @param	EffectTagQuery				Executing tag query
	 * @return	Query result.
	 */
	bool ExecuteTagQuery(const FGameplayTagQuery& EffectTagQuery);

//=================== Conditions and timing that check before activate main effect ===================//

	/**
	 * Effect's activatable timing.
	 */ 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = Effect,meta = (EditCondition = "!bIgnoreNotify", EditConditionHides))
	TObjectPtr<UDMSConditionCombiner> Conditions;

	/**
	 * Has a choice about triggering the effect ? 
	 * true : Forced trigger when meet the conditions. / false : Can choose Y / N of trigger.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect, meta = (EditCondition = "!bIgnoreNotify", EditConditionHides))
	bool bForced;

	/**
	 * This Effect doesn't receive notifies. 
	 * It will not be activated except through other 'Activate Effect' effect. 
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	bool bIgnoreNotify;


//=================== Pre-activate ( Decision step ) ===================//

	/**
	 * Classes of decision widget what this effect will use.
	 */
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	//TArray<TSubclassOf<UDMSDecisionWidget>> DecisionWidgetClasses;

	/**
	 * Create decision widget with "DecisionWidgetClasses".
	 * @param	WidgetOwner
	 * @return	Crated widgets.
	 */
	//TArray<UDMSDecisionWidget*> CreateDecisionWidgets(UDMSSequence* OwnerSequence, APlayerController* WidgetOwner);

	/**
	 * Implements on BP.How to initializing decision widget's candidate or search range. 
	 * @param	iWidget						Param's order follows "DecisionWidgetClasses" property's one.
	 */ 
	//UFUNCTION(BlueprintNativeEvent, Category = Effect)
	//void InitializeDecisionWidget(const TArray<UDMSDecisionWidget*>& iWidget);
	//virtual void InitializeDecisionWidget_Implementation(const TArray<UDMSDecisionWidget*>& iWidget) {};


//=================== Main effect ===================//

	/**
	 * Actual effects that activatable in that timing
	 * Works in order to 0~n
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect, Instanced)
	TArray<UDMSEffectDefinition*> EffectDefinitions;

	/**
	 * Target generator to be used by the EffectNode when the sequence using this EffectNode does not have an explicit target.
	 * The EffectNode uses this target generator to set the target of sequence by itself.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = Effect)
	TObjectPtr<UDMSTargetGenerator> TargetGenerator;

	/**
	 * Implement how to generate applying targets. ( Effect like targeting player but apply to player's card or something )
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = Effect)
	TObjectPtr<UDMSTargetGenerator> ApplyTargetGenerator;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect, meta = (EditCondition = "TargetGenerator||ApplyTargetGenerator", EditConditionHides))
	bool bLazyTargetting;

	/**
	 * Use this when effect has to set targets with runtime data ( Sequence ).
	 * @param	iSequence						Current sequence.
	 * @return	Generated targets.
	 */
	UFUNCTION(BlueprintCallable,Category = Effect)
	static TArray<TScriptInterface<IDMSEffectorInterface>> GeneratePresetTarget(UDMSEffectNode* Node, UDMSSequence* iSequence);

	/**
	 * Implement how to generate applying targets. ( Effect like targeting player but apply to player's card or something )
	 * @param	iSequence						Current sequence.
	 * @return	Generated apply targets.
	 */
	UFUNCTION(BlueprintCallable, Category = Effect)
	static TArray<FDMSSequenceEIStorage> GenerateApplyTarget(UDMSEffectNode* Node, UDMSSequence* iSequence);

	//=================== Child effect ===================//

	/**
	 * Effect's child(sub) effect
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = Effect)
	TObjectPtr<UDMSEffectNodeWrapper> ChildEffect;

	/**
	 * Flag of "Is this effect has chainable window?".
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	bool bIsChainableEffect;

	//=================== Step ===================//

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = Effect)
	TArray<TObjectPtr<UDMSSequenceStep>> StepRequirements;



	//virtual void Serialize(FArchive& Ar) override;
	//virtual void PostInitProperties() override;
};

/** 
 * 	========================================
 *
 *	Effect Node Wrapper :: Wrapper for exposing node to Blueprint in various ways.
 *
 *	=========================================
 */
UCLASS(Abstract,BlueprintType, Const, DefaultToInstanced, EditInlineNew, ClassGroup = (Effect))
class DMSCORE_API UDMSEffectNodeWrapper : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure,BlueprintCallable, Category = Effect, meta = (DisplayName = "Get Effect Node"))
	UDMSEffectNode* GetEffectNodeBP() { return GetEffectNode(); }
	virtual UDMSEffectNode* GetEffectNode(){return nullptr;}
//	virtual void CreateSelectors(UDMSSequence* OwnerSeq, APlayerController* WidgetOwner){}
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
class DMSCORE_API UDMSEffectNodeWrapper_Manual : public UDMSEffectNodeWrapper
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Instanced, Category = Effect)
	TObjectPtr<UDMSEffectNode> EffectNode;

	virtual UDMSEffectNode* GetEffectNode() { return EffectNode; }
//	virtual void CreateSelectors(UDMSSequence* OwnerSeq, APlayerController* WidgetOwner){ EffectNode->CreateSelectors(OwnerSeq,WidgetOwner); }
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
class DMSCORE_API UDMSEffectNodeWrapper_Preset : public UDMSEffectNodeWrapper
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = Effect, meta = (DisplayName = "Effect Node Class"))
	TSubclassOf<UDMSEffectNode> EffectNode; 

	virtual UDMSEffectNode* GetEffectNode() { return EffectNode.GetDefaultObject(); }
};


DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FNodeComparer, UDMSEffectNode*, ComparingNode);

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
class DMSCORE_API UDMSEffectSet : public UObject
{
	GENERATED_BODY()

public:

	/**
	 * Effect nodes of this effect set.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect, Instanced)
	TArray<TObjectPtr<UDMSEffectNodeWrapper>> EffectNodes;

	/**
	 * Execute param tag query to all effect nodes.
	 * @param	EffectTagQuery				Executing tag query
	 * @return	Query result.
	 */
	bool ExecuteTagQuery(const FGameplayTagQuery& EffectTagQuery);
};

