// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Components/ActorComponent.h"
#include "DMSCardVisualComponent.generated.h"

// ======================================
// 이 컴포넌트의 실용성에 대해 재고해보기.
// 목적 자체는 카드 상태에 따라 다르게 보이는 것을 구현하기 위함인데.
// ======================================
UCLASS( ClassGroup=(Card), meta=(BlueprintSpawnableComponent) )
class PROJECTDMSGAME_API UDMSCardVisualComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDMSCardVisualComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	
};
