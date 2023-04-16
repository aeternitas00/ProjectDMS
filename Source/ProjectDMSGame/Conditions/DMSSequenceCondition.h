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
  *	노티파이 매니저가 노티파이 리시버들에게 [ "SourceObjectClass"가 "EffectKeyword"를 (어떻게) "TargetObjectClass"에게 "Timing"(하기전/하는중/한후) ]
  *	추가적으로 소스 오브젝트나 타겟 오브젝트들의 클래스 단위가 아닌 객체 단위의 캔디데이트를 제공 하려면 어찌 해야할까?
  */

UCLASS(Blueprintable, BlueprintType, Const, EditInlineNew, ClassGroup = (Condition), meta = (DisplayName = "CO Timing Condition"))
class PROJECTDMSGAME_API UDMSSeqTimingCondition : public UDMSConditionObject
{
	GENERATED_BODY()

public:
	/**
	 * 'Before / During / After' What happens
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Condition)
	EDMSTimingFlag Timing;

	/**
	 * 'What'
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Condition)
	FGameplayTagQuery EffectTagQuery;

	virtual bool CheckOperation_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence) const;
};


