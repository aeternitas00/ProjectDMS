// Copyright JeongWoo Lee

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


class UDMSSpawnableContainerComponent;
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
class PROJECTDMSGAME_API UDMSContainerManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDMSContainerManagerComponent();

protected:
	// Set replicated in constructor
	UPROPERTY(BlueprintReadOnly,VisibleInstanceOnly)
	TMap<FGameplayTag, TObjectPtr<UDMSSpawnableContainerComponent>> Containers;

	/**
	* Player's default card containers list and intancing class.
	*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FGameplayTag, TSubclassOf<UDMSSpawnableContainerComponent>> ContainerTypes;
public:
	// Post Init 이후 생성된 카드 컨테이너들에 대해 레플리케이션 실행.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION(BlueprintCallable)
	UDMSSpawnableContainerComponent* SearchContainer(const FGameplayTag& ContainerName);
		
	void ConstructContainer(const FGameplayTag& ContainerName, TSubclassOf<UDMSSpawnableContainerComponent> ContainerClass);

	static void MigrateObjects(UDMSSpawnableContainerComponent* Origin, uint16 OrgIdx, uint16 Num, UDMSSpawnableContainerComponent* Dest, uint16 DestIdx);
	static void MigrateObjects(ADMSSpawnableBase* Card, UDMSSpawnableContainerComponent* Dest, uint16 DestIdx);
	static void MigrateObjects(TArray<ADMSSpawnableBase*> Cards, UDMSSpawnableContainerComponent* Dest, uint16 DestIdx);

	UFUNCTION(BlueprintCallable)
	void AddObjectsToContainer(TArray<ADMSSpawnableBase*> Objects,const FGameplayTag& ContainerName);

	UFUNCTION(BlueprintCallable)
	TArray<ADMSSpawnableBase*> GetAllObjects();

	virtual void InitializeComponent();

	/**
	 * 
	 */
	void OnDisplayWidgetReady();
};