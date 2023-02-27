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

#include "Effect/DMSEffectElementSelectorWidget.h"
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

	for (auto ContainerDef : CardContainerTypes)
		CardManagerComponent->ConstructContainer(ContainerDef.Key, ContainerDef.Value);

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
	SelectingObject = Object;

	//,,,
}


void ADMSPlayerController::PlayCardDep(ADMSCardBase* Card)
{
	UDMSSeqManager* SeqMan = UDMSCoreFunctionLibrary::GetDMSSequenceManager();

	if(!SeqMan) return;

	// TODO :: Cost work

	//UDMSEffectNode* CostSelectorNode;
	//UDMSEffect_ActivateEffect* AEffect;

	//Selector from BP
	//AEffect->Selectors.Add(NewObject<UDMSEffectElementSelectorWidget>(this));

	//CostSelectorNode->EffectDefinitions.Add(AEffect);

	//Card->GetCardDefinition()->CostAndPlayCondition;

	// Static?
	UDMSEffectNode_PlayCard* PlayCardDef = NewObject<UDMSEffectNode_PlayCard>(SeqMan);

	TArray<TScriptInterface<IDMSEffectorInterface>> Targets;
	Targets.Add(TScriptInterface<IDMSEffectorInterface>(Card));
	//UDMSDataObjectSet* Datas = NewObject<UDMSDataObjectSet>();

	//Datas->SetData(TEXT("Target"), Card);
	//Datas->SetData(TEXT("Test"), 1);

	SeqMan->RequestCreateSequence(this,this, PlayCardDef, Targets);
}

void ADMSPlayerController::AttachEffectInstance(UDMSEffectInstance* EI)
{
	EffectManagerComponent->AttachEffectInstance(EI);
}

void ADMSPlayerController::OnNotifyReceived(UDMSSequence* Seq,UObject* SourceTweak) 
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan,
		FString::Printf(TEXT("%s : OnNotifyReceived"), *this->GetName())
	);
	EffectManagerComponent->OnNotifyReceived(Seq,this);
}

inline UDMSAttribute* ADMSPlayerController::GetAttribute(const FName& AttributeName) { return AttributeComponent->GetAttribute(AttributeName); }

inline bool ADMSPlayerController::TryModAttribute(const FDMSAttributeModifier& Modifier) { return AttributeComponent->TryModAttribute(Modifier); }

inline float ADMSPlayerController::GetAttributeValue(const FName& AttributeName) { return AttributeComponent->GetAttribute(AttributeName) ? AttributeComponent->GetAttribute(AttributeName)->GetValue() : -1.0f; }
