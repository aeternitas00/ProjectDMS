// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Effect/DMSEffectDefinition.h"
#include "DMSEffect_SkillTest.generated.h"

USTRUCT(BlueprintType)
struct FDMSSelectorData_SkillTest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName StatName;

	// Get stat from ( true == SourceController / false == SourceObject ) 
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bSourceFlag;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bTestToStaticValue;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (EditCondition = "bTestToStaticValue", EditConditionHides))
	float StaticTargetValue;
};
/**
 *	�⺻������ ��ų �׽�Ʈ ����.
 *	Sequence->Source->Attribute �� Target->Attribute�� ��
 */
UCLASS()
class PROJECTDMSGAME_API UDMSEffect_SkillTest : public UDMSEffectDefinition
{
	GENERATED_BODY()
public:
	UDMSEffect_SkillTest();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FDMSSelectorData_SkillTest SkillTestData;

	static const FName SkillBonusName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Effect, meta = (EditCondition = "bIsUsingSelector", EditConditionHides))
	TSubclassOf<UDMSSelector_SkillTest> PairedWidgetClass;

	// ENum TestMethod ?
	// 1. Test with ( Source[StatName] + Static Value )
	// 2. Test with ( Source[StatName] + Target[StatName] )
	// 3. Custom? ( Multiple stat )

	virtual void Work_Implementation(UDMSEffectInstance* iEI) override; 

	virtual TSubclassOf<UDMSEffectElementSelectorWidget> GetPairedSelector_Implementation() { return PairedWidgetClass.Get(); }

	virtual void InitializePairedSelector(UDMSEffectElementSelectorWidget* WidgetInstance) override;
};

UCLASS(Blueprintable, Abstract)
class UDMSSelector_SkillTest : public UDMSEffectElementSelectorWidget
{
	GENERATED_BODY()

protected:
	//UPROPERTY(BlueprintReadOnly)
	//float V;

public:
	UPROPERTY(BlueprintReadOnly)
	FDMSSelectorData_SkillTest SkillTestData;

	virtual void OnPopupSelector_Implementation() override;
	virtual void OnCloseSelector_Implementation() override;

	virtual	bool SetupWidget_Implementation() override;

	friend class UDMSEffect_SkillTest;
};