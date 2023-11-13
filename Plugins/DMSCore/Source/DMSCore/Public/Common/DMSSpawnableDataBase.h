// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *
 *	========================================
 */

#include "DMSCoreIncludes.h"
#include "DMSSpawnableDataBase.generated.h"

/**
 * Base class of All DMS spawnable actors to support custom initialization through assets in our project.
 * 단순 스포너블 이라기 보단 좀 더 특정성있는 네이밍을 하는 것이 좋을 듯.
 */

UCLASS(Abstract)
class DMSCORE_API ADMSSpawnableBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	//ADMSSpawnableBase(){}

protected:
	/**
	 * The data asset that the actor will reference. Initialization is performed through this asset.
	 */
	UPROPERTY(BlueprintReadWrite,EditAnywhere,/*Replicated, */BlueprintGetter = GetOriginalData)
	TObjectPtr<const UDMSSpawnableDataBase> OriginalData;

	/**
	 * The player state ID of the player who owns the ownership of this actor.
	 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere,/* Replicated,*/ BlueprintGetter = GetOwnerID)
	int32 OwnerID;

public:
	/**
	 * Simple getter of OwnerID
	 */
	UFUNCTION(BlueprintCallable)
	int32 GetOwnerID() const {return OwnerID;}

	/**
	 * Simple setter of OwnerID
	 */
	UFUNCTION(BlueprintCallable)
	void SetOwnerID(const int32& inID) {OwnerID= inID;}

	/**
	 * Simple getter of OriginalData
	 */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE const UDMSSpawnableDataBase* GetOriginalData() const {return OriginalData;}

	/**
	 * Initialization function for the actor using the OriginalData.
	 */
	UFUNCTION(BlueprintCallable)
	void Initialize(const UDMSSpawnableDataBase* inData);

	/**
	 * Function to implement additional initialization logic per class after OriginalData initialized.
	 */
	UFUNCTION(BlueprintNativeEvent)

	void OnInitialized();
	virtual void OnInitialized_Implementation(){}

	/**
	 * Function to implement post-initialization behavior that will be executed immediately after the actor initialization is complete.
	 */
	UFUNCTION(BlueprintNativeEvent)

	void PostInitialize();
	virtual void PostInitialize_Implementation(){}

};


 /**
  *
  * ========================================
  *
  *	DMSSpawnableDataBase : Base of Spawnable DMS actor's data asset
  *
  * =========================================
  *
  */
UCLASS(Abstract, Blueprintable, BlueprintType, Const, ClassGroup = (Spawnable))
class DMSCORE_API UDMSSpawnableDataBase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * Class to be used when spawning an actor solely based on data.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	TSubclassOf<ADMSSpawnableBase> SpawningClass;

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	// TODO :: Asset Management Helper Funcs
};

