#pragma once

#include "DMSCoreIncludes.h"
//#include "UObject/NoExportTypes.h"
#include "DMSValueSelectorDefinition.generated.h"

class UDMSConfirmWidgetBase;
class UDMSTargetGenerator;

UCLASS(BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced, Abstract)
class DMSCORE_API UDMSValueSelectorDefinition : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	bool bIsCandidatesFromData;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bIsCandidatesFromData", EditConditionHides))
	FGameplayTag CandidatesKey;

	TSubclassOf<UDMSConfirmWidgetBase> GetWidgetClass();
};



//
// 이펙트의 수치가 디폴트값을 지정하는 것 대신 런타임에 선택으로 정할 수 있게 구성 하고 싶을 경우 이 스트럭쳐를 사용하여 이펙트 처리 로직 구성.
//
USTRUCT(BlueprintType)
struct FDMSValueSelectionForm
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool ForEachTargets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag OutDataKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TObjectPtr<UDMSValueSelectorDefinition> ValueSelector;
};



// == Samples == //


UCLASS()
class DMSCORE_API UDMSValueSelectorDefinition_Integer : public UDMSValueSelectorDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "CandidatesFlag != 0", EditConditionHides))
	bool isRanged;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "CandidatesFlag != 0 && isRanged", EditConditionHides))
	int32 Min;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "CandidatesFlag != 0 && isRanged", EditConditionHides))
	int32 Max;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "CandidatesFlag != 0 && !isRanged", EditConditionHides))
	TArray<int32> List;
};

UCLASS()
class DMSCORE_API UDMSValueSelectorDefinition_Object : public UDMSValueSelectorDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "CandidatesFlag != 0", EditConditionHides))
	TObjectPtr<UDMSTargetGenerator> CandidatesGenerator;
};
