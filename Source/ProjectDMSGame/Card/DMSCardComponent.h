// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Components/ActorComponent.h"
#include "DMSCardComponent.generated.h"

class UDMSEffectInstance;
class UDMSCardDefinition;

// 
// Rename :: CardEffectComponent
UCLASS( ClassGroup=(Card), meta=(BlueprintSpawnableComponent) )
class PROJECTDMSGAME_API UDMSCardComponent : public UActorComponent
{
	GENERATED_BODY()

protected:


public:	
	// Sets default values for this component's properties
	UDMSCardComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

//========================================================================================

	UDMSCardDefinition* GetCardDefinition();
	// Lookup CardDefinition Func
	
	// 위를 이용한 발동시 결과에 대한 미리보기 기능
		
};
