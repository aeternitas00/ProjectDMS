// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DMSPlayerController.h"
#include "Player/DMSPlayerState.h"
#include "Gamemodes/DMSGameMode.h"
#include "Sequence/DMSSeqManager.h"
#include "Camera/DMSCameraPawn.h"
#include "System/DMSSaveGame.h"

#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEffectorInterface.h"
#include "Effect/DMSEffect_ActivateEffect.h"

#include "Card/DMSCardDefinition.h"
#include "Card/DMSCardBase.h"

#include "Attribute/DMSAttributeComponent.h"

#include "Character/DMSCharacterBase.h"
#include "Library/DMSCoreFunctionLibrary.h"



ADMSPlayerController::ADMSPlayerController(const FObjectInitializer& ObjectInitializer)
	: ADMSPlayerControllerBase(ObjectInitializer)/*, InteractionMode(EDMSCardInteractionMode::PIM_Play)*/
{
	bShowMouseCursor=true;
	bEnableClickEvents=true;
	bEnableMouseOverEvents=true;
}



void ADMSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	EnableInput(this);	

	//CreateHUDWidgets(); 
	//SetupHUDWidgets();

	//Server_OnSetupComplete();
}


void ADMSPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}




ADMSCameraPawn* ADMSPlayerController::GetCameraPawn()
{
	return GetPawn<ADMSCameraPawn>();
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


void ADMSPlayerController::Server_OnSetupComplete_Implementation()
{
	ADMSGameMode* GameMode = GetWorld()->GetAuthGameMode<ADMSGameMode>();

	Cast<ADMSPlayerState>(PlayerState)->PlayerHUDReady();
	GameMode->PlayerReady();
}
