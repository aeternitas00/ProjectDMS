// Copyright JeongWoo Lee

#pragma once

/**
 * 	========================================
 *
 *	DMS_GAME_MODULE ?
 *
 *	Simple singleton getter for DMS ones.
 *
 *	========================================
 */

//#include "DMSCoreIncludes.h"
#include "Common/DMSCommons.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DMSCoreFunctionLibrary.generated.h"

class ADMSGameModeBase;
class ADMSGameStateBase;
class UDMSSeqManager;
class UDMSNotifyManager;
class UDMSEffectHandler;
class UDMSSelectorManager;
class GetDMSPhaseManager;
//enum class EDMSTimingFlag;

/**
 * 
 */
UCLASS()
class DMSCORE_API UDMSCoreFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Project DMS")
	static ADMSGameModeBase* GetDMSGameMode(UObject* WorldContext = nullptr);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Project DMS")
	static ADMSGameStateBase* GetDMSGameState(UObject* WorldContext = nullptr);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Project DMS")
	static UDMSSeqManager* GetDMSSequenceManager(UObject* WorldContext = nullptr);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Project DMS")
	static UDMSNotifyManager* GetDMSNotifyManager(UObject* WorldContext = nullptr);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Project DMS")
	static UDMSEffectHandler* GetDMSEffectHandler(UObject* WorldContext = nullptr);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Project DMS")
	static UDMSPhaseManager* GetDMSPhaseManager(UObject* WorldContext = nullptr);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Project DMS")
	static UDMSSelectorManager* GetDMSSelectorManager(UObject* WorldContext = nullptr);

	static FString GetTimingString(const EDMSTimingFlag& Flag);

	template<typename Type>
	static void ShuffleArray(FRandomStream& Stream, TArray<Type>& Array, const int32& ShuffleRange = 0);
};


template<typename Type>
void UDMSCoreFunctionLibrary::ShuffleArray(FRandomStream& Stream, TArray<Type>& Array, const int32& ShuffleRange)
{
	const int32 LastIndex = (ShuffleRange <= 0 || ShuffleRange > Array.Num() - 1) ? Array.Num() - 1 : ShuffleRange;

	for (int32 i = 0; i <= LastIndex; i += 1) {
		const int32 Index = Stream.RandRange(i, LastIndex);
		if (i == Index) {
			continue;
		}

		Array.Swap(i, Index);
	}
}