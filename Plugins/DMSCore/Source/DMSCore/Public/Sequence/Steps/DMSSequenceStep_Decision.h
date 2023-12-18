// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sequence/DMSSequenceStep.h"
#include "Selector/DMSSelectorManager.h"
#include "Selector/DMSDecisionDefinition.h"
#include "DMSSequenceStep_Decision.generated.h"

class UDMSTargetGenerator;
class UDMSDecisionWidget;
class ADMSPlayerControllerBase;
class UDMSEffectInstance;

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
class DMSCORE_API UDMSSequenceStep_Decision : public UDMSSequenceStep
{
	GENERATED_BODY()

public:
	UDMSSequenceStep_Decision();

	/**
	 * Target generator for who will handle this step.
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced, Category = Decision)
	TObjectPtr<UDMSTargetGenerator> DecisionMaker;

	/**
	 * Definitions regarding what to select and how it should be processed.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Decision)
	TArray<FDMSDecisionDefinition> DecisionDefinitions;

	virtual FGameplayTag GetStepTag_Implementation() const;

	virtual void OnBefore_Implementation() override;
	virtual void OnDuring_Implementation() override;
	virtual void OnAfter_Implementation() override;

	template<typename FuncSuccessed>
	void RunWidgetQueue(ADMSPlayerControllerBase* WidgetOwner, FuncSuccessed&& Successed);
};

template<typename FuncSuccessed>
void UDMSSequenceStep_Decision::RunWidgetQueue(ADMSPlayerControllerBase* WidgetOwner, FuncSuccessed&& Successed)
{
	WidgetOwner->RunWidgetQueue(		
		Successed,
		[=](UDMSSequence* pSequence) {
			// Decision canceled
			DMS_LOG_SIMPLE(TEXT("Decision canceled"));
			ProgressComplete(false);
		}
	);
}