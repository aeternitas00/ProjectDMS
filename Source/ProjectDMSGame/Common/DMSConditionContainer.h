#pragma once

#include "ProjectDMS.h"
#include "Common/DMSTimingCondition.h"
#include "Common/DMSStateCondition.h"
#include "DMSConditionContainer.generated.h"

class UDMSSequence;

USTRUCT(BlueprintType)
struct FDMSConditionContainer
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = Condition)
	TArray<UDMSTimingConditionWrapper*> TimingCondition;

	UPROPERTY(EditDefaultsOnly, Category = Condition)
	FString TimingCombinator;

	UPROPERTY(EditDefaultsOnly, Category = Condition)
	TArray<UDMSStateConditionWrapper*> StateCondition;
	
	UPROPERTY(EditDefaultsOnly, Category = Condition)
	FString StateCombinator;

	void AddTimingCondition(UDMSTimingCondition* iCondition);
	void AddTimingCondition(const TSubclassOf<UDMSTimingCondition>& iCondition);
	void AddStateCondition(UDMSStateCondition* iCondition);
	void AddStateCondition(const TSubclassOf<UDMSStateCondition>& iCondition);
	void ClearConditions() { TimingCondition.Empty(); TimingCombinator="";StateCondition.Empty(); StateCombinator="";}
	bool CheckCondition(UObject* Caller, UDMSSequence* iSeq);
};
