#include "DMSAssetManager.h"
#include "Card/DMSCardDefinition.h"

const FPrimaryAssetType	UDMSAssetManager::CardAssetType = TEXT("Card");

UDMSAssetManager& UDMSAssetManager::Get()
{
	check(GEngine);

	if (UDMSAssetManager* Singleton = Cast<UDMSAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogDMS, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini.  It must be set to UDMSAssetManager"));

	// Fatal error above prevents this from being called.
	return *NewObject<UDMSAssetManager>();
}


UDMSCardDefinition* UDMSAssetManager::ForceLoadCard(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning)
{
	FSoftObjectPath Path = GetPrimaryAssetPath(PrimaryAssetId);

	// This does a synchronous load and may hitch
	UDMSCardDefinition* LoadedCard = Cast<UDMSCardDefinition>(Path.TryLoad());

	if (bLogWarning && LoadedCard == nullptr)
	{
		DMS_LOG_SIMPLE(TEXT("Failed to load item for identifier %s"), *PrimaryAssetId.ToString());
	}

	return LoadedCard;
}