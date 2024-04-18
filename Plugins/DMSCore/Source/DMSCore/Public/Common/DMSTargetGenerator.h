// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DMSCoreIncludes.h"
#include "UObject/NoExportTypes.h"
#include "DMSTargetGenerator.generated.h"

class ADMSSequence;
/**
 *	Simple target generating Functor object.
 */
UCLASS(Blueprintable, BlueprintType, Abstract, EditInlineNew, meta=(DisplayName="Target Finder Base"))
class DMSCORE_API UDMSTargetGenerator : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	TArray<UObject*> GetTargets(UObject* Caller, ADMSSequence* CurrentSequence) const;
	virtual TArray<UObject*> GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const;
};

UCLASS()
class DMSCORE_API UDMSTargetGenerator_SourceObject : public UDMSTargetGenerator
{
	GENERATED_BODY()

public:
	virtual TArray<UObject*> GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const;
};

UCLASS()
class DMSCORE_API UDMSTargetGenerator_SourcePlayer : public UDMSTargetGenerator
{
	GENERATED_BODY()

public:
	virtual TArray<UObject*> GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const;
};

UCLASS()
class DMSCORE_API UDMSTargetGenerator_FollowParentSeq : public UDMSTargetGenerator
{
	GENERATED_BODY()

public:
	virtual TArray<UObject*> GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const;
};

UCLASS()
class DMSCORE_API UDMSTargetGenerator_SequenceTarget : public UDMSTargetGenerator
{
	GENERATED_BODY()

public:
	virtual TArray<UObject*> GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const;
};

UCLASS()
class DMSCORE_API UDMSTargetGenerator_Caller : public UDMSTargetGenerator
{
	GENERATED_BODY()

public:
	virtual TArray<UObject*> GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const;
};

UCLASS()
class DMSCORE_API UDMSTargetGenerator_Sequence: public UDMSTargetGenerator
{
	GENERATED_BODY()

public:
	virtual TArray<UObject*> GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const;
};

UCLASS()
class DMSCORE_API UDMSTargetGenerator_OwnerOfCaller : public UDMSTargetGenerator
{
	GENERATED_BODY()

public:
	virtual TArray<UObject*> GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const;
};

//UCLASS()
//class DMSCORE_API UDMSTargetGenerator_FromAttribute: public UDMSTargetGenerator

//UCLASS()
//class DMSCORE_API UDMSTargetGenerator_FromData : public UDMSTargetGenerator
//{
//	GENERATED_BODY()
//
//public:
//	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
//	FGameplayTag DataTag;
//
//	virtual TArray<UObject*> GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const;
//};