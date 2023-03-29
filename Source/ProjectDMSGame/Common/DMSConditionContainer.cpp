#include "Common/DMSConditionContainer.h"
#include "Sequence/DMSSequence.h"

//void UDMSConditionContainer::AddTimingCondition(UDMSTimingCondition* iCondition)
//{
//	auto temp = NewObject<UDMSTimingConditionWrapper_Manual>(this);
//	temp->Condition = iCondition;
//	//TimingCondition.Empty();
//	TimingCondition.Add(temp);
//}
//
//void UDMSConditionContainer::AddTimingCondition(const TSubclassOf<UDMSTimingCondition>& iCondition)
//{
//	auto* temp = NewObject<UDMSTimingConditionWrapper_Preset>(this);
//	temp->Condition = iCondition;
//	//TimingCondition.Empty();
//	TimingCondition.Add(temp);
//}
//
//void UDMSConditionContainer::AddStateCondition(UDMSStateCondition* iCondition)
//{
//	auto temp = NewObject<UDMSStateConditionWrapper_Manual>(this);
//	temp->Condition = iCondition;
//	//StateCondition.Empty();
//	StateCondition.Add(temp);
//}
//
//void UDMSConditionContainer::AddStateCondition(const TSubclassOf<UDMSStateCondition>& iCondition)
//{
//	auto temp = NewObject<UDMSStateConditionWrapper_Preset>(this);
//	temp->Condition = iCondition;
//	//StateCondition.Empty();
//	StateCondition.Add(temp);
//}

bool UDMSConditionContainer::CheckCondition(UObject* Caller, UDMSSequence* iSeq)
{
	bool Timing = TimingConditions->CheckCondition(Caller, iSeq);
	bool State = StateConditions->CheckCondition(Caller, iSeq);
	
	return (Timing && State);
}

//void UDMSConditionContainer_::UpdateReferences()
//{
//	for (auto T : TimingCondition) T->Rename(nullptr,this);
//
//	for (auto S : StateCondition) S->Rename(nullptr, this);
//}
