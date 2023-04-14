// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *	Base of effect relative selector widget. ( RENAME? )
 *
 *	========================================
 */

#include "ProjectDMS.h"
#include "Selector/DMSConfirmWidgetBase.h"
#include "DMSEffectElementSelectorWidget.generated.h"

class UDMSEffectDefinition;

/**
 *	It is simillar to Decision Widget but has a little difference.
 *	Decision Widget is relative to Effect Node and collect data for whole effects in it. but this one relative to single Effect Definition and collect data only for that.
 *	So we decided to use this widget paired with ED, and the added members are for that. 
 *	( See the Effect_ModAtt for usage examples. )
 */
UCLASS(Blueprintable, Abstract)
class PROJECTDMSGAME_API UDMSEffectElementSelectorWidget : public UDMSConfirmWidgetBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bForEachTarget;

	UPROPERTY(BlueprintReadOnly)
	TArray<UDMSDataObject*> CandidatesData;
protected:
	UPROPERTY(BlueprintReadOnly)
	UDMSEffectDefinition* SourceEffectDefinition;

public:
	UDMSEffectElementSelectorWidget(const FObjectInitializer& ObjectInitializer) :
	UDMSConfirmWidgetBase(ObjectInitializer),bForEachTarget(false),SourceEffectDefinition(nullptr)
	{}
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UDMSDataObjectSet* MakeOutputData();
	virtual UDMSDataObjectSet* MakeOutputData_Implementation(){return NewObject<UDMSDataObjectSet>(); }

	FORCEINLINE void SetSourceEffectDefinition(UDMSEffectDefinition* iDef) { SourceEffectDefinition = iDef;}
	FORCEINLINE UDMSDataObject* GetCandidatesData(const uint8& TargetIdx) { return CandidatesData[TargetIdx]; }

};

