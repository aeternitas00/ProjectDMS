// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DMSPlayerController.h"
#include "Gamemodes/DMSGameMode.h"
#include "Sequence/DMSSeqManager.h"

#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEIManagerComponent.h"
#include "Effect/DMSEffectorInterface.h"
#include "EffectSet/DMSEffect_ActivateEffect.h"

#include "Card/DMSCardDefinition.h"
#include "Card/DMSCardContainerComponent.h"
#include "Card/DMSCardManagerComponent.h"
#include "Card/DMSCardBase.h"

#include "Selector/DMSEffectElementSelectorWidget.h"
#include "Attribute/DMSAttributeComponent.h"

#include "Library/DMSCoreFunctionLibrary.h"



ADMSPlayerController::ADMSPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)/*, InteractionMode(EDMSCardInteractionMode::PIM_Play)*/
{
	bShowMouseCursor=true;
	bEnableClickEvents=true;
	bEnableMouseOverEvents=true;

	CardManagerComponent = CreateDefaultSubobject<UDMSCardManagerComponent>(TEXT("CardManagerComponent"));
	EffectManagerComponent = CreateDefaultSubobject<UDMSEIManagerComponent>("EffectManagerComponent");
	AttributeComponent = CreateDefaultSubobject<UDMSAttributeComponent>(TEXT("AttributeComponent"));

	DefaultStats.Add(TAG_DMS_Attribute_Resource, 10);
	DefaultStats.Add(TAG_DMS_Attribute_ActionPoint, 10);
	DefaultStats.Add(TAG_DMS_Attribute_HP, 10);
	DefaultStats.Add(TAG_DMS_Attribute_STR, 5);
	DefaultStats.Add(TAG_DMS_Attribute_INT, 5);
	DefaultStats.Add(TAG_DMS_Attribute_DEX, 5);
	DefaultStats.Add(TAG_DMS_Attribute_SavedSkillBonus, 0);
}



void ADMSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	EnableInput(this);
}

void ADMSPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	for (auto ContainerDef : CardContainerTypes){
		CardManagerComponent->ConstructContainer(ContainerDef.Key, ContainerDef.Value);
	}

	for (auto Stat : DefaultStats) {
		AttributeComponent->MakeAttribute(Stat.Key, Stat.Value);
	}
}


void/**/ ADMSPlayerController::PopupSelectorWidget(TSubclassOf<UDMSEffectElementSelectorWidget> WidgetClass)
{
}

void ADMSPlayerController::InstigateObject(UObject* Object)
{
	InstigatingObject = Object;

	// Check 
	// 
	// Highlight or Outline
	//
	// 오브젝트 타입 별 정보 표시 UI 띄우기.
}

void ADMSPlayerController::SelectObject(UObject* Object)
{
	UObject* tFormer = SelectingObject;
	SelectingObject=Object;
	OnSelectedObject(tFormer);
}


UDMSCardContainerComponent* ADMSPlayerController::SearchContainer(const FName& ContainerName)
{
	return CardManagerComponent ? CardManagerComponent->SearchContainer(ContainerName) : nullptr;
}

void ADMSPlayerController::AttachEffectInstance(UDMSEffectInstance* EI)
{
	EffectManagerComponent->AttachEffectInstance(EI);
}

bool ADMSPlayerController::OnNotifyReceived(TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*>& ResponsedObjects,
	bool iChainable,UDMSSequence* Seq,UObject* SourceTweak)
{
	return EffectManagerComponent->OnNotifyReceived(ResponsedObjects, iChainable,Seq,this);
}
