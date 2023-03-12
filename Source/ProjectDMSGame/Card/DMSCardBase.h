// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_GAME_MODULE
 *
 *	Base of Card actor in DMS GAME
 *
 *	========================================
 */

#include "ProjectDMS.h"
#include "GameFramework/Actor.h"
#include "Effect/DMSEffectorInterface.h"
#include "DMSCardBase.generated.h"

class UDMSCardDefinition;
class UDMSCardContainerComponent;
class UDMSEIManagerComponent;

/**
 * 	========================================
 *
 *	ADMSCardBase : Base of Card actor. Wiil be implemented in BP.
 *
 *	========================================
 */
UCLASS(Blueprintable, Abstract)
class PROJECTDMSGAME_API ADMSCardBase : public AActor, public IDMSEffectorInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADMSCardBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	// Here?
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	const UDMSCardDefinition* CardDefinition;

	UPROPERTY(BlueprintReadOnly)
	UDMSCardContainerComponent* OwningContainer;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UDMSEIManagerComponent* EffectManagerComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual UObject* GetObject() override { return this; }
	virtual void AttachEffectInstance(UDMSEffectInstance* EI) override;
	virtual void OnNotifyReceived(bool iChainable, UDMSSequence* Seq, UObject* SourceTweak) override;
	virtual UDMSEffectSet* GetOwningEffectSet(const FName& iSetName) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InitializeCard(const UDMSCardDefinition* iCardDefinition/*...*/);
	void InitializeCard_Implementation(const UDMSCardDefinition* iCardDefinition/*...*/);

	// Native Get,Setter
	const UDMSCardDefinition* GetCardDefinition();
	void SetCardDefinition(const UDMSCardDefinition* iCardDefinition); // Init

	UDMSCardContainerComponent* GetOwningContainer() {return OwningContainer;}

	// Hide or something?
	void SetOwningContainer(UDMSCardContainerComponent* Container) { OwningContainer = Container;}

	
};
