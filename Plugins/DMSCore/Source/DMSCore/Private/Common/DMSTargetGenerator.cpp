// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/DMSTargetGenerator.h"
#include "Sequence/DMSSequence.h"
#include "Effect/DMSEffectorInterface.h"

TArray<UObject*> UDMSTargetGenerator::GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const
{
	return TArray<UObject*>();
}

TArray<UObject*> UDMSTargetGenerator_SourceObject::GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const
{
	return { CurrentSequence->GetSourceObject() };
}

TArray<UObject*> UDMSTargetGenerator_SourcePlayer::GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const
{
	return { CurrentSequence->GetSourcePlayer() };
}

TArray<UObject*> UDMSTargetGenerator_FollowParentSeq::GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const
{
	TArray<UObject*> rv;
	if (CurrentSequence->ParentSequence == nullptr) return rv;

	for (auto& _Interface : CurrentSequence->ParentSequence->GetTargets())
		rv.Add(_Interface.GetObject());

	return rv;
}

TArray<UObject*> UDMSTargetGenerator_SequenceTarget::GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const
{
	TArray<UObject*> rv;

	for (auto& _Interface : CurrentSequence->GetTargets())
		rv.Add(_Interface.GetObject());

	return rv;
}

TArray<UObject*> UDMSTargetGenerator_Caller::GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const
{
	return { Caller };
}

TArray<UObject*> UDMSTargetGenerator_Sequence::GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const
{
	return { CurrentSequence };
}

TArray<UObject*> UDMSTargetGenerator_OwnerOfCaller::GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const
{
	if (Caller->Implements<UDMSEffectorInterface>()) return { Cast<IDMSEffectorInterface>(Caller)->Execute_GetOwningPlayer(Caller) };
	return {};
}


//TArray<UObject*> UDMSTargetGenerator_FromData::GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const
//{
//	UDMSDataObject* TempData=nullptr;
//	TArray<UDMSDataObject*> TempDataArr;
//	TArray<UObject*> TempArr;
//
//	if (CurrentSequence->SequenceDatas->GetValidDataValue<UDMSDataObject*>(DataTag, TempData))
//	{
//		if (TempData->TypeCheck<UObject*>())
//			return { TempData->Get<UObject*>() };
//		else if (TempData->TypeCheck<TArray<UObject*>>())
//			return TempData->Get<TArray<UObject*>>();
//	}
//	else if (CurrentSequence->SequenceDatas->GetValidDataValue<TArray<UDMSDataObject*>>(DataTag, TempDataArr))
//	{
//		for (auto& tData : TempDataArr)
//		{
//			if (tData->TypeCheck<UObject*>())
//				TempArr.Add(tData->Get<UObject*>());
//		}
//		return TempArr;
//	}
//	else { // temp
//		UObject* TempObject=nullptr;
//	
//
//		if (CurrentSequence->SequenceDatas->GetValidDataValue<UObject*>(DataTag, TempObject))
//			return { TempObject };
//		else if (CurrentSequence->SequenceDatas->GetValidDataValue<TArray<UObject*>>(DataTag, TempArr))
//			return TempArr;
//	}
//	return {};
//}
