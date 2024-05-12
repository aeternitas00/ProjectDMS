#include "Conditions/DMSObjectAttributeCondition.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Attribute/DMSAttribute.h"
#include "Attribute/DMSAttributeValue_Numeric.h"
#include "Sequence/DMSSequence.h"
#include "Effect/DMSEffectInstance.h"

bool UDMSObjectAttributeCondition::SingleCheckCondition_Implementation(UObject* CheckingGameObject, ADMSSequence* CurrentSequence, UObject* Target) const
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
	
	UDMSAttribute* CAtt = AttComp->GetAttribute(ConditionAttributeTag);
	UDMSAttributeValue_Numeric* CAttVal = CAtt ? Cast<UDMSAttributeValue_Numeric>(Att->AttributeValue) : nullptr;
	float fCVal = CAttVal ? CAttVal->GetValue() : Value;

	switch (Operator)
	{
		case EDMSComparisonOperator::BO_Equal:
			return AttVal->GetValue() == fCVal;	
		case EDMSComparisonOperator::BO_Greater:
			return AttVal->GetValue() > fCVal;
		case EDMSComparisonOperator::BO_Less:
			return AttVal->GetValue() < fCVal;
		case EDMSComparisonOperator::BO_GreaterEqual:
			return AttVal->GetValue() >= fCVal;
		case EDMSComparisonOperator::BO_LessEqual:
			return AttVal->GetValue() <= fCVal;
		default: 
			return bNullIsTrue;
	}
}

