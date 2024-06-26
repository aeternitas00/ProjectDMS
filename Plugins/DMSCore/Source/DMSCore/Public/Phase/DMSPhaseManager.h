// Copyright JeongWoo Lee

#pragma once

#include "DMSCoreIncludes.h"
#include "Phase/DMSPhase.h"
#include "UObject/NoExportTypes.h"
#include "DMSPhaseManager.generated.h"

// class ADMSPhase;
class UDMSPhase;
/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Phase))
class DMSCORE_API UDMSPhaseManager : public UActorComponent
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
	TObjectPtr<UDMSPhase> CurrentPhase;

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
	void InitiatePhaseLoop(){ if (CurrentPhase!=nullptr) CurrentPhase->StartPhase(); }

	/**
	 * Getter of CurrentPhase.
	 */
	UFUNCTION(BlueprintCallable,meta =(AdvancedDisplay = 1))
	void ProceedToNextPhase(FGameplayTag JumpTarget);

	/**
	 * Generate Phases.
	 */
	UFUNCTION(BlueprintCallable)
	void GeneratePhases();

	/**
	 * 
	 */
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void SetupPhases();
	virtual void SetupPhases_Implementation();
};
