// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/DMSTargetGenerator.h"
#include "Sequence/DMSSequence.h"
#include "Effect/DMSEffectorInterface.h"
#include "Effect/DMSEffectInstance.h"
#include "Attribute/DMSAttributeValue_Object.h"
#include "Attribute/DMSAttributeComponent.h"

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

TArray<UObject*> UDMSTargetGenerator_UseParentSequence::GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const
{
	ADMSSequence* UsingSequence = CurrentSequence->ParentSequence;

	if(!SequenceTagQuery.IsEmpty())
	{
		while(UsingSequence)
		{
			if (UsingSequence->GetSequenceTags().MatchesQuery(SequenceTagQuery)) break;
			UsingSequence = UsingSequence->ParentSequence;
		}
	}

	return UsingSequence ? ParentTargetGenerator->GetTargets(Caller,UsingSequence) : TArray<UObject*>();
}


TArray<UObject*> UDMSTargetGenerator_FollowParentSeq::GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const
{
	TArray<UObject*> rv;
	if (CurrentSequence->ParentSequence == nullptr) return rv;

	for (auto& _Interface : CurrentSequence->ParentSequence->GetTargets())
		rv.Add(_Interface.GetObject());

	return rv;
}

TArray<UObject*> UDMSTargetGenerator_FollowParentSeqApply::GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const
{
	TArray<UObject*> rv;
	if (CurrentSequence->ParentSequence == nullptr) return rv;

	for (auto& Storage : CurrentSequence->ParentSequence->GetEIStorage()){
		for(auto& _Interface : Storage.ApplyTargets)
			rv.Add(_Interface.GetObject());
	}
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

TArray<UObject*> UDMSTargetGenerator_CallerAsActor::GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const
{
	if(Caller->IsA<ADMSActiveEffect>())
		return { Cast<ADMSActiveEffect>(Caller)->GetApplyTarget() };
	else 
		return { Caller };
}

TArray<UObject*> UDMSTargetGenerator_Sequence::GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const
{
	return { CurrentSequence };
}

TArray<UObject*> UDMSTargetGenerator_SequenceAEs::GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const
{
	TArray<UObject*> rv;
	for(auto& AE : CurrentSequence->GetAllActiveEffects()) rv.Add(AE);
	return rv;
}


TArray<UObject*> UDMSTargetGenerator_OwnerOfCaller::GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const
{
	if (Caller->Implements<UDMSEffectorInterface>()) return { Cast<IDMSEffectorInterface>(Caller)->Execute_GetOwningPlayer(Caller) };
	return {};
}


TArray<UObject*> UDMSTargetGenerator_FromAttribute::GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const
{
	TArray<UObject*> rv;
	auto lAttributeOwners = AttributeSource->GetTargets(Caller,CurrentSequence);

	for(auto& Owner : lAttributeOwners)
	{
		auto AOwner = Cast<AActor>(Owner);
		auto OwnerComp = AOwner ? AOwner->GetComponentByClass<UDMSAttributeComponent>() : nullptr;
		TArray<UDMSAttributeValue_Object*> ObjAtts = OwnerComp ? OwnerComp->GetTypedAttributeValuesByQuery<UDMSAttributeValue_Object>(AttributeTagQuery) : TArray<UDMSAttributeValue_Object*>();
		for(auto& ObjAtt : ObjAtts) rv.Append(ObjAtt->GetValue());
	}

	return rv;
}

TArray<UObject*> UDMSTargetGenerator_UseTGAsCaller::GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const
{
	TArray<UObject*> rv;
	auto NewCallers= CallerTG->GetTargets(Caller,CurrentSequence);

	for(auto& NewCaller : NewCallers){
		rv.Append(ResultTG->GetTargets(NewCaller,CurrentSequence));
	}
	return rv;
}

