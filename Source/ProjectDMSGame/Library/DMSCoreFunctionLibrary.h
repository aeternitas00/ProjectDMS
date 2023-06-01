// Fill out your copyright notice in the Description page of Project Settings.

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

#include "ProjectDMS.h"
#include "Common/DMSCommons.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DMSCoreFunctionLibrary.generated.h"

class ADMSGameMode;
class ADMSGameState;
class UDMSSeqManager;
class UDMSNotifyManager;
class UDMSEffectHandler;
class UDMSPhaseManager;
class UDMSConditionObjectBase;
struct FDMSCardData;

/**
 * 
 */
UCLASS()
class PROJECTDMSGAME_API UDMSCoreFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static FString GetTimingString(const EDMSTimingFlag& Flag);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Project DMS")
	static ADMSGameMode* GetDMSGameMode();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Project DMS")
	static ADMSGameState* GetDMSGameState();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Project DMS")
	static UDMSSeqManager* GetDMSSequenceManager();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Project DMS")
	static UDMSNotifyManager* GetDMSNotifyManager();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Project DMS")
	static UDMSEffectHandler* GetDMSEffectHandler();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Project DMS")
	static UDMSPhaseManager* GetDMSPhaseManager();

	// Helper function
	// Attribute Library?
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Project DMS")
	static bool GetAttributeFromActor(AActor* iActor, const FGameplayTag& Name, float& outValue);


	// Helper function
	// Card Library?
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Project DMS")
	static bool CheckCardIsCommitable(ADMSCardBase* iCard);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Project DMS")
	static bool CheckCondition(UDMSConditionObjectBase* Condition, UObject* CheckingGameObject, UDMSSequence* CurrentSequence);

	UFUNCTION(BlueprintCallable)
	static void SetDataOwner(const FDMSCardData& Data, UObject* Owner);

	template<typename Type>
	static void ShuffleArray(FRandomStream& Stream, TArray<Type>& Array, const int32& ShuffleRange);
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