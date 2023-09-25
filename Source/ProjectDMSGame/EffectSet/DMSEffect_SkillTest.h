// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Effect/DMSEffectDefinition.h"
#include "DMSEffect_SkillTest.generated.h"

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






	//virtual void Work_Implementation(UDMSSequence* SourceSequence, UDMSEffectInstance* iEI, const FOnExecuteCompleted& OnWorkCompleted) override;

	//virtual TSubclassOf<UDMSEffectElementSelectorWidget> GetPairedSelector_Implementation() { return EffectSelectorWidget.Get(); }

	//virtual void InitializePairedSelector(UDMSEffectElementSelectorWidget* WidgetInstance) override;
};

