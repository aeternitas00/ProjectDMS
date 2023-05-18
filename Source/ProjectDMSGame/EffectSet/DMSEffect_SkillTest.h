// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Effect/DMSEffectDefinition.h"
#include "DMSEffect_SkillTest.generated.h"


UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Effect_SkillTest)


USTRUCT(BlueprintType)
struct FDMSSelectorData_SkillTest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FGameplayTag StatName;

	// Get stat from ( true == SourcePlayer / false == SourceObject ) 
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bGetAttributeTargetIsPlayer;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bTestToStaticValue;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (EditCondition = "bTestToStaticValue", EditConditionHides))
	float StaticTargetValue;
};
/**
 *	기본형태의 스킬 테스트 구현.
 *	Sequence->Source->Attribute 와 Target->Attribute를 비교
 */
UCLASS()
class PROJECTDMSGAME_API UDMSEffect_SkillTest : public UDMSEffectDefinition
{
	GENERATED_BODY()
public:
	UDMSEffect_SkillTest();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FDMSSelectorData_SkillTest SkillTestData;

	static const FGameplayTag SkillBonusName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Effect, meta = (EditCondition = "bIsUsingSelector", EditConditionHides))
	TSubclassOf<UDMSSelector_SkillTest> PairedWidgetClass;

	// ENum TestMethod ?
	// 1. Test with ( Source[StatName] + Static Value )
	// 2. Test with ( Source[StatName] + Target[StatName] )
	// 3. Custom? ( Multiple stat )

	virtual void Work_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnWorkCompleted& OnWorkCompleted) override;

	virtual TSubclassOf<UDMSEffectElementSelectorWidget> GetPairedSelector_Implementation() { return PairedWidgetClass.Get(); }

	virtual void InitializePairedSelector(UDMSEffectElementSelectorWidget* WidgetInstance) override;
};

UCLASS(Blueprintable, Abstract)
class UDMSSelector_SkillTest : public UDMSEffectElementSelectorWidget
{
	GENERATED_BODY()

protected:

public:
	/**
	 *
	 */
	UPROPERTY(BlueprintReadOnly)
	FDMSSelectorData_SkillTest SkillTestData;

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<ADMSCardBase>> CommitableCards;

	UPROPERTY(BlueprintReadWrite)
	float OutBonusValue;

	//virtual void OnPopupSelector_Implementation() override;
	//virtual void OnCloseSelector_Implementation() override;

	virtual UDMSDataObjectSet* MakeOutputData_Implementation() override;
	virtual	bool SetupWidget_Implementation() override;

	friend class UDMSEffect_SkillTest;
};