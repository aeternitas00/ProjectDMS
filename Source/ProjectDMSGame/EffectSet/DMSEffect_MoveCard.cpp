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

void UDMSEffect_MoveCard::Work_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnExecuteCompleted& OnWorkCompleted)
{
	UDMSCardContainerComponent* FromTemp;
	UDMSCardContainerComponent* Container_Destination=nullptr;
	//UDMSEIManagerComponent* CardEffector = Cast<UDMSEIManagerComponent>(iEI->GetOuter());
	ADMSCardBase* Card = Cast<ADMSCardBase>(iEI->GetApplyTarget()->GetObject());

	if (Card==nullptr)
	{	
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Outer is not card")); 
		OnWorkCompleted.ExecuteIfBound(false); 
		return; 
	}
	FromTemp = Card->GetOwningContainer();

	// ������ �÷��̾��� (Name) �����̳� / AI�� ����ߴٴ��� ������ ī�尡 ���� �ִ� �����̳��� ���� �������� (Name)�����̳� ã�Ƽ�
	UDMSCardManagerComponent* Manager = Cast<UDMSCardManagerComponent>(iEI->SourcePlayer->GetComponentByClass(UDMSCardManagerComponent::StaticClass()));
	if (Manager) {
		Container_Destination = Manager->SearchContainer(NameDestination); 
	}
	if (Container_Destination==nullptr&& FromTemp!=nullptr) {
		Container_Destination = Cast<UDMSCardManagerComponent>(FromTemp->GetOuter())->SearchContainer(NameDestination);
	}

	if (Container_Destination == nullptr)
	{
		/*No proper container.. continue; */ 
		//DMS_LOG_SCREEN(TEXT("%s : Move Card Failed"), *iEI->GetName());
		OnWorkCompleted.ExecuteIfBound(false);
		return;
	}
	else 
	{
		//DMS_LOG_SCREEN(TEXT("%s : Move Card to %s"), *iEI->GetName(), *Container_Destination->GetName());
		UDMSCardManagerComponent::MigrateCard(Card, Container_Destination, 0);
	}

	DMS_LOG_SIMPLE(TEXT("==== %s : MOVE CARD WORK COMPLETED ===="), *SourceSequence->GetName());

	OnWorkCompleted.ExecuteIfBound(true);
}

