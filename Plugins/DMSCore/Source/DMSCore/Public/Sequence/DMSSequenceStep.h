// Copyright JeongWoo Lee

#pragma once

//#include "DMSCoreIncludes.h"
#include "Sequence/DMSSequence.h"
#include "Common/DMSCommons.h"
#include "DMSSequenceStep.generated.h"

UENUM(BlueprintType)
enum class EDMSBroadCastFlag : uint8
{
	BF_Broadcast = 0 UMETA(DisplayName = "Broadcast"),
	BF_FreeActionWindow = 0 UMETA(DisplayName = "Broadcast with FreeAction Window"),
	BF_Unchainable UMETA(DisplayName = "Broadcast but unchainable"),
	BF_NoBroadcast UMETA(DisplayName = "No broadcast"),
};

USTRUCT(BlueprintType)
struct DMSCORE_API FDMSStepProgressMetaData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FName ProgressFunctionName;

	UPROPERTY(BlueprintReadOnly)
	EDMSBroadCastFlag ProgressBroadcastFlag;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag ProgressTag;
};

/**
 * progress unit of a sequence.
 */
UCLASS(NotBlueprintable, DefaultToInstanced, EditInlineNew)
class DMSCORE_API UDMSSequenceStep : public UObject
{
	GENERATED_BODY()

protected:
	TArray<TScriptInterface<IDMSEffectorInterface>> MainTargetQueue;
	TScriptInterface<IDMSEffectorInterface> CurrentMainTarget;

	int CurrentProgressIndex;
	int CurrentStepIndex;
	int CurrentMainTargetIndex;

public:
	UDMSSequenceStep();

	/**
	 *
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<UDMSSequenceStepDefinition>> StepDefinitions;

	/**
	 * Reference owner sequence.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<ADMSSequence> OwnerSequence;

	void InitializeStepProgress(ADMSSequence* iOwnerSequence, const TArray<TObjectPtr<UDMSSequenceStepDefinition>>& iStepDefinitions);

	void RunStepProgressQueue();

	void ExecuteNextStep();
	void ExecuteNextProgress();

	void SetNextStep(int ProgressIdx);
	void SetNextStep(const FGameplayTag& ProgressTag);

	FORCEINLINE bool IsProgressQueueFinished() const;

	UFUNCTION()
	void ProgressEnd(bool bSucceeded = true);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UDMSSequenceStepDefinition* GetCurrentStepDefinition() const {return StepDefinitions[CurrentStepIndex];}

	/**
	 * Current timing of this step.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FGameplayTag GetCurrentStepTag() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FGameplayTag GetCurrentProgressExactTag() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FGameplayTagContainer GetCurrentProgressTags() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FDMSStepProgressMetaData GetCurrentProgressData() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TScriptInterface<IDMSEffectorInterface> GetCurrentMainTarget() const;

	//FStepContext GetCurrentStepContext() const;
	/**
	 * Initiate step delegates.
	 * @param	StepInitiated			
	 * @param	StepFinished			
	 */
	//template<typename FuncInitiated, typename FuncFinished>
	//void InitializeDelegates(FuncInitiated&& StepInitiated, FuncFinished&& StepFinished);

	friend class ADMSSequence;
};

//template<typename FuncInitiated, typename FuncFinished>
//void UDMSSequenceStep::InitializeDelegates(FuncInitiated&& StepInitiated, FuncFinished&& StepFinished)
//{
//	OnStepInitiated_Delegate.AddLambda(StepInitiated);
//	OnStepFinished_Delegate.AddLambda(StepFinished);
//}


/**
 * progress unit of a sequence.
 */
UCLASS(Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew, Abstract)
class DMSCORE_API UDMSSequenceStepDefinition : public UObject
{
	GENERATED_BODY()

public:
	UDMSSequenceStepDefinition():bExecuteStepByEachMainTarget(false),EnableExecuteByEach(true),tHideOption(false){}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = Effect, meta=(EditCondition="EnableExecuteByEach",EditConditionHides))
	bool bExecuteStepByEachMainTarget;

	UPROPERTY(VisibleDefaultsOnly,Category = Effect, meta=(EditCondition="tHideOption",EditConditionHides))
	bool EnableExecuteByEach;

	UPROPERTY()
	bool tHideOption;

	UFUNCTION(BlueprintCallable)
	void BroadcastProgress(UDMSSequenceStep* InstancedStep, FName AfterFunctionName);

	//UFUNCTION()
	//void Progress_Sample(UDMSSequenceStep* InstancedStep){ /*SOME BEHAVIORS ...*/ InstancedStep->ProgressEnd(false); }

	UFUNCTION(BlueprintNativeEvent,BlueprintPure)
	TArray<FDMSStepProgressMetaData> GetOrderedProgressData() const;
	virtual TArray<FDMSStepProgressMetaData> GetOrderedProgressData_Implementation() const {return {};}

	/**
	 * Get tag of step.
	 */
	UFUNCTION(BlueprintNativeEvent,BlueprintPure)
	FGameplayTag GetPureStepTag() const;
	virtual FGameplayTag GetPureStepTag_Implementation() const {return FGameplayTag::EmptyTag;}

	/**
	 * Get tags of step ( with additional datas ).
	 */
	UFUNCTION(BlueprintNativeEvent,BlueprintPure)
	FGameplayTagContainer GetStepTag(const UDMSSequenceStep* InstancedStep) const;
	virtual FGameplayTagContainer GetStepTag_Implementation(const UDMSSequenceStep* InstancedStep) const {return FGameplayTagContainer(GetPureStepTag());}
};

