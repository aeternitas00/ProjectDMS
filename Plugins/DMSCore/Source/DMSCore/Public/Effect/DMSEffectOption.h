// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DMSCoreIncludes.h"
#include "UObject/NoExportTypes.h"
#include "DMSEffectOption.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew, Abstract, ClassGroup = (Effect))
class DMSCORE_API UDMSEffectOption : public UObject
{
	GENERATED_BODY()
	
public:

	/*
	 * @param	OnOptionCompleted	()->void lambda param 
	 */
	template<typename FuncCompleted >
	void ExecuteOption(FuncCompleted&& OnOptionCompleted);
};

template<typename FuncCompleted>
void UDMSEffectOption::ExecuteOption(FuncCompleted&& OnOptionCompleted)
{
	// ... DO SOMETHING ...

	OnOptionCompleted();
}
