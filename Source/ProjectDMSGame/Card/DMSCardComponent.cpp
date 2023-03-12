// Fill out your copyright notice in the Description page of Project Settings.


#include "Card/DMSCardComponent.h"
#include "Effect/DMSEffectInstance.h"
#include "Card/DMSCardDefinition.h"
#include "Card/DMSCardBase.h"
#include "GameModes/DMSGameMode.h"
#include "EffectSet/TypeBehavior/DMSEffectNode_TypeBehavior.h"
#include "Effect/DMSEffectDefinition.h"
#include "Effect/DMSEffectHandler.h"

// Sets default values for this component's properties
UDMSCardComponent::UDMSCardComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDMSCardComponent::BeginPlay()
{
	Super::BeginPlay();

	//if(!GetDMSEffectHandler()) return;

	//UDMSEffectNode* TypeNodeCDO = GetCardDefinition()!=nullptr ? Cast<UDMSEffectNode>( GetCardDefinition()->DefaultCardType.GetDefaultObject()) : nullptr;

	//if(TypeNodeCDO!=nullptr){
	//	GetDMSEffectHandler()->CreateEffectInstance(GetOwner(), TypeNodeCDO);
	//}
}


// Called every frame
void UDMSCardComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

const UDMSCardDefinition* UDMSCardComponent::GetCardDefinition()
{
	return Cast<ADMSCardBase>(GetOwner())->GetCardDefinition();
}

