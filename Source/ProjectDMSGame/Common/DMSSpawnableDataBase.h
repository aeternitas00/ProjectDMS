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

#include "ProjectDMS.h"
#include "DMSSpawnableDataBase.generated.h"

/**
 * Base class of All DMS spawnable actors. 
 * 단순 스포너블 이라기 보단 좀 더 특정성있는 네이밍을 하는 것이 좋을 듯.
 */

UCLASS(Abstract)
class PROJECTDMSGAME_API ADMSSpawnableBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	//ADMSSpawnableBase(){}

protected:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TObjectPtr<const UDMSSpawnableDataBase> OriginalData;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE const UDMSSpawnableDataBase* GetOriginalData(){return OriginalData;}

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void Initialize(const UDMSSpawnableDataBase* inData);
	virtual void Initialize_Implementation(const UDMSSpawnableDataBase* inData) { OriginalData = inData; }

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
UCLASS(Abstract, Blueprintable, BlueprintType, Const, ClassGroup = (Card))
class PROJECTDMSGAME_API UDMSSpawnableDataBase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	TSubclassOf<ADMSSpawnableBase> SpawningClass;

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	// TODO :: Asset Management Helper Funcs
};