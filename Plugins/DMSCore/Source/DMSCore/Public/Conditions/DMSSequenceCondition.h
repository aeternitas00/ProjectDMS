#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *	Timing checking part of Effect's condition.
 * 
 *	( RENAME? )
 *
 * =========================================
 */

#include "Conditions/DMSConditionObject.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/DataAsset.h"
#include "DMSSequenceCondition.generated.h"


 /**
  * ========================================
  *
  *	A simple container that collect single conditions and compose them like commonly used boolean expressions.
  *	Define timing condition of Effect's. Sort of 'relative' condition.
  *	Comparing own data with passed notify data is main feature.
  *
  * =========================================
  *
  */

UCLASS(Blueprintable, BlueprintType, Const, EditInlineNew, ClassGroup = (Condition), meta = (DisplayName = "CO Timing Condition"))
class DMSCORE_API UDMSSeqTimingCondition : public UDMSConditionObject
{
	GENERATED_BODY()

public:
	/**
	 * 'Before / During / After' What happens
	 */
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Condition)
	//EDMSTimingFlag Timing;

	/**
	 * 'What'
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Condition)
	FGameplayTagQuery EffectTagQuery;

	virtual bool CheckOperation_Implementation(UObject* CheckingGameObject, ADMSSequence* CurrentSequence) const;
};

UCLASS(Blueprintable, BlueprintType, Const, EditInlineNew, ClassGroup = (Condition), meta = (DisplayName = "CO Sequence State Condition"))
class DMSCORE_API UDMSSeqStateCondition : public UDMSConditionObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Condition)
	TSet<EDMSSequenceState> SuccessCondition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Condition)
	bool CheckParent;

	virtual bool CheckOperation_Implementation(UObject* CheckingGameObject, ADMSSequence* CurrentSequence) const;
};

