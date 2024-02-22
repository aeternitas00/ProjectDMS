#include "Conditions/DMSObjectAttributeCondition.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Attribute/DMSAttribute.h"
#include "Sequence/DMSSequence.h"
#include "Effect/DMSEffectInstance.h"

bool UDMSObjectAttributeCondition::SingleCheckCondition_Implementation(UObject* CheckingGameObject, UDMSSequence* CurrentSequence, UObject* Target) const
{
	AActor* tOuter = Cast<AActor>(Target);
	if (tOuter == nullptr) {
		//DMS_LOG_SCREEN(TEXT("%s : Outer (%s) is not actor"), *GetName(), *Target->GetName());
		return bNullIsTrue;
	}

	UDMSAttributeComponent* AttComp = Cast<UDMSAttributeComponent>(tOuter->GetComponentByClass(UDMSAttributeComponent::StaticClass()));
	if (AttComp == nullptr) { return bNullIsTrue; }

	UDMSAttribute* Att = AttComp->GetAttribute(AttributeTag);
	if (Att == nullptr) { return bNullIsTrue; }

	UDMSAttributeValue_Numeric* AttVal = Cast<UDMSAttributeValue_Numeric>(Att->AttributeValue);
	if (AttVal == nullptr) { return bNullIsTrue; }

	bool rv;
	switch (Operator)
	{
		case EDMSComparisonOperator::BO_Equal:
			rv = AttVal->GetValue() == Value; break;			
		case EDMSComparisonOperator::BO_Greater:
			rv = AttVal->GetValue() > Value; break;
		case EDMSComparisonOperator::BO_Less:
			rv = AttVal->GetValue() < Value; break;
		case EDMSComparisonOperator::BO_GreaterEqual:
			rv = AttVal->GetValue() >= Value; break;
		case EDMSComparisonOperator::BO_LessEqual:
			rv = AttVal->GetValue() <= Value; break;
		default: return bNullIsTrue;
	}

	return rv;
}

