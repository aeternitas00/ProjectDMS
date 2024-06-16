// Copyright JeongWoo Lee

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

void UDMSEIManagerComponent::DetachActiveEffect(ADMSActiveEffect* EI)
{
	OwnEffectInstances.Remove(EI);
}

// SourceTweak :: are we actually need this?
bool UDMSEIManagerComponent::OnNotifyReceived(TMultiMap<TScriptInterface<IDMSEffectorInterface>, ADMSActiveEffect*>& ResponsedObjects, bool iChainable, ADMSSequence* Seq, AActor* SourceTweak)
{
	bool rv=false;
	for (auto OwnEI : OwnEffectInstances) {
		//NOTE :: beware of order
		rv = OwnEI->ReceiveNotify(ResponsedObjects, iChainable, Seq, SourceTweak==nullptr ? GetOwner() : SourceTweak) || rv;
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

	auto EH = UDMSCoreFunctionLibrary::GetDMSEffectHandler(this);
	if (!EH) { DMS_LOG_DETAIL(Error, TEXT("No Effect Handler")); 	return; }

	auto EffectNodes = EffectSet->EffectNodes;

	for (auto& EffectWrapper : EffectNodes)
	{
		auto Effect = EffectWrapper ? EffectWrapper->GetEffectNode() : nullptr;
		if (!Effect || Effect->Conditions == nullptr)
			{DMS_LOG_SIMPLE(TEXT("Invalid Effect or NO CONDITION"));continue;}

		auto AE = EH->CreatePersistentActiveEffect(GetOwner(), GetOwnerAsInterface()->Execute_GetOwningPlayer(GetOwner()), GetOwner(), Effect);

		OwnEffectInstances.Add(AE);
	}
}
