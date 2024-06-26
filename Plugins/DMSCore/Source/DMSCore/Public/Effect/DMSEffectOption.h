// Copyright JeongWoo Lee

#pragma once

#include "DMSCoreIncludes.h"
#include "Effect/DMSEffectDefinition.h"
#include "UObject/NoExportTypes.h"
#include "DMSEffectOption.generated.h"

class ADMSSequence;
class ADMSActiveEffect;

/**
 * Object class for executing actions that do not affect the overall flow before the execution of the effect, 
 * ensuring non-disruptive behavior
 * 
 * =============================================================
 * TODO ::
 * 애니메이션 같은 것들과 동기 처리를 어떻게 할지 생각 해보기?
 * 애니메이션을 쓰는 입장에서 따로 처리를 구현하게 시키는게 맞나? 
 */
UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew, Abstract, ClassGroup = (Effect))
class DMSCORE_API UDMSEffectOption : public UObject
{
	GENERATED_BODY()


public:
	/**
	 * Next option ref ( sort of tree )
	 */
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly,Instanced)
	TObjectPtr<UDMSEffectOption> NextOption;
	/*
	 * @param	OnOptionCompleted	()->void lambda param 
	 */
	void ExecuteOption(ADMSSequence* iSourceSeq, ADMSActiveEffect* iEI, const FOnOptionCompleted& OnOptionCompleted);

	//void ExecuteOption(ADMSSequence* SourceSequence, ADMSActiveEffect* iEI, FOnExecuteCompleted OnOptionCompleted);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnExecuteOption(ADMSSequence* iSourceSeq, ADMSActiveEffect* iEI, const FOnOptionCompleted& OnOptionCompleted);
	virtual void OnExecuteOption_Implementation(ADMSSequence* iSourceSeq, ADMSActiveEffect* iEI , const FOnOptionCompleted& OnOptionCompleted);

};


