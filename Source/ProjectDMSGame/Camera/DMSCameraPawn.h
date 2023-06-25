// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "DMSCameraPawn.generated.h"

class UCameraComponent;
class UFloatingPawnMovement;
class UPlayerMappableInputConfig;
struct FInputActionValue;

/**
 * 카메라 제어용 폰 ( 기본 플로팅 폰 대신 마우스 휠 사용 확대 축소 인풋 처리 구현하기 위함 )
 */
UCLASS()
class PROJECTDMSGAME_API ADMSCameraPawn : public APawn
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UFloatingPawnMovement> MovementComponent;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UPlayerMappableInputConfig> DefaultInputConfig;

	bool isFocusing;
	FVector FocusDestination;

    UPROPERTY(EditAnywhere)
    float FocusSpeed;
public:
	// Sets default values for this pawn's properties
	ADMSCameraPawn();

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void Input_Move(const FInputActionValue& InputActionValue);


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void FocusToLocation(const FVector& Destination);

};
