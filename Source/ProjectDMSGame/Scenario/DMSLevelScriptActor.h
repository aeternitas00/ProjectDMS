// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Engine/LevelScriptActor.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "DMSLevelScriptActor.generated.h"

class ADMSLocationBase;
class ADMSSpawnableBase;

USTRUCT(BlueprintType)
struct FDMSLocatingActors
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<TObjectPtr<ADMSSpawnableBase>> Actors;
};
/**
 * 
 */
UCLASS()
class PROJECTDMSGAME_API ADMSLevelScriptActor : public ALevelScriptActor, public IDMSEffectorInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<ADMSLevelScriptActor> PreviewDummy;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TMap<TObjectPtr<ADMSLocationBase>, FDMSLocatingActors> SpawnedDMSActors;

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, Category = Scenario)
	TArray<TObjectPtr<ADMSLocationBase>> SpawnedLocations;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Scenario)
	TArray<TObjectPtr<ADMSLocationBase>> StartingLocations;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Scenario)
	bool bIsStartingSelectable;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Scenario)
	class UDMSEIManagerComponent* EffectManagerComponent;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = Scenario)
	TMap<FGameplayTag, TObjectPtr<UDMSEffectSet>> ScenarioRules;

public:
	ADMSLevelScriptActor(/*const FObjectInitializer& ObjectInitializer*/);

	UFUNCTION(BlueprintCallable)
	void InitializeDMSGame(/*UPARAM(ref) TArray<ADMSLocationBase*>& Locations*/);

	UFUNCTION(BlueprintCallable)
	TArray<ADMSLocationBase*> GetStartingLocations(/*UPARAM(ref) TArray<ADMSLocationBase*>& Locations*/);

	/**
	 * Interfaces
	 */
	//virtual UObject* GetObject() override {return this;} // RENAME?
	virtual AActor* GetOwningPlayer() override {return UDMSCoreFunctionLibrary::GetDMSGameMode()->GameState;} // RENAME?
	virtual IDMSEffectorInterface* GetPreviewObject() {return PreviewDummy;}
	//virtual void AttachEffectInstance(UDMSEffectInstance* EI) override;
	//virtual bool OnNotifyReceived(TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*>& ResponsedObjects, bool iChainable, UDMSSequence* Seq, UObject* SourceTweak = nullptr) override;
	virtual UDMSEffectSet* GetOwningEffectSet(const FGameplayTag& iSetName) override;
};
