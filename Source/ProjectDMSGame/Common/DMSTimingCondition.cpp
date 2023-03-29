#include "Common/DMSTimingCondition.h"
#include "Common/DMSCommons.h"
#include "Common/DMSObjectSelectionComparer.h"
#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEffectDefinition.h"
#include "Sequence/DMSSequence.h"


bool UDMSTimingCondition::CheckCondition(UObject* Caller, UDMSSequence* iSeq) const
{
	// Detailed query option?
	bool CheckerRs=true;
	// TODO :: Use CheckersConbinater. Current code is "All &"
	for ( auto Check : Checkers )
	{
		auto Targets = GetCompareTarget(Caller, iSeq, Check->CompareTargetFlag);
		CheckerRs = CheckerRs && Check->Check(Caller, Targets);
	}
	//return iSeq->Progress == Timing 
	//	&& iSeq->OriginalEffectNode->IsContainKeyword(EffectKeyword) 
	//	&& CheckerRs
	//;
	return (iSeq->Progress == Timing || Timing == EDMSTimingFlag::T_Null)
		&& iSeq->OriginalEffectNode->ExecuteTagQuery(EffectTagQuery)
		&& CheckerRs
	;

}

TArray<UObject*> UDMSTimingCondition::GetCompareTarget(UObject* Caller, UDMSSequence* iSeq,  const EDMSObjectSelectorFlag& Flag) const
{
	TArray<UObject*> Rv;
	switch (Flag)
	{
	case EDMSObjectSelectorFlag::OSF_Default:
		Rv.Add(Caller);
		break;
	case EDMSObjectSelectorFlag::OSF_SourceObj:
		Rv.Add(iSeq->SourceObject);
		break;
	case EDMSObjectSelectorFlag::OSF_SourceCtr:
		Rv.Add(iSeq->SourceController);
		break;
	case EDMSObjectSelectorFlag::OSF_Target:
		for (auto InterfaceScript : iSeq->Targets)
			Rv.Add(InterfaceScript.GetObject());
		break;
	case EDMSObjectSelectorFlag::OSF_Data:
	case EDMSObjectSelectorFlag::OSF_Custom:
		//Values.Add(State.CheckState(Seq->Target, Seq));
		break;
	default:	break;
	}

	return Rv;
}

bool UDMSTimingClassWrapper::CheckCondition(UObject* Caller, UDMSSequence* iSeq) const
{
	return Condition.GetDefaultObject()->CheckCondition(Caller, iSeq);
}

bool UDMSTimingCombiner::CheckCondition(UObject* Caller, UDMSSequence* iSeq) const
{
	bool rv = bIsAnd;

	if (Conditions.Num() == 0)return bEmptyTimingIsTrue;

	for (auto Condition : Conditions)
	{
		rv = bIsAnd ? rv && Condition->CheckCondition(Caller, iSeq) : rv || Condition->CheckCondition(Caller, iSeq);
	}

	return rv;
}

bool UDMSNotifyCheckerDefinition_ObjectCompareBase::Check_Implementation(UObject* Caller, const TArray<UObject*>& CompareTarget)
{
	return CompareTarget.Num() == 0 ? bAllowNull : SelectionRules.CompareObject(Caller, CompareTarget);
}