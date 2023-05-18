// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sequence/DMSSequenceStep.h"
#include "DMSSequenceStep_Apply.generated.h"

/**
 * Smallest unit of a sequence.
 */
UCLASS()
class PROJECTDMSGAME_API UDMSSequenceStep_Apply : public UDMSSequenceStep
{
	GENERATED_BODY()

public:
	virtual void OnBefore_Implementation() override;
	virtual void OnDuring_Implementation();
	virtual void OnAfter_Implementation();
};

