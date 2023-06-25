#include "Common/DMSSpawnableDataBase.h"
#include "System/DMSAssetManager.h"
#include "DMSSpawnableDataBase.h"

FPrimaryAssetId UDMSSpawnableDataBase::GetPrimaryAssetId() const
{
	return FPrimaryAssetId();
}

void ADMSSpawnableBase::Initialize(const UDMSSpawnableDataBase* inData)
{
	OriginalData = inData; 
	
	OnInitialized();
	
	PostInitialize();
}
