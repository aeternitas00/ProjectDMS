// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DMSPlayerController.h"
#include "Gamemodes/DMSGameMode.h"
#include "Sequence/DMSSeqManager.h"

#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEIManagerComponent.h"
#include "Effect/DMSEffectorInterface.h"
#include "EffectSet/DMSEffectNode_PlayCard.h"
#include "EffectSet/DMSEffect_ActivateEffect.h"

#include "Card/DMSCardDefinition.h"
#include "Card/DMSCardContainerComponent.h"
#include "Card/DMSCardManagerComponent.h"
#include "Card/DMSCardBase.h"

#include "Selector/DMSEffectElementSelectorWidget.h"
#include "Attribute/DMSAttributeComponent.h"

#include "Library/DMSCoreFunctionLibrary.h"



ADMSPlayerController::ADMSPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShowMouseCursor=true;
	bEnableClickEvents=true;
	bEnableMouseOverEvents=true;

	CardManagerComponent = CreateDefaultSubobject<UDMSCardManagerComponent>(TEXT("CardManagerComponent"));
	EffectManagerComponent = CreateDefaultSubobject<UDMSEIManagerComponent>("EffectManagerComponent");
	AttributeComponent = CreateDefaultSubobject<UDMSAttributeComponent>(TEXT("AttributeComponent"));

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
	// Will be changed with same way to above
	AttributeComponent->MakeAttribute("Resource", 10);
	AttributeComponent->MakeAttribute("ActionPoint", 10);
	AttributeComponent->MakeAttribute("HP", 10);

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

	//,,,
}


//void ADMSPlayerController::PlayCardDep(ADMSCardBase* Card)
//{
//	UDMSSeqManager* SeqMan = UDMSCoreFunctionLibrary::GetDMSSequenceManager();
//
//	if(!SeqMan) return;
//
//	UDMSEffectNode_PlayCard* PlayCardDef = NewObject<UDMSEffectNode_PlayCard>(SeqMan);
//
//	TArray<TScriptInterface<IDMSEffectorInterface>> Targets;
//	Targets.Add(TScriptInterface<IDMSEffectorInterface>(Card));
//
//	SeqMan->RequestCreateSequence(this,this, PlayCardDef, Targets);
//}

UDMSCardContainerComponent* ADMSPlayerController::SearchContainer(const FName& ContainerName)
{
	return CardManagerComponent ? CardManagerComponent->SearchContainer(ContainerName) : nullptr;
}

void ADMSPlayerController::AttachEffectInstance(UDMSEffectInstance* EI)
{
	EffectManagerComponent->AttachEffectInstance(EI);
}

void ADMSPlayerController::OnNotifyReceived(bool iChainable,UDMSSequence* Seq,UObject* SourceTweak)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan,
	//	FString::Printf(TEXT("%s : OnNotifyReceived"), *this->GetName())
	//);
	EffectManagerComponent->OnNotifyReceived(iChainable,Seq,this);
}

//UDMSAttribute* ADMSPlayerController::GetAttribute(const FName& AttributeName) { return AttributeComponent->GetAttribute(AttributeName); }
//bool ADMSPlayerController::TryModAttribute(const FDMSAttributeModifier& Modifier) { return AttributeComponent->TryModAttribute(Modifier); }
//void ADMSPlayerController::MakeAttribute(const FName& AttributeName, const float& DefValue){ AttributeComponent->MakeAttribute(AttributeName,DefValue); }

float ADMSPlayerController::GetAttributeValue(const FName& AttributeName) 
{
	return AttributeComponent->GetAttribute(AttributeName) ? AttributeComponent->GetAttribute(AttributeName)->GetValue() : -1.0f; 
}
