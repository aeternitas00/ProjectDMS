// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Attribute/DMSAttribute.h"
#include "Attribute/DMSAttributeInterface.h"
#include "Components/ActorComponent.h"
#include "DMSAttributeComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTDMSGAME_API UDMSAttributeComponent : public UActorComponent, public IDMSAttributeInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDMSAttributeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Deprecated
	UPROPERTY()
	TMap<FName, float> Attributes; // Resources

	UPROPERTY()
	TMap<FName,UDMSAttribute*> _Attributes; // Resources

	void MakeAttribute(const FName& AttributeName, const float& DefValue =0.0f);
	virtual UDMSAttribute* GetAttribute(const FName& AttributeName) override;
	virtual bool TryModAttribute(const FDMSAttributeModifier& Modifier) override;
};
