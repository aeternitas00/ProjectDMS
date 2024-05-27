// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *	Base class for a task processor designed to handle similar contexts repetitively and synchronously when game events are divided into small context units
 *
 *	========================================
 */

#include "DMSCoreIncludes.h"
#include "DMSSynchronousTaskWorker.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnTaskCompleted, bool, Succeeded);

UCLASS(Abstract)
class DMSCORE_API UDMSSynchronousTaskWorker : public UObject
{
	GENERATED_BODY()

private:
	//UPROPERTY(BlueprintReadOnly)
	FOnTaskCompleted CompletedDelegate;

	//UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UObject>> Contexts;

	//UPROPERTY(BlueprintReadOnly)
	int CurrentContextIndex = 0;

	bool StopIfContFailed;
protected:
	UFUNCTION(BlueprintCallable)
	UObject* GetCurrentContext(){return Contexts[CurrentContextIndex];}

public:
	UFUNCTION(BlueprintCallable)
	void SetupTaskWorkerDelegate(const TArray<UObject*>& iContexts, const FOnTaskCompleted& inCompletedDelegate);
	
	UFUNCTION(BlueprintCallable)
	void RunTaskWorker(bool iStopIfContFailed/*WorkerOption*/);
	
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void Work();
	virtual void Work_Implementation();

	UFUNCTION(BlueprintCallable)
	void CompleteSingleTask(bool Succeeded);

	UFUNCTION(BlueprintCallable)
	void CloseTaskQueue(bool WorkerSucceeded);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void OnAllTaskCompleted(bool WorkerSucceeded);	
	virtual void OnAllTaskCompleted_Implementation(bool WorkerSucceeded);
};


