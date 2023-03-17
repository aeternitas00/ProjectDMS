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
	PrimaryComponentTick.bCanEverTick = true;

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

void UDMSEIManagerComponent::OnNotifyReceived(bool iChainable, UDMSSequence* Seq, UObject* SourceTweak)
{
	for (auto OwnEI : OwnEffectInstances)
	{
		OwnEI->OnNotifyReceived(iChainable,Seq, SourceTweak);
	}
}

UDMSEffectSet* UDMSEIManagerComponent::GetOwningEffectSet(const FName& iSetName)
{
	auto Owner = Cast<IDMSEffectorInterface>(GetOwner());
	return Owner!=nullptr ? Owner->GetOwningEffectSet(iSetName) : nullptr;
}

//UDMSAttribute* UDMSEIManagerComponent::GetAttribute(const FName& AttributeName)
//{
//	auto Owner = Cast<IDMSAttributeInterface>(GetOwner());
//	return Owner != nullptr ? Owner->GetAttribute(AttributeName) : nullptr;
//}
//
//bool UDMSEIManagerComponent::TryModAttribute(const FDMSAttributeModifier& Modifier)
//{
//	DMS_LOG_SCREEN(TEXT("%s : TryModAttribute to %s"), *GetName(), *GetOwner()->GetName());
//
//	auto Owner = Cast<IDMSAttributeInterface>(GetOwner());
//
//	if (Owner == nullptr) DMS_LOG_SCREEN(TEXT("%s : Outer (%s) doesn't implements AttributeInterface"), *GetName(), *GetOwner()->GetName());
//
//	return Owner != nullptr ? Owner->TryModAttribute(Modifier) : false;
//}
//
//void UDMSEIManagerComponent::MakeAttribute(const FName& AttributeName, const float& DefValue)
//{
//	auto Owner = Cast<IDMSAttributeInterface>(GetOwner());
//
//	if (Owner == nullptr) return;
//	Owner->MakeAttribute(AttributeName, DefValue);
//}

void UDMSEIManagerComponent::SetupOwnEffect(UDMSEffectSet* EffectSet)
{
	if (EffectSet == nullptr) { DMS_LOG(Display, TEXT("%s : No Default Effect"),*GetOwner()->GetName()); return; }
	auto EffectNodes = EffectSet->EffectNodes;
	auto EH = UDMSCoreFunctionLibrary::GetDMSEffectHandler();
	if (!EH) { DMS_LOG(Error, TEXT("No Effect Handler")); 	return; }

	uint8 idx = 0;
	for (auto EffectWrapper : EffectNodes)
	{
		// Inefficient way??
		UDMSEffectNode* Effect = EffectWrapper->GetEffectNode();
		UDMSEffectNode* Node = NewObject<UDMSEffectNode>();
		UDMSEffect_ActivateEffect* AEffect = NewObject<UDMSEffect_ActivateEffect>();
		UDMSDataObjectSet* DataSet = NewObject<UDMSDataObjectSet>();

		AEffect->EffectIdx = idx++;

		// NODE INITIALIZER?
		Node->EffectDefinitions.Add(AEffect);
		Node->Conditions = Effect->Conditions;
		Node->bIsChainableEffect = false;
		Node->bForced = Effect->bForced;
		//Node->PresetTargetFlag = EDMSPresetTargetFlag::PTF_Data;
		Node->PresetTargetFlag = EDMSPresetTargetFlag::PTF_Self;
		Node->AdvanceConditions = Effect->AdvanceConditions;
		Node->ChildEffect = Effect->ChildEffect;

		// 왜이렇게했더라
		// 
		//TArray<TScriptInterface<IDMSEffectorInterface>> PresetTarget;
		//PresetTarget.Add(TScriptInterface<IDMSEffectorInterface>(this));

		//DataSet->SetData("PresetTarget", PresetTarget, true);

		AActor* CardOwner = const_cast<AActor*>(GetOwner()->GetNetOwner());
		//DMS_LOG_SCREEN(TEXT("%s : Card Owner"), *CardOwner->GetName());

		auto EIs = EH->CreateEffectInstance(GetOwner(), CardOwner, Node, DataSet);
		EIs[0]->ChangeEIState(EDMSEIState::EIS_Default);
		EIs[0]->Rename(nullptr, GetOwner());
		OwnEffectInstances.Add(EIs[0]);
	}
}
