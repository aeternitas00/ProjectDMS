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
//	게임 내에서 사용 할 노티파이 데이터를 받아 왔을 때 이에 반응 여부를 이 클래스로 체크함.
//	=> 조건 체크, 타이밍 체크등에 사용.
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
