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
 * 
 *  개편예정
 */

//#include "DMSCoreIncludes.h"
#//include "Selector/DMSConfirmWidgetBase.h"
//#include "DMSEffectElementSelectorWidget.generated.h"

//class UDMSEffectDefinition;

/**
 *	It is simillar to Decision Widget but has a little difference.
 *	Decision Widget is relative to Effect Node and collect data for whole effects in it. but this one relative to single Effect Definition and collect data only for that.
 *	So we decided to use this widget paired with ED, and the added members are for that. 
 *	( See the Effect_ModAtt for usage examples. )
 */
//UCLASS(Blueprintable, Abstract)
//class DMSCORE_API UDMSEffectElementSelectorWidget : public UDMSConfirmWidgetBase
//{
//	GENERATED_BODY()
//	
//public:
//
//protected:
//
//	/**
//	 * Paired effect definition.
//	 */
//	UPROPERTY(BlueprintReadOnly)
//	UDMSEffectDefinition* SourceEffectDefinition;
//
//public:
//	UDMSEffectElementSelectorWidget(const FObjectInitializer& ObjectInitializer) :
//	UDMSConfirmWidgetBase(ObjectInitializer),SourceEffectDefinition(nullptr)
//	{}
//	
//	/**
//	 * Processes the result selected in the widget for sending it to the sequence's EI Data.
//	 * @return	New UDMSDataObjectSet. Use OutDataKey of paired ED.
//	 */
//	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
//	UDMSDataObjectSet* MakeOutputData();
//	virtual UDMSDataObjectSet* MakeOutputData_Implementation(){return NewObject<UDMSDataObjectSet>(); }
//
//	/**
//	 * Setter of SourceEffectDefinition
//	 */
//	FORCEINLINE void SetSourceEffectDefinition(UDMSEffectDefinition* iDef) { SourceEffectDefinition = iDef;}
//};

