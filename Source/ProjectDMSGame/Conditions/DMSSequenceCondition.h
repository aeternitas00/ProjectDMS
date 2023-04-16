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
  *	��Ƽ���� �Ŵ����� ��Ƽ���� ���ù��鿡�� [ "SourceObjectClass"�� "EffectKeyword"�� (���) "TargetObjectClass"���� "Timing"(�ϱ���/�ϴ���/����) ]
  *	�߰������� �ҽ� ������Ʈ�� Ÿ�� ������Ʈ���� Ŭ���� ������ �ƴ� ��ü ������ ĵ����Ʈ�� ���� �Ϸ��� ���� �ؾ��ұ�?
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


