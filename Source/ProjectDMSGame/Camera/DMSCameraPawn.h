// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "GameFramework/Pawn.h"
#include "DMSCameraPawn.generated.h"

class UCameraComponent;
class UPlayerMappableInputConfig;
struct FInputActionValue;

UCLASS()
class PROJECTDMSGAME_API ADMSCameraPawn : public APawn
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UPawnMovementComponent> MovementComponent;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UPlayerMappableInputConfig> DefaultInputConfig;

public:
	// Sets default values for this pawn's properties
	ADMSCameraPawn();

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	void Input_Move(const FInputActionValue& InputActionValue);


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
