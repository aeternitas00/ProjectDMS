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
* => �÷��̾� ������ ī�� �����κ��� ��� �� ���� ( �ʵ� �� )
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
	/* �� �����̳� �� Ư�� �Ļ�Ŭ������ ���� ���� */
	/* �̸�,������Ʈ ������?*/
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

// Draw, Discard ���� �⺻���� ������ ī�� �̵��� ��ũ�η� �̸� ������ ���� ������ .