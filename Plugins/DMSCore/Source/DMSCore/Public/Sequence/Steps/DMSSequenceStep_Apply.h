// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sequence/DMSSequenceStep.h"
#include "DMSSequenceStep_Apply.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Step_Apply)

/**
 * Smallest unit of a sequence.
 */
UCLASS()
class DMSCORE_API UDMSSequenceStep_Apply : public UDMSSequenceStep
{
	GENERATED_BODY()

public:
	UDMSSequenceStep_Apply();

	virtual void OnStepInitiated() override;

	virtual void Progress_During() override;

	virtual FGameplayTag GetStepTag_Implementation() const;

	virtual void OnBefore_Implementation() override;
	virtual void OnDuring_Implementation() override;
	virtual void OnAfter_Implementation() override;
};

