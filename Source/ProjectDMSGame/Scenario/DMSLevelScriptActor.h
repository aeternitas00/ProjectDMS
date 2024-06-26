// Copyright JeongWoo Lee

#pragma once

#include "ProjectDMS.h"
#include "Engine/LevelScriptActor.h"
#include "Effect/DMSEffectorInterface.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "DMSLevelScriptActor.generated.h"

class ADMSLocationBase;
class ADMSSpawnableBase;
class ADMSGameModeBase;

USTRUCT(BlueprintType)
struct FDMSLocatingActors
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<TObjectPtr<ADMSSpawnableBase>> Actors;
};

/**
 * Actor for script processing within the DMS level.
 */
UCLASS()
class PROJECTDMSGAME_API ADMSLevelScriptActor : public ALevelScriptActor, public IDMSEffectorInterface
{
	GENERATED_BODY()

protected:
	//UPROPERTY()
	//TObjectPtr<ADMSLevelScriptActor> PreviewDummy;

	/**
	 * Targets managed by script actors among the actors placed in the world, excluding those used for cosmetics.
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TMap<TObjectPtr<ADMSLocationBase>, FDMSLocatingActors> SpawnedDMSActors;
	
	/**
	 * LEGACY
	 */
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, Category = Scenario)
	TArray<TObjectPtr<ADMSLocationBase>> SpawnedLocations;

	/**
	 * LEGACY
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Scenario)
	TArray<TObjectPtr<ADMSLocationBase>> StartingLocations;

	/**
	 * Whether the starting location can be chosen by the player.
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Scenario)
	bool bIsStartingSelectable;

	/**
	 * Effect manager for handling scenario effects.
	 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Scenario)
	class UDMSEIManagerComponent* EIManagerComponent;

public:

	/**
	 * Rules & effect of scenario.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = Scenario)
	TMap<FGameplayTag, TObjectPtr<UDMSEffectSet>> ScenarioRules;

public:
	ADMSLevelScriptActor(/*const FObjectInitializer& ObjectInitializer*/);

	/**
	 * 
	 */
	UFUNCTION(BlueprintCallable)
	void InitializeDMSGame(/*UPARAM(ref) TArray<ADMSLocationBase*>& Locations*/);

	/**
	 *
	 */
	UFUNCTION(BlueprintCallable)
	TArray<ADMSLocationBase*> GetStartingLocations(/*UPARAM(ref) TArray<ADMSLocationBase*>& Locations*/);

	/**
	 * Interfaces
	 */
	//virtual UObject* GetObject() override {return this;} // RENAME?
	virtual AActor* GetOwningPlayer_Implementation() override; // RENAME?
	//virtual IDMSEffectorInterface* GetPreviewObject() {return PreviewDummy;}
	//virtual void AttachEffectInstance(ADMSActiveEffect* EI) override;
	//virtual bool ReceiveNotify(TMultiMap<TScriptInterface<IDMSEffectorInterface>, ADMSActiveEffect*>& ResponsedObjects, bool iChainable, ADMSSequence* Seq, UObject* SourceTweak = nullptr) override;
	virtual UDMSEffectSet* GetOwningEffectSet(const FGameplayTag& iSetName) override;
};
