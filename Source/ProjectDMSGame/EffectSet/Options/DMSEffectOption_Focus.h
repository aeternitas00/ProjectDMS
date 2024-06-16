// Copyright JeongWoo Lee

#pragma once

#include "ProjectDMS.h"
#include "Effect/DMSEffectOption.h"
#include "DMSEffectOption_Focus.generated.h"

class UDMSTargetGenerator;

/**
 * 
 */
UCLASS()
class PROJECTDMSGAME_API UDMSEffectOption_Focus : public UDMSEffectOption
{
	GENERATED_BODY()

public:
	UDMSEffectOption_Focus();
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly,Instanced)
	TObjectPtr<UDMSTargetGenerator> FocusTarget;

	virtual void OnExecuteOption_Implementation(ADMSSequence* iSourceSeq, ADMSActiveEffect* iEI, const FOnOptionCompleted& OnOptionCompleted);

};
