// Copyright JeongWoo Lee


#include "Camera/DMSCameraPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerMappableInputConfig.h"
#include "InputMappingContext.h"

// Sets default values
ADMSCameraPawn::ADMSCameraPawn():FocusSpeed(10.f)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingMovement"));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	BaseEyeHeight=0.0f;
}

void ADMSCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isFocusing)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector NewLocation = FMath::VInterpTo(CurrentLocation, FocusDestination, DeltaTime, FocusSpeed);
		SetActorLocation(NewLocation);
		float DistanceSq = FVector::DistSquared(CurrentLocation, FocusDestination);
		if (DistanceSq < 5.0f)
			isFocusing = false;
		
	}
}

void ADMSCameraPawn::Input_Move(const FInputActionValue& InputActionValue)
{
	if (Controller && !isFocusing)
	{
		const FVector Value = InputActionValue.Get<FVector>();
		//const FRotator MovementRotation(Controller->GetControlRotation().Pitch, 0.0f, 0.0f);

		if (Value.X != 0.0f)
		{
			//const FVector MovementDirection = FVector::RightVector;
			AddMovementInput(FVector::RightVector, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			//const FVector MovementDirection = FVector::UpVector;
			AddMovementInput(FVector::UpVector, Value.Y);
		}

		if (Value.Z != 0.0f)
		{
			//const FVector MovementDirection = FVector::ForwardVector;
			CameraComponent->SetFieldOfView(CameraComponent->FieldOfView + Value.Z);
			//AddMovementInput(FVector::ForwardVector, Value.Z);
		}
	}
}

// Called to bind functionality to input
void ADMSCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	FModifyContextOptions Options = {};
	Options.bIgnoreAllPressedKeysUntilRelease = false;
	// Actually add the config to the local player							
	Subsystem->AddPlayerMappableConfig(DefaultInputConfig.LoadSynchronous(), Options);

	UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	// temporal logic
	TArray< TObjectPtr<UInputMappingContext>> Keys;
	DefaultInputConfig.Get()->GetMappingContexts().GetKeys(Keys);
	const UInputAction* Action = Keys[0].Get()->GetMapping(0).Action;
	EIC->BindAction(Action, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
}


void ADMSCameraPawn::FocusToLocation(const FVector& Destination)
{
	isFocusing=true;
	FocusDestination = Destination;
}