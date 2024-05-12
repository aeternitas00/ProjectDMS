// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectSet/DMSEffect_MoveCard.h"
#include "Effect/DMSEffectInstance.h"
#include "Effect/DMSEIManagerComponent.h"
#include "Card/DMSCardManagerComponent.h"
#include "Card/DMSCardContainerComponent.h"
#include "Card/DMSCardBase.h"
#include "Sequence/DMSSequence.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Effect_MoveCard, "Effect.MoveCard");

UDMSEffect_MoveCard::UDMSEffect_MoveCard() 
{ 
	EffectTag = TAG_DMS_Effect_MoveCard;
}

void UDMSEffect_MoveCard::Work_Implementation(ADMSSequence* SourceSequence, ADMSActiveEffect* iEI, const FOnExecuteCompleted& OnWorkCompleted)
{
	UDMSSpawnableContainerComponent* FromTemp;
	UDMSSpawnableContainerComponent* Container_Destination=nullptr;
	//UDMSEIManagerComponent* CardEffector = Cast<UDMSEIManagerComponent>(iEI->GetOuter());
	ADMSSpawnableBase* Spawnable = Cast<ADMSSpawnableBase>(iEI->GetApplyTarget());

	if (Spawnable==nullptr)
	{	
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Outer is not card")); 
		OnWorkCompleted.ExecuteIfBound(false); 
		return; 
	}
	FromTemp = Spawnable->GetOwningContainer();
	TArray<UObject*> TArr = ContainerOwner ? ContainerOwner->GetTargets(iEI,SourceSequence) : TArray<UObject*>();
	AActor* DestOwner = TArr.Num()>0 ? Cast<AActor>(TArr[0]) : iEI->SourcePlayer.Get() ;

	// 시전한 플레이어의 (Name) 컨테이너 / AI가 사용했다던가 했을땐 카드가 원래 있던 컨테이너의 주인 기준으로 (Name)컨테이너 찾아서
	auto Manager = DestOwner ? DestOwner->GetComponentByClass<UDMSContainerManagerComponent>() : nullptr;
	//Cast<UDMSContainerManagerComponent>(iEI->SourcePlayer->GetComponentByClass(UDMSContainerManagerComponent::StaticClass()));
	if (Manager) {
		Container_Destination = Manager->SearchContainer(TagDestination); 
	}

	//if (Container_Destination==nullptr&& FromTemp!=nullptr) {
	//	Container_Destination = Cast<UDMSContainerManagerComponent>(FromTemp->GetOuter())->SearchContainer(TagDestination);
	//}

	if (Container_Destination == nullptr)
	{
		/*No proper container.. continue; */ 
		DMS_LOG_SCREEN(TEXT("%s : Move Spawnable Failed"), *iEI->GetName());
		OnWorkCompleted.ExecuteIfBound(false);
		return;
	}
	else 
	{
		DMS_LOG_SCREEN(TEXT("%s : Move Spawnable to %s"), *iEI->GetName(), *Container_Destination->GetName());
		UDMSContainerManagerComponent::MigrateObjects(Spawnable, Container_Destination, 0);
	}

	DMS_LOG_SIMPLE(TEXT("==== %s : MOVE CARD WORK COMPLETED ===="), *SourceSequence->GetName());

	OnWorkCompleted.ExecuteIfBound(true);
}

bool UDMSEffect_MoveCard::Predict_Implementation(ADMSSequence* SourceSequence, ADMSActiveEffect* iEI)
{
	// retrun Owner has dest tagged container?
	return true;
}

