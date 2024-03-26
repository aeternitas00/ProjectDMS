// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Selector/DMSConfirmWidgetBase.h"
#include "Selector/DMSSelectorManager.h"
#include "Sequence/DMSSequenceStep.h"
#include "DMSSequenceStep_FT.generated.h"

class UDMSSelectorHandle;
class UDMSSelectorRequestGenerator;

/**
 * Skill test is for one tester : one test target 
 */
UCLASS()
class PROJECTDMSGAME_API UDMSSequenceStep_FT : public UDMSSequenceStepDefinition
{
	GENERATED_BODY()

protected: 

public: 
	UDMSSequenceStep_FT(){}

	virtual FGameplayTag GetPureStepTag_Implementation() const;
};

UCLASS(Blueprintable, Abstract)
class PROJECTDMSGAME_API UDMSSelector_FT: public UDMSSelectorBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDMSSequenceStep> OwnerStep;

	UFUNCTION(BlueprintCallable)
	TScriptInterface<IDMSEffectorInterface> GetTriggerableEffectors();

};