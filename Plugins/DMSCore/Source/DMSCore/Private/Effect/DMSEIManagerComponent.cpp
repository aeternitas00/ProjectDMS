// Fill out your copyright notice in the Description page of Project Settings.

#include "Effect/DMSEIManagerComponent.h"

#include "GameModes/DMSGameModeBase.h"

#include "Sequence/Steps/DMSSequenceStep_Decision.h"

#include "Effect/DMSEffectHandler.h"
#include "Effect/DMSEffectDefinition.h"
#include "Effect/DMSEffect_ActivateEffect.h"

#include "Library/DMSCoreFunctionLibrary.h"
// Sets default values for this component's properties
UDMSEIManagerComponent::UDMSEIManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	SetIsReplicatedByDefault(true);
	// ...
}


// Called when the game starts
void UDMSEIManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	//DMS_LOG_SCREEN(TEXT("%s -> %s"), *GetOuter()->GetName(), *GetOwner()->GetName());
	// ...
	
}


void UDMSEIManagerComponent::AttachEffectInstance(ADMSActiveEffect* EI)
{
	//FString Str = (GetOwner()->GetName() + FString(TEXT("%n"), AttachedEffects.Num()));

	OwnEffectInstances.Add(EI);
	//EI->Rename(nullptr, this);
}

bool UDMSEIManagerComponent::OnNotifyReceived(TMultiMap<TScriptInterface<IDMSEffectorInterface>, ADMSActiveEffect*>& ResponsedObjects, bool iChainable, UDMSSequence* Seq, AActor* SourceTweak)
{
	bool rv=false;
	for (auto OwnEI : OwnEffectInstances) {
		rv = rv || OwnEI->OnNotifyReceived(ResponsedObjects, iChainable, Seq, SourceTweak==nullptr ? GetOwner() : SourceTweak);
	}
	return rv;
}


//UDMSEffectNode* UDMSEIManagerComponent::ActivatorNodeGenerator(const FGameplayTag& EffectSetName, const uint8& idx)
//{
//	auto EH = UDMSCoreFunctionLibrary::GetDMSEffectHandler();
//
//	UDMSEffectNode* Node = NewObject<UDMSEffectNode>(this);
//	UDMSEffect_ActivateEffect_Static* AEffect = NewObject<UDMSEffect_ActivateEffect_Static>(Node);
//
//	AEffect->EffectIdxArr = {idx};
//	AEffect->UseEffectFromOuter = true;
//	AEffect->EffectSetName = EffectSetName;
//
//	// NODE INITIALIZER?
//	Node->EffectDefinitions.Add(AEffect);
//
//	return Node;
//};



void UDMSEIManagerComponent::SetupOwnEffect(UDMSEffectSet* EffectSet,const FGameplayTag& SetName )
{
	DMS_LOG_SIMPLE(TEXT("%s : Setup own effects [%s]"), *GetOwner()->GetName(), *SetName.ToString());
	if (EffectSet == nullptr) { DMS_LOG_DETAIL(Display, TEXT("%s : No Default Effect"),*GetOwner()->GetName()); return; }
	auto EffectNodes = EffectSet->EffectNodes;
	auto EH = UDMSCoreFunctionLibrary::GetDMSEffectHandler();
	if (!EH) { DMS_LOG_DETAIL(Error, TEXT("No Effect Handler")); 	return; }


	uint8 idx = 0;
	for (auto& EffectWrapper : EffectNodes)
	{
		auto Effect = EffectWrapper->GetEffectNode();

		if (Effect->Conditions == nullptr)
			{DMS_LOG_SIMPLE(TEXT("NO CONDITION"));continue;}

		auto EIs = EH->CreateEffectInstance(GetOwner(), GetOwnerAsInterface()->GetOwningPlayer(), GetOwner(), Effect);
		//EIs[0]->ChangeEIState(EDMSEIState::EIS_Persistent);
		OwnEffectInstances.Add(EIs[0]);
	}
}
