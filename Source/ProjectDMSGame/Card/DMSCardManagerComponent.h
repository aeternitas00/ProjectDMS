// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Components/ActorComponent.h"
#include "DMSCardContainerComponent.h"
#include "DMSCardManagerComponent.generated.h"


class UDMSCardContainerComponent;
class ADMSCardBase;
/*
* 
* Management playing cards, card containers, and etc...
* Expected to being attached to PC or PS.
* => 플레이어 단위의 카드 관리부분을 담당 할 것임 ( 필드 등 )
* 
* ===========================================================
* 
* 
*/
UCLASS(ClassGroup = (Card), meta = (BlueprintSpawnableComponent))
class PROJECTDMSGAME_API UDMSCardManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDMSCardManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	/* 각 컨테이너 별 특수 파생클래스로 구현 염두 */
	/* 이름,컴포넌트 맵으로?*/
	//UPROPERTY(Replicated)
	//UDMSCardContainerComponent* Deck;

	//UPROPERTY(Replicated)
	//UDMSCardContainerComponent* Hand;

	//UPROPERTY(Replicated)
	//UDMSCardContainerComponent* PlayArea;

	//UPROPERTY(Replicated)
	//UDMSCardContainerComponent* DiscardPile;

	// Set replicated in constructor
	UPROPERTY(BlueprintReadOnly)
	TMap<FName,UDMSCardContainerComponent*> Containers; 

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UDMSCardContainerComponent* SerachContainer(const FName& ContainerName);

	void ConstructContainer(const FName& ContainerName, TSubclassOf<UDMSCardContainerComponent> ContainerClass);

	static void MigrateCard(UDMSCardContainerComponent* Origin, uint16 OrgIdx, uint16 Num, UDMSCardContainerComponent* Dest, uint16 DestIdx);
	static void MigrateCard(ADMSCardBase* Card, UDMSCardContainerComponent* Dest, uint16 DestIdx);
	static void MigrateCard(TArray<ADMSCardBase*> Cards, UDMSCardContainerComponent* Dest, uint16 DestIdx);

	UFUNCTION(BlueprintCallable)
	void AddCardtoContainer(TArray<ADMSCardBase*> Cards,const FName& ContainerName);
	// make this MACRO
	//FORCEINLINE void DRAW(uint16 Num){ MigrateCard(Deck, 0, Num, Hand, 0); }
};

// Draw, Discard 같은 기본적인 형태의 카드 이동은 매크로로 미리 구현해 놓는 식으로 .