// Fill out your copyright notice in the Description page of Project Settings.


#include "Card/DMSCardBase.h"
#include "Card/DMSCardDefinition.h"
#include "Card/DMSCardContainerComponent.h"
#include "GameModes/DMSGameMode.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Effect/DMSEffectHandler.h"
#include "Effect/DMSEIManagerComponent.h"
#include "Effect/DMSEffectorInterface.h"
#include "EffectSet/DMSEffect_ActivateEffect.h"
#include "EffectSet/DMSEffectNode_PlayCard.h"

// Sets default values
ADMSCardBase::ADMSCardBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates=true;
	SetReplicateMovement(true);

	EffectManagerComponent=CreateDefaultSubobject<UDMSEIManagerComponent>("EffectManagerComponent");

}

// Called when the game starts or when spawned
void ADMSCardBase::BeginPlay()
{
	Super::BeginPlay();
	
	// Automatic Type Behavior EI Attachment with carddefintion->defaultCardType
}

const UDMSCardDefinition* ADMSCardBase::GetCardDefinition()
{
	return CardDefinition;
}

void ADMSCardBase::SetCardDefinition(const UDMSCardDefinition* iCardDefinition)
{
	CardDefinition = iCardDefinition;
}


void ADMSCardBase::InitializeCard_Implementation(const UDMSCardDefinition* iCardDefinition/*, Saved data...*/)
{
	// EffectManagerComponent->CleanupOwnEffect(); ?
	SetCardDefinition(iCardDefinition);

	auto NodeGenerator = [this](UDMSEffectNodeWrapper* EffectWrapper, const FName& EffectSetName, const int& idx)->UDMSEffectNode* {
		auto EH = UDMSCoreFunctionLibrary::GetDMSEffectHandler();
		//UDMSEffectNode* Effect = EffectWrapper->GetEffectNode();
		UDMSEffectNode* Node = NewObject<UDMSEffectNode>(EffectManagerComponent);
		UDMSEffect_ActivateEffect* AEffect = NewObject<UDMSEffect_ActivateEffect>(Node);
		//UDMSDataObjectSet* DataSet = NewObject<UDMSDataObjectSet>(this);

		AEffect->EffectIdx = idx;
		AEffect->EffectSetName = EffectSetName;
		// NODE INITIALIZER?
		Node->EffectDefinitions.Add(AEffect);

		return Node;
	};

	// ¿ì¸®²¨¸¸?
	if (CardDefinition->CardEffectSets.Contains("Effect"))
		EffectManagerComponent->SetupOwnEffect(CardDefinition->CardEffectSets["Effect"], "Effect", NodeGenerator);

	if (CardDefinition->CardEffectSets.Contains("Cost"))
		EffectManagerComponent->SetupOwnEffect(CardDefinition->CardEffectSets["Cost"],"Cost", NodeGenerator);
}

void ADMSCardBase::AttachEffectInstance(UDMSEffectInstance* EI)
{
	EffectManagerComponent->AttachEffectInstance(EI);
}

bool ADMSCardBase::OnNotifyReceived(TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*>& ResponsedObjects, bool iChainable, UDMSSequence* Seq, UObject* SourceTweaker)
{
	//DMS_LOG_SCREEN(TEXT("%s : OnNotifyReceived"), *this->GetName());
	return EffectManagerComponent->OnNotifyReceived(ResponsedObjects,iChainable,Seq,this);
}

UDMSEffectSet* ADMSCardBase::GetOwningEffectSet(const FName& iSetName)
{
	//return CardDefinition->CardEffects;
	return CardDefinition->CardEffectSets.Contains(iSetName) ? CardDefinition->CardEffectSets[iSetName] : nullptr;
}


