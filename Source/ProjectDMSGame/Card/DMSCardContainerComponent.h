// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_GAME_MODULE
 *
 *	Component that represents each pile of cards, such as a deck or hand.
 *
 *	========================================
 */

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DMSCardBase.h"
#include "DMSCardContainerComponent.generated.h"

/**
 * 
 * 
 * 
 * NOTE : 
 * USTRUCT(BlueprintType)
	struct FProjectDMSCardContainer : public FFastArraySerializer ??

	Lyra의 Equipment는 AppliedEquipmentEntry 구조체에 EquipmentInstance와 TSubclassOf<ULyraEquipmentDefinition> 를 짝 지어놓고, 
	EquipmentList에 이를 저장한뒤, EquipmentManagerComponent가 EquipmentList를 갖는 형태로 관리함.

	Item도 비슷하지만 살짝 다른구조.. 

	비슷한 맥락의 카드 컨테이너에 대해 어떻게 구성할지 다시 생각해봐야 할듯.


 */

USTRUCT(BlueprintType)
struct FDMSCardList //; public FFastArraySerializer
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<ADMSCardBase*> Cards; // or Actor?

	// Etc..

	const int Num() const {return Cards.Num();}
};

/**
 * 	========================================
 *
 *	각각의 카드 더미를 나타내는 컴포넌트. 이 카드 더미들을 DMSCardManagerComponent가 여러개 보유햐여 관리하게 됨.
 *	기본적인 형태의 카드 입출력, 입출력시에 대한 반응 등을 가지며 카드 더미가 특수한 비해비어를 가져야 할 경우 상속 받은 커스텀 카드 컨테이너를 구현하여 사용하면 됨.
 * 
 *	========================================
 */
UCLASS(Blueprintable, ClassGroup = (Card), meta = (BlueprintSpawnableComponent))
class PROJECTDMSGAME_API UDMSCardContainerComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	//UPROPERTY()
	//FName ContainerName;

public:

	/**
	 * Wrapper of card array. 
	 */
	UPROPERTY()
	FDMSCardList CardList;

protected:
	virtual void BeginPlay() override;
public:

	//Generic card pile actions
	TArray<ADMSCardBase*> PopAt(uint16 Idx);
	TArray<ADMSCardBase*> PopAt(uint16 Idx,uint16 Num);
	void Insert(TArray<ADMSCardBase*> iContainer, uint16 Idx);
	void Find(uint16 Idx, uint16 Range);
	void Remove(ADMSCardBase* iCard);
	void Remove(TArray<ADMSCardBase*> iCards);


	UFUNCTION(BlueprintCallable,BlueprintPure)
	const int GetNum() const {return CardList.Num(); }

	UFUNCTION(BlueprintCallable)
	TArray<ADMSCardBase*> GetCards() {return CardList.Cards;}
	
	// Get [Num] cards from container. if [Num] is greater than container's size, returns entire container.
	UFUNCTION(BlueprintCallable)
	TArray<ADMSCardBase*> GetTopNCards(int Num);

	UFUNCTION(BlueprintCallable)
	void ShuffleTopNCards(int Num=0);

	UFUNCTION(BlueprintPure,BlueprintCallable)
	FName GetContainerName() const {return FName(*GetName())/*ContainerName */; }

	UFUNCTION(BlueprintNativeEvent)
	void OnContainerAdded(const TArray<ADMSCardBase*>& AddedCards);
	void OnContainerAdded_Implementation(const TArray<ADMSCardBase*>& AddedCards){}
	UFUNCTION(BlueprintNativeEvent)
	void OnContainerRemoved(const TArray<ADMSCardBase*>& RemovedCards);
	void OnContainerRemoved_Implementation(const TArray<ADMSCardBase*>& RemovedCards){}
	//UFUNCTION(BlueprintImplementableEvent)
	//void OnContainerUpdated();
};
