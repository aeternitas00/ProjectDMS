// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "DMSPhase.h"
#include "UObject/NoExportTypes.h"
#include "DMSPhaseManager.generated.h"

class UDMSPhase;
/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Phase))
class PROJECTDMSGAME_API UDMSPhaseManager : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	
	/**
	 * Starting point of first round
	 */
	UPROPERTY(EditDefaultsOnly)
	uint8 StartIndex;

	/**
	 * Proceed with this container's order ( ROUND )
	 */
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UDMSPhase>> DefaultPhaseList;

	/**
	 * Current phase. 
	 */
	UPROPERTY()
	UDMSPhase* CurrentPhase;

public:

	/**
	 * Getter of CurrentPhase.
	 */
	UFUNCTION(BlueprintCallable)
	UDMSPhase* GetCurrentPhase() {return CurrentPhase;}

	/**
	 * Getter of CurrentPhase.
	 */
	UFUNCTION(BlueprintCallable)
	void InitiatePhaseLoop(){ if (CurrentPhase!=nullptr) CurrentPhase->PhaseStart(); }

	/**
	 * Getter of CurrentPhase.
	 */
	UFUNCTION(BlueprintCallable,meta =(AdvancedDisplay = 1))
	void ProceedToNextPhase(FGameplayTag JumpTarget);

	/**
	 * Getter of CurrentPhase.
	 */
	UFUNCTION(BlueprintCallable)
	void GeneratePhases();

	/**
	 * Getter of CurrentPhase.
	 */
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void UpdatePhaseWidget(UDMSPhase* iPhase);

	/**
	 * Getter of CurrentPhase.
	 */
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void SetupPhases();

	//virtual void SetupPhases_Implementation();
};
