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
 * Smallest unit of a sequence.
 */
UCLASS()
class DMSCORE_API UDMSSequenceStep_Decision : public UDMSSequenceStep
{
	GENERATED_BODY()

public:
	UDMSSequenceStep_Decision();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced, Category = Decision)
	TObjectPtr<UDMSTargetGenerator> DecisionMaker;

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