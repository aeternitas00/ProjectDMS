// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/DMSGameState.h"
#include "Player/DMSPlayerState.h"
#include "Player/DMSPlayerController.h"
#include "Camera/DMSCameraPawn.h"
#include "Card/DMSCardManagerComponent.h"
#include "Attribute/DMSAttributeComponent.h"
//#include "Effect/DMSEIManagerComponent.h"

ADMSGameState::ADMSGameState(const FObjectInitializer& Initializer) : Super(Initializer)
{
	ContainerManagerComponent = CreateDefaultSubobject<UDMSContainerManagerComponent>(TEXT("ContainerManagerComponent"));
	AttributeComponent = CreateDefaultSubobject<UDMSAttributeComponent>(TEXT("AttributesComponent"));
	AttributeComponent->SetIsReplicated(true);
}
 
void ADMSGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADMSGameState, ContainerManagerComponent);
}


void ADMSGameState::SetupDefaults_Implementation()
{
	
	//ContainerManagerComponent;
}

void ADMSGameState::SetPlayersFocusTarget(AActor* Target)
{
	if (Target == nullptr) return;

	auto PlayerControllers = GetDMSPlayerControllers();

	for (auto PC : PlayerControllers)
	{
		auto Loc = Target->GetActorLocation();
		auto OrLoc = PC->GetCameraPawn()->GetActorLocation();
		PC->GetCameraPawn()->FocusToLocation(FVector(OrLoc.X, Loc.Y, Loc.Z));
	}
}


TArray<ADMSPlayerState*> ADMSGameState::GetDMSPlayers()
{
	TArray<ADMSPlayerState*> rv;
	
	for (auto& Player : PlayerArray)
	{
		if (Player->IsA<ADMSPlayerState>())
			rv.Add(Cast<ADMSPlayerState>(Player));
	}

	return rv;
}

TArray<ADMSPlayerController*> ADMSGameState::GetDMSPlayerControllers()
{
	TArray<ADMSPlayerController*> rv;

	for (auto& Player : PlayerArray)
	{
		if (Player->GetPlayerController()->IsA<ADMSPlayerController>())
			rv.Add(Cast<ADMSPlayerController>(Player->GetPlayerController()));
	}

	return rv;
}
