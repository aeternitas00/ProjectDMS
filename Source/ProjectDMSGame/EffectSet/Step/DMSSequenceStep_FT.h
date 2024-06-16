// Copyright JeongWoo Lee

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

	UFUNCTION()
	void Progress_FT(UDMSSequenceStep* InstancedStep);

	virtual FGameplayTag GetPureStepTag_Implementation() const;
};

