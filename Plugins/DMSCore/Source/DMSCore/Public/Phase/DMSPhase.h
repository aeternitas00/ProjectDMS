// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "DMSCoreIncludes.h"
#include "Common/DMSCommons.h"
#include "UObject/NoExportTypes.h"
#include "DMSPhase.generated.h"

class UDMSEffectNode;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnPhaseStateChangedSignature, UDMSPhase*, CurrentPhase);
DECLARE_DELEGATE_OneParam(FOnPhaseStateChangedSignature_Native, UDMSPhase*);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhaseStateChanged, UDMSPhase*, CurrentPhase);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPhaseStateChanged_Native, UDMSPhase*);

/**
 * 
 */
UCLASS(Abstract, BlueprintType,Blueprintable,ClassGroup = (Phase))
class DMSCORE_API UDMSPhase : public UObject
{
	GENERATED_BODY()

public:
	UDMSPhase();

	UPROPERTY(BlueprintReadOnly)
	UDMSPhase* NextPhase;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	FGameplayTag PhaseTag;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly,Instanced)
	UDMSEffectNode* PhaseNode;

	UPROPERTY(BlueprintAssignable)
	FOnPhaseStateChanged OnPhaseStarted;
	FOnPhaseStateChanged_Native OnPhaseStarted_Native;

	UFUNCTION(BlueprintCallable)
	void PhaseStart();

	UPROPERTY(BlueprintAssignable)
	FOnPhaseStateChanged OnPhaseEnded;
	FOnPhaseStateChanged_Native OnPhaseEnded_Native;

	UFUNCTION(BlueprintCallable)
	void PhaseEnd();
};
