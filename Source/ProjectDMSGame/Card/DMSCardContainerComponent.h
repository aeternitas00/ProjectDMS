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

	Lyra�� Equipment�� AppliedEquipmentEntry ����ü�� EquipmentInstance�� TSubclassOf<ULyraEquipmentDefinition> �� ¦ �������, 
	EquipmentList�� �̸� �����ѵ�, EquipmentManagerComponent�� EquipmentList�� ���� ���·� ������.

	Item�� ��������� ��¦ �ٸ�����.. 

	����� �ƶ��� ī�� �����̳ʿ� ���� ��� �������� �ٽ� �����غ��� �ҵ�.


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
 *	������ ī�� ���̸� ��Ÿ���� ������Ʈ. �� ī�� ���̵��� DMSCardManagerComponent�� ������ �����Ῡ �����ϰ� ��.
 *	�⺻���� ������ ī�� �����, ����½ÿ� ���� ���� ���� ������ ī�� ���̰� Ư���� ���غ� ������ �� ��� ��� ���� Ŀ���� ī�� �����̳ʸ� �����Ͽ� ����ϸ� ��.
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
