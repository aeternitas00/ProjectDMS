// Copyright JeongWoo Lee


#include "Effect/DMSEffectorActorBase.h"
#include "Effect/DMSEIManagerComponent.h"
#include "GameModes/DMSGameStateBase.h"
//#include "Player/DMSPlayerState.h"

// Sets default values
ADMSEffectorActorBase::ADMSEffectorActorBase(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	EIManagerComponent = CreateDefaultSubobject<UDMSEIManagerComponent>("EIManagerComponent");
}

//void ADMSEffectorActorBase::ResetPreviewObject()
//{
//	if (PreviewDummy!=nullptr) PreviewDummy->Destroy();
//	PreviewDummy = DuplicateObject(this, this, FName(GetName() + TEXT("_Preview")));
//	PreviewDummy->SetActorHiddenInGame(true);
//}

void ADMSEffectorActorBase::OnInitialized_Implementation() 
{
	Super::OnInitialized_Implementation();
	//ResetPreviewObject();
}
