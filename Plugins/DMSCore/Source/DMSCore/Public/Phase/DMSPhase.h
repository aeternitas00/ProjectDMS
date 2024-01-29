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
 * TODO :: Make this AInfo based
 */
UCLASS(Abstract, BlueprintType,Blueprintable,ClassGroup = (Phase))
class DMSCORE_API UDMSPhase : public UObject
{
	GENERATED_BODY()

public:
	UDMSPhase();

	/**
	 * Ref of next phase
	 */
	UPROPERTY(BlueprintReadOnly)
	UDMSPhase* NextPhase;

	/**
	 * Main tag of phase.
	 */
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	FGameplayTag PhaseTag;

	/**
	 * Effects to be triggered during the progression of the phase.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly,Instanced)
	UDMSEffectNode* PhaseNode;

	// ========= Synchronous phase flow control ========= //

	//==== START Phase ====//
	
	//== ENTRY ==//
	// Called from outer.
	UFUNCTION(BlueprintCallable)
	void StartPhase();

	//== PRESTART ==//
	UFUNCTION(BlueprintNativeEvent)
	void OnPreStartPhase();
	virtual void OnPreStartPhase_Implementation(){End_OnPreStartPhase();};

	UFUNCTION(BlueprintCallable)
	void End_OnPreStartPhase();

	//== ONSTART ==//
	UFUNCTION(BlueprintNativeEvent)
	void OnStartPhase();
	virtual void OnStartPhase_Implementation();

	UPROPERTY(BlueprintAssignable)
	FOnPhaseStateChanged OnStartPhase_Dynamic;
	FOnPhaseStateChanged_Native OnStartPhase_Native;

	//==== END Phase ====//
	
	//== ENTRY ==//
	// Called from outer.
	UFUNCTION(BlueprintCallable)
	void EndPhase();

	//== PREEND ==//
	UFUNCTION(BlueprintNativeEvent)
	void OnPreEndPhase();
	virtual void OnPreEndPhase_Implementation(){End_OnPreEndPhase();};

	UFUNCTION(BlueprintCallable)
	void End_OnPreEndPhase();

	//== ONEND ==//
	UFUNCTION(BlueprintNativeEvent)
	void OnEndPhase();
	virtual void OnEndPhase_Implementation();


	UPROPERTY(BlueprintAssignable)
	FOnPhaseStateChanged OnEndPhase_Dynamic;
	FOnPhaseStateChanged_Native OnEndPhase_Native;
};
