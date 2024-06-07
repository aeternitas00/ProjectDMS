// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sequence/DMSSequenceStep.h"
#include "Selector/DMSSelectorManager.h"
#include "Selector/DMSDecisionDefinition.h"
#include "DMSSequenceStep_Decision.generated.h"

class UDMSTargetGenerator;
class UDMSDecisionWidget;
class ADMSPlayerControllerBase;
class ADMSActiveEffect;

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Step_Decision)

/**
*	Definition of "Decision Step": It is a step that a player must go through when trying to activate an effect. 
*	When a player tries to activate an effect, the necessary information for that effect is collected, and the resulting changes are previewed to the player based on that information. 
*	The player can then make a final decision on whether to activate the effect while considering the preview, and the "Decision Step" ends.
*
*	This widget is designed for the step of "collecting the necessary information", where the player has the choice to select the information and confirm it. 
*	For example, if the effect can be activated on "one enemy", the widget for this would prompt the player to choose from the list of available enemies as the target.
*/
UCLASS()
class DMSCORE_API UDMSSequenceStepDefinition_Decision : public UDMSSequenceStepDefinition
{
	GENERATED_BODY()

public:
	UDMSSequenceStepDefinition_Decision();

	/**
	* Target generator for who will handle this step.
	*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced, Category = Decision)
	TObjectPtr<UDMSTargetGenerator> DecisionMaker;

	/**
	* Definitions regarding what to select and how it should be processed.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = Decision)
	TArray<TObjectPtr<UDMSDecisionDefinitionBase>> DecisionDefinitions__;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	EDMSBroadCastFlag BroadcastFlag_Decision;

	UFUNCTION()
	void Progress_Decision(UDMSSequenceStep* InstancedStep);

	UFUNCTION()
	void MakeDecision(UDMSSequenceStep* InstancedStep);

	// Implementations

	//virtual TArray<FName> GetDefaultProgressOrder_Implementation() const {return {"Progress_Decision"};}
	virtual TArray<FDMSStepProgressMetaData> GetOrderedProgressData_Implementation() const;
	virtual FGameplayTag GetPureStepTag_Implementation() const;
	virtual FGameplayTagContainer GetStepTag_Implementation(const UDMSSequenceStep* InstancedStep) const;
	virtual bool GetProgressOps_Implementation(const FGameplayTag& ProgressTag,TArray<FProgressExecutor>& OutExecutor);
};

class USelReqGenerator_ObjCand;

UCLASS()
class DMSCORE_API UDMSSequenceStepDefinition_TargetSelect : public UDMSSequenceStepDefinition
{
	GENERATED_BODY()

public:
	UDMSSequenceStepDefinition_TargetSelect(){ EnableExecuteByEach=false; bExecuteStepByEachMainTarget=false; }

	/**
	* Target generator for who will handle this step.
	*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced, Category = Decision)
	TObjectPtr<UDMSTargetGenerator> DecisionMaker;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced, Category = Decision)
	TObjectPtr<USelReqGenerator_ObjCand> SelectorRequest;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	EDMSBroadCastFlag BroadcastFlag_TargetSelect;

	UFUNCTION()
	void Progress_TargetSelect(UDMSSequenceStep* InstancedStep);

	UFUNCTION()
	void TargetSelect(UDMSSequenceStep* InstancedStep);

	// Implementations
	//virtual TArray<FName> GetDefaultProgressOrder_Implementation() const {return {"Progress_TargetSelect"};}
	virtual TArray<FDMSStepProgressMetaData> GetOrderedProgressData_Implementation() const;
	virtual FGameplayTag GetPureStepTag_Implementation() const;
	//virtual FGameplayTagContainer GetStepTag_Implementation(const UDMSSequenceStep* InstancedStep) const;
	virtual bool GetProgressOps_Implementation(const FGameplayTag& ProgressTag,TArray<FProgressExecutor>& OutExecutor);
};
