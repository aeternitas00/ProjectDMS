// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "GameFramework/Actor.h"
#include "Effect/DMSEffectorInterface.h"
#include "DMSCardBase.generated.h"

class UDMSCardDefinition;
class UDMSCardContainerComponent;
class UDMSEIManagerComponent;

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
	UDMSCardDefinition* CardDefinition;

	UPROPERTY(BlueprintReadOnly)
	UDMSCardContainerComponent* OwningContainer;

	UPROPERTY()
	UDMSEIManagerComponent* EffectManagerComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual UObject* GetObject() override { return this; }
	virtual void AttachEffectInstance(UDMSEffectInstance* EI) override;
	virtual void OnNotifyReceived(UDMSSequence* Seq, UObject* SourceTweak) override;
	virtual UDMSEffectSet* GetOwningEffectSet(const FName& iSetName) override;

	UFUNCTION(BlueprintCallable)
	void InitializeCard(UDMSCardDefinition* iCardDefinition/*...*/);

	// Native Get,Setter
	UDMSCardDefinition* GetCardDefinition();
	void SetCardDefinition(UDMSCardDefinition* iCardDefinition); // Init

	UDMSCardContainerComponent* GetOwningContainer() {return OwningContainer;}

	// Hide or something?
	void SetOwningContainer(UDMSCardContainerComponent* Container) { OwningContainer = Container;}

	
};
