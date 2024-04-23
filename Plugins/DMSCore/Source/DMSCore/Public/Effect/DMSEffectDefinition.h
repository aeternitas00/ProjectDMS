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
#include "Attribute/DMSAttribute.h"
#include "Common/DMSCommonDelegates.h"
#include "Common/DMSTargetGenerator.h"
#include "DMSEffectDefinition.generated.h"

class UDMSAttribute;
class UDMSEffectOption;
class UDMSSequenceStep;
class UDMSSequenceStepDefinition;
class UDMSDecisionWidget;
class UDMSDataObjectSet;
class ADMSSequence;
class UDMSConditionCombiner;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnOptionCompleted,UDMSEffectOption*,CompletedOption);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnExecuteCompleted, bool, Succeeded);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExecuteCompleted, bool, Succeeded);



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

	/**
	 * The list of options to be executed before applying this effect.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	TArray<TObjectPtr<UDMSEffectOption>> EffectOptions;

public:
	UDMSEffectDefinition()/*: bIsUsingSelector(false)*/{
	/*	EffectOptions = CreateDefaultSubobject<UDMSEffectOptionSet>("EffectOptions");*/
	}

	// 실행할 함수의 중복을 줄이기 위해 CardDefinition - DMSEffect에 실행부인 (Work)를 두는 것으로 하였음.
	// 이것은 카드 종류 하나에 실행부 하나만을 두기 까지 압축하는 것을 의도
	// 성공 여부 파악, 체인 확장을 위한 핸들 리턴?
	
	void ExecuteEffectOptions(ADMSSequence* SourceSequence, ADMSActiveEffect* iEI, const FOnOptionCompleted& OnOptionCompleted);
	void ExecuteEffectDefinition(ADMSSequence* SourceSequence, ADMSActiveEffect* iEI, const FOnExecuteCompleted& OnExecuteCompleted);

	/**
	 * Execution part of the this effect.
	 * @param	SourceSequence					Target sequence.
	 * @param	iEi								Source effect instance
	 * @param	OnWorkCompleted					Delegate excuted when work complete
	 */
	UFUNCTION(BlueprintNativeEvent)
	void Work(ADMSSequence* SourceSequence, ADMSActiveEffect* iEI, const FOnExecuteCompleted& OnWorkCompleted); // temp
	virtual void Work_Implementation(ADMSSequence* SourceSequence, ADMSActiveEffect* iEI, const FOnExecuteCompleted& OnWorkCompleted){ OnWorkCompleted.ExecuteIfBound(true); }

	/**
	 * Predict whether work will succeed or fail
	 * @param	SourceSequence					Target sequence.
	 * @param	iEi								Source effect instance
	 * @return	true if effect activation condition check is successful
	 */
	UFUNCTION(BlueprintNativeEvent)
	bool Predict(ADMSSequence* SourceSequence, ADMSActiveEffect* iEI); // temp
	virtual bool Predict_Implementation(ADMSSequence* SourceSequence, ADMSActiveEffect* iEI) { return true; }

	// ====================== //
	//		For selector
	// ====================== //	

	//virtual void Serialize(FArchive& Ar) override;
};



/**
 *
 * 	========================================
 *
 *	Effect Node :: Customizable effect definition container. Sort of sequence definition.
 *	This class represents multiple line of effect text. 
 *	( ex. Deal 3 Damage and heal self that much. Then, if target HP is lower than ~ : Deal 2 Damage. )
 *	Pseudo tree architecture for sub-effect and branchs.
 *
 *	NOTE : RENAME? ( Actually this class is sort of Sequence definition.
 *	=========================================
 *	
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Effect), DefaultToInstanced, EditInlineNew, meta = (DisplayName = "Effect Node Base"))
class DMSCORE_API UDMSEffectNode : public UObject
{
	GENERATED_BODY()

public:
	UDMSEffectNode();
	
//=================== Notifying data ===================//

	/**
	 * 이펙트 노드에 대한 대표 키워드. 비워두어도 상관 없음. 
	 * 대표 키워드를 통한 노티파이 플로우의 세부적인 쿼리옵션을 사용하고 싶을 경우 사용.
	 */
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	//FGameplayTag NodeTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect, meta = (DisplayName = "Node Additional Tag"))
	FGameplayTagContainer NodeTags;
	/**
	 * It returns a container that consolidates all the tags associated with the node like EffectDefinition's one (e.g., tags for the target attribute in ED_ModAtt). 
	 */
	UFUNCTION(BlueprintNativeEvent)
	FGameplayTagContainer GenerateTagContainer(ADMSSequence* CurrentSequence);
	virtual FGameplayTagContainer GenerateTagContainer_Implementation(ADMSSequence* CurrentSequence);

	/**
	 * Execute param tag query to generated node tag container.
	 * @param	EffectTagQuery				Executing tag query
	 * @return	Query result.
	 */
	bool ExecuteTagQuery(const FGameplayTagQuery& EffectTagQuery,ADMSSequence* CurrentSequence = nullptr);

