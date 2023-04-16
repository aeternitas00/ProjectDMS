// Fill out your copyright notice in the Description page of Project Settings.

#include "Effect/DMSEIManagerComponent.h"

#include "GameModes/DMSGameMode.h"
#include "Effect/DMSEffectHandler.h"
#include "Effect/DMSEffectDefinition.h"
#include "EffectSet/DMSEffect_ActivateEffect.h"
#include "Library/DMSCoreFunctionLibrary.h"
// Sets default values for this component's properties
UDMSEIManagerComponent::UDMSEIManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDMSEIManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	//DMS_LOG_SCREEN(TEXT("%s -> %s"), *GetOuter()->GetName(), *GetOwner()->GetName());
	// ...
	
}


// Called every frame
void UDMSEIManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDMSEIManagerComponent::AttachEffectInstance(UDMSEffectInstance* EI)
{
	//FString Str = (GetOwner()->GetName() + FString(TEXT("%n"), EffectInstances.Num()));
	EffectInstances.Add(EI);
	EI->Rename(nullptr, this);
}

bool UDMSEIManagerComponent::OnNotifyReceived(TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*>& ResponsedObjects, bool iChainable, UDMSSequence* Seq, UObject* SourceTweak)
{
	bool rv=false;
	for (auto OwnEI : OwnEffectInstances)
	{
		if (OwnEI->OnNotifyReceived(ResponsedObjects, iChainable, Seq, SourceTweak)){ 
			rv=true;
		}
	}
	return rv;
}

UDMSEffectSet* UDMSEIManagerComponent::GetOwningEffectSet(const FName& iSetName)
{
	auto Owner = Cast<IDMSEffectorInterface>(GetOwner());
	return Owner!=nullptr ? Owner->GetOwningEffectSet(iSetName) : nullptr;
}

template <typename FuncNodeInitializer>	
void UDMSEIManagerComponent::SetupOwnEffect(UDMSEffectSet* EffectSet,const FName& SetName, FuncNodeInitializer&& NodeInitializer )
{
	if (EffectSet == nullptr) { DMS_LOG_DETAIL(Display, TEXT("%s : No Default Effect"),*GetOwner()->GetName()); return; }
	auto EffectNodes = EffectSet->EffectNodes;
	auto EH = UDMSCoreFunctionLibrary::GetDMSEffectHandler();
	if (!EH) { DMS_LOG_DETAIL(Error, TEXT("No Effect Handler")); 	return; }

	uint8 idx = 0;
	for (auto EffectWrapper : EffectNodes)
	{
		UDMSEffectNode* Node = NodeInitializer(EffectWrapper, SetName,idx++);
		auto Effect = EffectWrapper->GetEffectNode();

		Node->Rename(nullptr, this);
		if (Effect->Conditions == nullptr)
			DMS_LOG_SIMPLE(TEXT("NO CONDITION"));
		Node->Conditions = DuplicateObject(Effect->Conditions, Node);
		if (Node->Conditions == nullptr)
			DMS_LOG_SIMPLE(TEXT("CONDITION DUPLICATION FAILED"));
		Node->bIsChainableEffect = false;
		Node->bForced = Effect->bForced;
		Node->PresetTargetFlag = EDMSPresetTargetFlag::PTF_Self;

		AActor* CardOwner = const_cast<AActor*>(GetOwner()->GetNetOwner());

		auto EIs = EH->CreateEffectInstance(GetOwner(), CardOwner, Node);
		EIs[0]->ChangeEIState(EDMSEIState::EIS_Persistent);
		EIs[0]->Rename(nullptr, GetOwner());
		OwnEffectInstances.Add(EIs[0]);
	}
}
