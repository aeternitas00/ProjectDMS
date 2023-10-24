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
// ����Ʈ�� ��ġ�� ����Ʈ���� �����ϴ� �� ��� ��Ÿ�ӿ� �������� ���� �� �ְ� ���� �ϰ� ���� ��� �� ��Ʈ���ĸ� ����Ͽ� ����Ʈ ó�� ���� ����.
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
