#pragma once

/**
 * 	========================================
 *
 *	DMS_GAME_MODULE
 *
 *	Simple implements of Object's Attribute checking condition.
 *
 * =========================================
 */

#include "Conditions/DMSObjectConditionBase.h"
#include "DMSObjectAttributeCondition.generated.h"

class ADMSSequence;

UCLASS(BlueprintType)
class PROJECTDMSGAME_API UDMSObjectAttributeCondition : public UDMSObjectConditionBase
{
	GENERATED_BODY()

public:
	/**
	 * Checking attribute's tag.
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Condition)
	FGameplayTagContainer AttributeTag;

	/**
	 * Checking operator.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Condition)
	EDMSComparisonOperator Operator;

	/**
	* Checking attribute's tag.
	*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Condition)
	FGameplayTagContainer ConditionAttributeTag;

	/**
	 * Operating value.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Condition, meta=(DisplayName="Default condition value"))
	int Value; // float? 

	virtual bool SingleCheckCondition_Implementation(UObject* CheckingGameObject, ADMSSequence* CurrentSequence, UObject* Target) const;
};


//UCLASS(BlueprintType)
//class PROJECTDMSGAME_API UDMSLocatingCondition : public UDMSObjectConditionBase
//{
//	GENERATED_BODY()
//
//public:
//	/**
//	 * Checking attribute's tag.
//	 */
//	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Condition)
//	FGameplayTag AttributeTag;
//
//	/**
//	 * Operating value.
//	 */
//	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Condition)
//	int Value; // float? 
//
//	virtual bool SingleCheckCondition_Implementation(UObject* CheckingGameObject, ADMSSequence* CurrentSequence, UObject* Target) const;
//};
