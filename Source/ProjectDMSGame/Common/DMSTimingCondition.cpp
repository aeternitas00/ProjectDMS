#include "Common/DMSTimingCondition.h"
#include "Common/DMSCommons.h"
#include "Common/DMSObjectSelectionComparer.h"
#include "Effect/DMSEffectInstance.h"
#include "Sequence/DMSSequence.h"

bool UDMSNotifyCheckerDefinition_Numeric::Check(UObject* Caller, TArray<UObject*> CompareTarget)
{
	//if (iSeq->EIDatas->GetData(DataName)->TypeCheck<float>()) return false;

	//float Value=iSeq->EIDatas->GetData(DataName)->Get<float>();
	//return bIsRanged ? (Value>=MinValue && Value <= MaxValue): Value==ExactValue;

	return bAllowNull;
}

//bool UDMSTimingCondition::isChildOf(const UDMSTimingCondition& i) const
//{
//	TArray<FString> OrgC = {}, iC = {};
//	bool rv = true;
//
//	if (Timing != i.Timing) return false;
//
//	EffectKeyword.ToString().ParseIntoArray(OrgC, TEXT("."));
//	i.EffectKeyword.ToString().ParseIntoArray(iC, TEXT("."));
//
//	for (char n = 0; n < iC.Num(); n++)
//	{
//		if (OrgC[n] != iC[n]) { rv = false; break; }
//	}
//
//	return rv;
//}


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
	return iSeq->Progress == Timing
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

// UFUNCTION + _Implements
//
bool UDMSNotifyCheckerDefinition_ObjectCompareBase::Check(UObject* Caller, TArray<UObject*> CompareTarget)
{ 
	return CompareTarget.Num() == 0 ? bAllowNull : SelectionRules.CompareObject(Caller, CompareTarget);
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