//=================== Main Effect ===================//
	//== TODO :: Migrate to ApplyStep ==//
	//=================== Conditions and timing that check before activate main effect ===================//

	/**
	 * Effect's default attributes.
	 */ 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect, meta = (DisplayName = "Active Effect's Default Attributes"))
	TArray<FDMSAttributeDefinition> EffectAttributes;

	/**
	 * Has a choice about triggering the effect ? 
	 * true : Forced trigger when meet the conditions. / false : Can choose Y / N of trigger.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect, meta = (DisplayName = "Is forced trigger", EditCondition = "!bIgnoreNotify", EditConditionHides))
	bool bForced;

	/**
	 * Has a choice about triggering the effect ? 
	 * true : Forced trigger when meet the conditions. / false : Can choose Y / N of trigger.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect, meta = (DisplayName = "Can activate multiple time in single notify", EditCondition = "!bIgnoreNotify&&!bForced", EditConditionHides))
	bool bCanResponseMulTime;

	/**
	 * This Effect doesn't receive notifies. 
	 * It will not be activated except through other 'Activate Effect' effect. 
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect, meta = (DisplayName = "Is unable to trigger"))
	bool bIgnoreNotify;

	/**
	 * Flag of "Is this effect has chainable window?".
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	bool bIsChainableEffect;

	/**
	 * Effect's activatable timing.
	 */ 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = Effect, meta = (DisplayName = "Trigger Conditions" ,EditCondition = "!bIgnoreNotify", EditConditionHides))
	TObjectPtr<UDMSConditionCombiner> Conditions;

	/**
	 * Effect's terminate condition.
	 */ 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = Effect, meta = (EditCondition = "bIsPersistent", EditConditionHides))
	TObjectPtr<UDMSConditionCombiner> TerminateConditions;

	//=================== Definitions ===================//

	/**
	 * Actual effects that activatable in that timing
	 * Works in order to 0~n
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect, Instanced)
	TArray<TObjectPtr<UDMSEffectDefinition>> EffectDefinitions;

	/**
	 * Target generator to be used by the EffectNode when the sequence using this EffectNode does not have an explicit target.
	 * The EffectNode uses this target generator to set the target of sequence by itself.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = Effect, meta = (DisplayName = "Main target searcher"))
	TObjectPtr<UDMSTargetGenerator> TargetGenerator;

	/**
	 * Implement how to generate applying targets. ( Effect like targeting player but apply to player's card or something )
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = Effect, meta = (DisplayName = "Apply target searcher"))
	TObjectPtr<UDMSTargetGenerator> ApplyTargetGenerator;

	/**
	 * Use this when effect has to set targets with runtime data ( Sequence ).
	 * @param	iSequence						Current sequence.
	 * @return	Generated targets.
	 */
	UFUNCTION(BlueprintCallable,Category = Effect)
	static TArray<TScriptInterface<IDMSEffectorInterface>> GeneratePresetTarget(UDMSEffectNode* Node, ADMSSequence* iSequence);

	/**
	 * Implement how to generate applying targets. ( Effect like targeting player but apply to player's card or something )
	 * @param	iSequence						Current sequence.
	 * @return	Generated apply targets.
	 */
	UFUNCTION(BlueprintCallable, Category = Effect)
	static TArray<FDMSSequenceEIStorage> GenerateApplyTarget(UDMSEffectNode* Node, ADMSSequence* iSequence);

	//=================== Child effect ===================//

	/**
	 * Effect's child(sub) effect
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = Effect)
	TObjectPtr<UDMSEffectNodeWrapper> ChildEffect;



	//=================== Step ===================//

public:

	/** 
	 * The list of steps that this sequence will have and execute.
	 */
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = Effect)
	//TArray<TObjectPtr<UDMSSequenceStep>> StepRequirements;
	//
	// NOTE :: EDITOR EXTENSION?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = Effect)
	TSet<TObjectPtr<UDMSSequenceStepDefinition>> StepClassRequirements;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	TArray<FGameplayTag> ProgressOrder;
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
//	virtual void CreateSelectors(ADMSSequence* OwnerSeq, APlayerController* WidgetOwner){}
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
//	virtual void CreateSelectors(ADMSSequence* OwnerSeq, APlayerController* WidgetOwner){ EffectNode->CreateSelectors(OwnerSeq,WidgetOwner); }
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
	bool ExecuteTagQuery(const FGameplayTagQuery& EffectTagQuery,ADMSSequence* CurrentSequence = nullptr);
};

