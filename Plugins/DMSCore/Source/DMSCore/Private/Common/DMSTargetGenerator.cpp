// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/DMSTargetGenerator.h"
#include "Sequence/DMSSequence.h"
#include "Effect/DMSEffectorInterface.h"

TArray<UObject*> UDMSTargetGenerator::GetTargets_Implementation(UObject* Caller, UDMSSequence* CurrentSequence) const
{
	return TArray<UObject*>();
}

TArray<UObject*> UDMSTargetGenerator_SourceObject::GetTargets_Implementation(UObject* Caller, UDMSSequence* CurrentSequence) const
{
	return { CurrentSequence->GetSourceObject() };
}

TArray<UObject*> UDMSTargetGenerator_SourcePlayer::GetTargets_Implementation(UObject* Caller, UDMSSequence* CurrentSequence) const
{
	return { CurrentSequence->GetSourcePlayer() };
}

TArray<UObject*> UDMSTargetGenerator_FollowParentSeq::GetTargets_Implementation(UObject* Caller, UDMSSequence* CurrentSequence) const
{
	TArray<UObject*> rv;
	if (CurrentSequence->ParentSequence == nullptr) return rv;

	for (auto& _Interface : CurrentSequence->ParentSequence->GetTargets())
		rv.Add(_Interface.GetObject());

	return rv;
}

TArray<UObject*> UDMSTargetGenerator_SequenceTarget::GetTargets_Implementation(UObject* Caller, UDMSSequence* CurrentSequence) const
{
	TArray<UObject*> rv;

	for (auto& _Interface : CurrentSequence->GetTargets())
		rv.Add(_Interface.GetObject());

	return rv;
}

TArray<UObject*> UDMSTargetGenerator_Caller::GetTargets_Implementation(UObject* Caller, UDMSSequence* CurrentSequence) const
{
	return { Caller };
}

TArray<UObject*> UDMSTargetGenerator_OwnerOfCaller::GetTargets_Implementation(UObject* Caller, UDMSSequence* CurrentSequence) const
{
	if (Caller->Implements<UDMSEffectorInterface>()) return { Cast<IDMSEffectorInterface>(Caller)->GetOwningPlayer() };
	return {};
}

TArray<UObject*> UDMSTargetGenerator_FromData::GetTargets_Implementation(UObject* Caller, UDMSSequence* CurrentSequence) const
{
	UObject* TempObject=nullptr;
	TArray<UObject*> TempArr;
	if (CurrentSequence->EIDatas->GetValidDataValue<UObject*>(DataTag, TempObject))
		return { TempObject };
	else if (CurrentSequence->EIDatas->GetValidDataValue<TArray<UObject*>>(DataTag, TempArr))
		return TempArr;
	return {};
}
