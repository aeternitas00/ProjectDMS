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
//	���� ������ ��� �� ��Ƽ���� �����͸� �޾� ���� �� �̿� ���� ���θ� �� Ŭ������ üũ��.
//	=> ���� üũ, Ÿ�̹� üũ� ���.
UCLASS(BlueprintType, DefaultToInstanced, EditInlineNew, ClassGroup = (Condition))
class PROJECTDMSGAME_API UDMSConditionContainer : public UObject
{
	GENERATED_BODY()

public:
	UDMSConditionContainer() /*: bEmptyTimingIsTrue(false), bEmptyStateIsTrue(false)*/ {
		TimingConditions = CreateDefaultSubobject<UDMSTimingCombiner>("TimingConditions");
		StateConditions = CreateDefaultSubobject<UDMSStateCombiner>("StateCondition");
	}

	UPROPERTY(EditDefaultsOnly, Instanced, Category = Condition)
	UDMSTimingCombiner* TimingConditions;

	UPROPERTY(EditDefaultsOnly, Instanced, Category = Condition)
	UDMSStateCombiner* StateConditions;

	bool CheckCondition(UObject* Caller, UDMSSequence* iSeq);
};
