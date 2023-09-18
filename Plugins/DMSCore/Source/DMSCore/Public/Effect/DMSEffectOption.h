// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DMSCoreIncludes.h"
#include "Effect/DMSEffectDefinition.h"
#include "UObject/NoExportTypes.h"
#include "DMSEffectOption.generated.h"

class UDMSSequence;
class UDMSEffectInstance;
/**
 * 
 */
UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew, Abstract, ClassGroup = (Effect))
class DMSCORE_API UDMSEffectOption : public UObject
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly,Instanced)
	TObjectPtr<UDMSEffectOption> NextOption;
	/*
	 * @param	OnOptionCompleted	()->void lambda param 
	 */
	void ExecuteOption(UDMSSequence* iSourceSeq, UDMSEffectInstance* iEI, const FOnOptionCompleted& OnOptionCompleted);

	//void ExecuteOption(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnExecuteCompleted& OnOptionCompleted);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnExecuteOption(UDMSSequence* iSourceSeq, UDMSEffectInstance* iEI, const FOnOptionCompleted& OnOptionCompleted);
	virtual void OnExecuteOption_Implementation(UDMSSequence* iSourceSeq, UDMSEffectInstance* iEI , const FOnOptionCompleted& OnOptionCompleted);

};


