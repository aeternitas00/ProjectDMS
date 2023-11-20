#include "Common/DMSSpawnableDataBase.h"
#include "Common/DMSSpawnableComponent.h"
//#include "System/DMSAssetManager.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "GameModes/DMSGameStateBase.h"
#include "Player/DMSPlayerStateBase.h"

FPrimaryAssetId UDMSSpawnableDataBase::GetPrimaryAssetId() const
{
	return FPrimaryAssetId();
}

void ADMSSpawnableBase::Initialize(const UDMSSpawnableDataBase* inData)
{
	OriginalData = inData; 

	ForEachComponent<UDMSSpawnableComponent>(false,[](UDMSSpawnableComponent* Comp){
		Comp->UpdateParentComponent();
	});

	OnInitialized();
	
	PostInitialize();
}
