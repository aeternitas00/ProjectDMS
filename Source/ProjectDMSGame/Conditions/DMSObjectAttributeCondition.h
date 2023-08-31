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

class UDMSSequence;

UCLASS(BlueprintType)
class PROJECTDMSGAME_API UDMSObjectAttributeCondition : public UDMSObjectConditionBase
{
	GENERATED_BODY()

public:
	/**
	 * Checking attribute's tag.
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Condition)
	FGameplayTag AttributeTag;

	/**
	 * Checking operator.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Condition)
	EDMSComparisonOperator Operator;

	/**
	 * Operating value.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Condition)
	int Value; // float? 

	virtual bool SingleCheckCondition_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence, UObject* Target) const;
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
//	virtual bool SingleCheckCondition_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence, UObject* Target) const;
//};
