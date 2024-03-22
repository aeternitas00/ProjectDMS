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
 *	=> 플레이어 단위의 카드 관리부분을 담당 할 것임 ( 필드 등 )
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
	UPROPERTY(BlueprintReadOnly,VisibleInstanceOnly)
	TMap<FGameplayTag, TObjectPtr<UDMSCardContainerComponent>> Containers;

	/**
	* Player's default card containers list and intancing class.
	*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FGameplayTag, TSubclassOf<UDMSCardContainerComponent>> CardContainerTypes;
public:
	// Post Init 이후 생성된 카드 컨테이너들에 대해 레플리케이션 실행.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION(BlueprintCallable)
	UDMSCardContainerComponent* SearchContainer(const FGameplayTag& ContainerName);
		
	void ConstructContainer(const FGameplayTag& ContainerName, TSubclassOf<UDMSCardContainerComponent> ContainerClass);

	static void MigrateCard(UDMSCardContainerComponent* Origin, uint16 OrgIdx, uint16 Num, UDMSCardContainerComponent* Dest, uint16 DestIdx);
	static void MigrateCard(ADMSCardBase* Card, UDMSCardContainerComponent* Dest, uint16 DestIdx);
	static void MigrateCard(TArray<ADMSCardBase*> Cards, UDMSCardContainerComponent* Dest, uint16 DestIdx);

	UFUNCTION(BlueprintCallable)
	void AddCardtoContainer(TArray<ADMSCardBase*> Cards,const FGameplayTag& ContainerName);

	UFUNCTION(BlueprintCallable)
	TArray<ADMSCardBase*> GetAllCards();

	virtual void InitializeComponent();
};