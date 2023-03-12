#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *	Container for conditions. 
 *	Basically conditions are kept in this container and linked to the effect.
 *
 * =========================================
 */

#include "ProjectDMS.h"
#include "Common/DMSTimingCondition.h"
#include "Common/DMSStateCondition.h"
#include "DMSConditionContainer.generated.h"

class UDMSSequence;

/**
 *	========================================
 *
 *	A simple container that collect conditions and compose them like commonly used boolean expressions.
 *	Timing and state condition are 'AND'ed.
 *
 *	========================================
 */
USTRUCT(BlueprintType)
struct FDMSConditionContainer
{
	GENERATED_BODY()

public:

	FDMSConditionContainer() : bEmptyTimingIsTrue(false), bEmptyStateIsTrue(false){}

	UPROPERTY(EditDefaultsOnly, Category = Condition)
	TArray<UDMSTimingConditionWrapper*> TimingCondition;

	UPROPERTY(EditDefaultsOnly, Category = Condition)
	FString TimingCombinator;

	UPROPERTY(EditDefaultsOnly, Category = Condition)
	TArray<UDMSStateConditionWrapper*> StateCondition;
	
	UPROPERTY(EditDefaultsOnly, Category = Condition)
	FString StateCombinator;

	// For advancing effect or debug. use it carefully!
	UPROPERTY(EditDefaultsOnly, Category = Condition)
	bool bEmptyTimingIsTrue;

	UPROPERTY(EditDefaultsOnly, Category = Condition)
	bool bEmptyStateIsTrue;


	void AddTimingCondition(UDMSTimingCondition* iCondition);
	void AddTimingCondition(const TSubclassOf<UDMSTimingCondition>& iCondition);
	void AddStateCondition(UDMSStateCondition* iCondition);
	void AddStateCondition(const TSubclassOf<UDMSStateCondition>& iCondition);
	void ClearConditions() { TimingCondition.Empty(); TimingCombinator="";StateCondition.Empty(); StateCombinator="";}
	bool CheckCondition(UObject* Caller, UDMSSequence* iSeq);
};
