#include "Common/DMSConditionContainer.h"
#include "Sequence/DMSSequence.h"

void FDMSConditionContainer::AddTimingCondition(UDMSTimingCondition* iCondition)
{
	auto temp = NewObject<UDMSTimingConditionWrapper_Manual>();
	temp->Condition = iCondition;
	//TimingCondition.Empty();
	TimingCondition.Add(temp);
}

void FDMSConditionContainer::AddTimingCondition(const TSubclassOf<UDMSTimingCondition>& iCondition)
{
	auto temp = NewObject<UDMSTimingConditionWrapper_Preset>();
	temp->Condition = iCondition;
	//TimingCondition.Empty();
	TimingCondition.Add(temp);
}

void FDMSConditionContainer::AddStateCondition(UDMSStateCondition* iCondition)
{
	auto temp = NewObject<UDMSStateConditionWrapper_Manual>();
	temp->Condition = iCondition;
	//StateCondition.Empty();
	StateCondition.Add(temp);
}

void FDMSConditionContainer::AddStateCondition(const TSubclassOf<UDMSStateCondition>& iCondition)
{
	auto temp = NewObject<UDMSStateConditionWrapper_Preset>();
	temp->Condition = iCondition;
	//StateCondition.Empty();
	StateCondition.Add(temp);
}

bool FDMSConditionContainer::CheckCondition(UObject* Caller, UDMSSequence* iSeq)
{
	bool Timing = true;
	if(TimingCondition.Num()==0 && !bEmptyTimingIsTrue) Timing=false;
	for (auto T : TimingCondition) Timing = Timing && T->CheckCondition(Caller, iSeq);

	bool State = true;
	if (StateCondition.Num() == 0 && !bEmptyStateIsTrue) State = false;
	for (auto S : StateCondition) State = State && S->CheckCondition(iSeq);

	return (Timing&&State);

	//return true;
}