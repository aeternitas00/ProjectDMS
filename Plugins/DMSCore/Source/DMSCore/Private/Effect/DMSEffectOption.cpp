// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/DMSEffectOption.h"

void UDMSEffectOption::OnExecuteOption_Implementation(ADMSSequence* iSourceSeq, ADMSActiveEffect* iEI, const FOnOptionCompleted& OnOptionCompleted)
{
	DMS_LOG_SIMPLE(TEXT("SIMPLE"));
	OnOptionCompleted.Execute(this);
}

void UDMSEffectOption::ExecuteOption(ADMSSequence* iSourceSeq, ADMSActiveEffect* iEI, const FOnOptionCompleted& OnOptionCompleted)
{
	//OnExecuteCompleted.BindLambda(OnOptionCompleted);

	OnExecuteOption(iSourceSeq, iEI, OnOptionCompleted);
}
