// Copyright JeongWoo Lee

#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 * =========================================
 */

#include "Selector/DMSDecisionDefinition.h"
#include "DMSDecisionDefinition_Numeric.generated.h"

UCLASS()
class DMSCORE_API UDMSSelectorRequestForm_Numeric : public UDMSSelectorRequestForm
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TArray<float> Candidates;
};

UCLASS()
class DMSCORE_API UDMSDecisionDefinition_Number : public UDMSDecisionDefinitionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced)
	TObjectPtr<USelReqGenerator_NumericCand> Generator;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced)
	TObjectPtr<UDMSSelBehavDefinition_Number> Behavior;
};

UCLASS(BlueprintType,Blueprintable, EditInlineNew, Abstract)
class DMSCORE_API UDMSSelBehavDefinition_Number : public UDMSSelectorBehaviorDefinition
{
	GENERATED_BODY()
public:
};

UCLASS()
class DMSCORE_API UDMSSelBehavDefinition_Number_UpdateSeqAtt : public UDMSSelBehavDefinition_Number
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	FGameplayTagContainer AttributeTags;

public:
	virtual void SetupFormDelegates(ADMSSequence* Sequence, TArray<UDMSSelectorRequestForm*>& FormArr, bool SelectForEachEI);
};

UCLASS()
class DMSCORE_API USelReqGenerator_NumericCand : public UDMSSelectorRequestGenerator
{
	GENERATED_BODY()
protected:
	virtual UDMSSelectorRequestForm* CreateNewRequestForm(ADMSSequence* Sequence);
public:
	virtual void CollectCandidates(ADMSSequence* Sequence, ADMSActiveEffect* TargetEI, UDMSSelectorRequestForm* CollectingRequestForm);
	virtual TArray<float> CollectNumbers(ADMSSequence* Sequence, ADMSActiveEffect* TargetEI) {return {};}
};

UCLASS()
class DMSCORE_API USelReqGenerator_NumericCand_FromAtt : public USelReqGenerator_NumericCand
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced)
	TObjectPtr<UDMSTargetGenerator> AttributeOwners;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FGameplayTagQuery AttributeTagQuery;

	virtual TArray<float> CollectNumbers(ADMSSequence* Sequence, ADMSActiveEffect* TargetEI);
};

UCLASS()
class DMSCORE_API USelReqGenerator_NumericCand_Ranged : public USelReqGenerator_NumericCand
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced)
	TObjectPtr<UDMSTargetGenerator> MinAttributeOwner;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FGameplayTagContainer MinAttributeTag;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float MinDefaultValue;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced)
	TObjectPtr<UDMSTargetGenerator> MaxAttributeOwner;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FGameplayTagContainer MaxAttributeTag;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float MaxDefaultValue;

	virtual TArray<float> CollectNumbers(ADMSSequence* Sequence, ADMSActiveEffect* TargetEI);
};