// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

// DMSCardManagerComponent�� �̰��� ������ ���� 
UCLASS(Blueprintable, ClassGroup = (Card), meta = (BlueprintSpawnableComponent))
class PROJECTDMSGAME_API UDMSCardContainerComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	FDMSCardList CardList;

protected:
	virtual void BeginPlay() override;
public:
	//generic card pile action

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
	
	UFUNCTION(BlueprintNativeEvent)
	void OnContainerAdded(const TArray<ADMSCardBase*>& AddedCards);
	void OnContainerAdded_Implementation(const TArray<ADMSCardBase*>& AddedCards){}
	UFUNCTION(BlueprintNativeEvent)
	void OnContainerRemoved(const TArray<ADMSCardBase*>& RemovedCards);
	void OnContainerRemoved_Implementation(const TArray<ADMSCardBase*>& RemovedCards){}
	//UFUNCTION(BlueprintImplementableEvent)
	//void OnContainerUpdated();
};
