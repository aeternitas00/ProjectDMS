// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_GAME_MODULE
 *
 *	Managementing card piles in DMS GAME
 *
 *	========================================
 */

#include "ProjectDMS.h"
#include "Components/ActorComponent.h"
#include "DMSCardContainerComponent.h"
#include "DMSCardManagerComponent.generated.h"


class UDMSCardContainerComponent;
class ADMSCardBase;
/**
 *	========================================
 *	
 *	DMS Card Manager Component : Management playing cards, card containers, and etc...
 *	Expected to being attached to PC or PS.
 *	=> �÷��̾� ������ ī�� �����κ��� ��� �� ���� ( �ʵ� �� )
 *	 
 *	========================================
 */
UCLASS(ClassGroup = (Card), meta = (BlueprintSpawnableComponent))
class PROJECTDMSGAME_API UDMSCardManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDMSCardManagerComponent();

protected:
	// Set replicated in constructor
	// ���� ������ ������ ���� ���� �ܺο��� �����ϰ� �� ��. ( DMS PC�� �� ) 
	UPROPERTY(BlueprintReadOnly)
	TMap<FName,UDMSCardContainerComponent*> Containers; 

public:
	// Post Init ���� ������ ī�� �����̳ʵ鿡 ���� ���ø����̼� ����.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION(BlueprintCallable)
	UDMSCardContainerComponent* SearchContainer(const FName& ContainerName);
		
	void ConstructContainer(const FName& ContainerName, TSubclassOf<UDMSCardContainerComponent> ContainerClass);

	static void MigrateCard(UDMSCardContainerComponent* Origin, uint16 OrgIdx, uint16 Num, UDMSCardContainerComponent* Dest, uint16 DestIdx);
	static void MigrateCard(ADMSCardBase* Card, UDMSCardContainerComponent* Dest, uint16 DestIdx);
	static void MigrateCard(TArray<ADMSCardBase*> Cards, UDMSCardContainerComponent* Dest, uint16 DestIdx);

	UFUNCTION(BlueprintCallable)
	void AddCardtoContainer(TArray<ADMSCardBase*> Cards,const FName& ContainerName);
};