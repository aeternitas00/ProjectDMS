// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Effect/DMSEffectorActorBase.h"
#include "Location/DMSLocatableInterface.h"
#include "DMSLocationBase.generated.h"

class UDMSLocationData;

UENUM(BlueprintType)
enum class EBitOperatorType : uint8
{
	AND = 0	UMETA(DisplayName = "&"),
	OR = 0	UMETA(DisplayName = "|"),
	XOR = 0	UMETA(DisplayName = "^"),
	//NOT = 0	UMETA(DisplayName = "~"),
};

UENUM(BlueprintType)
enum class EDMSLocationState : uint8
{
	LS_Default					= 0													UMETA(DisplayName = "Default"),
	
	// Entering Flag
	LS_BlockPlayerEnter			= 1													UMETA(DisplayName = "Player character cannot enter this location"),
	LS_BlockPlayerLeave			= 1 << 1											UMETA(DisplayName = "Player character cannot leave this location"),
	LS_BlockPlayerEoL			= LS_BlockPlayerEnter | LS_BlockPlayerLeave			UMETA(DisplayName = "Player character cannot enter or leave this location"),
	
	LS_BlockEnemyEnter			= 1	<< 2											UMETA(DisplayName = "Enemy character cannot enter this location"),
	LS_BlockEnemyLeave			= 1 << 3											UMETA(DisplayName = "Enemy character cannot leave this location"),
	LS_BlockEnemyEoL			= LS_BlockEnemyEnter | LS_BlockEnemyLeave			UMETA(DisplayName = "Enemy character cannot enter or leave this location"),

	LS_BlockAnyEnter			= LS_BlockPlayerEnter | LS_BlockEnemyEnter			UMETA(DisplayName = "Any character cannot enter this location"),
	LS_BlockAnyLeave			= LS_BlockPlayerLeave | LS_BlockEnemyLeave			UMETA(DisplayName = "Any character cannot enter this location"),
	LS_BlockAnyEoL				= LS_BlockPlayerEoL | LS_BlockEnemyEoL				UMETA(DisplayName = "Any character cannot enter this location"),

	// Interaction Flag
	//LS_BlockPlayerInteraction	= 1 << 4,
};

ENUM_CLASS_FLAGS(EDMSLocationState)

/**
 * 
 */
UCLASS()
class PROJECTDMSGAME_API ADMSLocationBase : public ADMSEffectorActorBase, public IDMSLocatableInterface
{
	GENERATED_BODY()

protected:
	//UPROPERTY(BlueprintReadWrite,EditAnywhere)
	//TObjectPtr<USceneComponent> ChildSlot;
	//UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	//TObjectPtr<const UDMSLocationData> LocationData;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool CanbeStartingLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EDMSLocationState LocationStateFlag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<TObjectPtr<ADMSLocationBase>> ConnectingLocations;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<TScriptInterface<IDMSLocatableInterface>> ActorsOnLocation;

public:
	ADMSLocationBase(const FObjectInitializer& ObjectInitializer);

	//UFUNCTION(BlueprintCallable)
	//EDMSLocationState BitOperateWithLocationFlag(const EBitOperatorType& Operator, const EDMSLocationState& inFlag);

	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool CanPlayerEnter() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CanPlayerLeave() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnActorEntered(const TScriptInterface<IDMSLocatableInterface>& Locatable);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnActorLeaved(const TScriptInterface<IDMSLocatableInterface>& Locatable);

	//virtual void Initialize_Implementation(const UDMSSpawnableDataBase* inData);

	//UFUNCTION(BlueprintImplementableEvent)
	//void InitializeLocation(const FDMSScenarioLocatingData& iLocData);

	UFUNCTION(BlueprintCallable)
	static bool MoveActorToDMSLocation(ADMSLocationBase* Dest, const TScriptInterface<IDMSLocatableInterface>& Locatable);

	UFUNCTION(BlueprintCallable)
	static void ConnectLocations(ADMSLocationBase* Start,ADMSLocationBase* Dest,const bool& IsOneWay);

public:
	ADMSLocationBase* GetCurrentLocation_Implementation(){return this;}
	virtual void Initialize_Implementation(const UDMSSpawnableDataBase* NewData);
	bool LocatingTo_Implementation(ADMSLocationBase* TargetLocation){return false;}
};
