// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sequence/DMSSequenceStep.h"
#include "DMSSequenceStep_Decision.generated.h"

class UDMSTargetGenerator;

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

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced)
	TObjectPtr<UDMSTargetGenerator> DecisionMaker;

	/**
	 * Classes of decision widget what this effect will use.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	TArray<TSubclassOf<UDMSDecisionWidget>> DecisionWidgetClasses;

	/**
	 * Create decision widget with "DecisionWidgetClasses".
	 * @param	WidgetOwner
	 * @return	Crated widgets.
	 */
	TArray<UDMSDecisionWidget*> CreateDecisionWidgets(APlayerController* WidgetOwner);

	virtual void OnBefore_Implementation() override;
	virtual void OnDuring_Implementation() override;
	virtual void OnAfter_Implementation() override;
};

