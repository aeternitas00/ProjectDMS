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

// DMSCardManagerComponent가 이것을 여러개 보유 
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
