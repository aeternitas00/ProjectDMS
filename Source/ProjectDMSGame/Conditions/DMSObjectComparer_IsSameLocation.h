#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *	Class for condition checking between objects through comparison.
 *
 * =========================================
 */

#include "Conditions/DMSObjectCompareCondition.h"
#include "DMSObjectComparer_IsSameLocation.generated.h"

UCLASS(BlueprintType, meta = (DisplayName = "Comparer : Target is in the same location"))
class PROJECTDMSGAME_API UDMSObjectComparer_IsSameLocation : public UDMSObjectComparer
{
	GENERATED_BODY()

public:

	virtual bool Compare_Implementation(UObject* SourceObject, UDMSSequence* iSeq, UObject* TargetObject, bool NullIsTrue) override;

};

