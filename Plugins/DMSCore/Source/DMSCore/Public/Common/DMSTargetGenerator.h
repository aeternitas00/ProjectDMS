// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DMSCoreIncludes.h"
#include "UObject/NoExportTypes.h"
#include "DMSTargetGenerator.generated.h"

class ADMSSequence;
/**
 *	Simple target generating Functor object.
 *	RENAME? :: Object finder, searcher .... whatever.
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

// Expose to Editor as instanced member of BP classes.
USTRUCT(BlueprintType)
struct DMSCORE_API FDMSInstancedTargetGenerator
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced)
	TObjectPtr<UDMSTargetGenerator> TargetGenerator;
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
class DMSCORE_API UDMSTargetGenerator_FollowParentSeqApply : public UDMSTargetGenerator
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

// Return caller directly
UCLASS()
class DMSCORE_API UDMSTargetGenerator_Caller : public UDMSTargetGenerator
{
	GENERATED_BODY()

public:
	virtual TArray<UObject*> GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const;
};

// Return caller if it is DMS Spawnable, if caller is AE, return applying target of AE.
UCLASS(meta=(DisplayName="DMSTarget Generator Caller as Spawnable"))
class DMSCORE_API UDMSTargetGenerator_CallerAsActor : public UDMSTargetGenerator
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
class DMSCORE_API UDMSTargetGenerator_SequenceAEs: public UDMSTargetGenerator
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

// Sort of nesting?
UCLASS()
class DMSCORE_API UDMSTargetGenerator_UseTGAsCaller: public UDMSTargetGenerator
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced)
	TObjectPtr<UDMSTargetGenerator> CallerTG;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced)
	TObjectPtr<UDMSTargetGenerator> ResultTG;

public:
	virtual TArray<UObject*> GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const;
};

UCLASS()
class DMSCORE_API UDMSTargetGenerator_UseParentSequence: public UDMSTargetGenerator
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced)
	TObjectPtr<UDMSTargetGenerator> ParentTargetGenerator;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FGameplayTagQuery SequenceTagQuery;

public:
	virtual TArray<UObject*> GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const;
};

UCLASS()
class DMSCORE_API UDMSTargetGenerator_FromAttribute: public UDMSTargetGenerator
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced)
	TObjectPtr<UDMSTargetGenerator> AttributeSource;

	UPROPERTY(BlueprintReadWrite , EditDefaultsOnly)
	FGameplayTagQuery AttributeTagQuery;
public:
	virtual TArray<UObject*> GetTargets_Implementation(UObject* Caller, ADMSSequence* CurrentSequence) const;
};
