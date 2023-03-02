// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *	Notifying
 *
 *	========================================
 *	이거 사실상 SeqMan이랑 병합해도 상관이 없지않나?
 */

#include "ProjectDMS.h"
#include "UObject/NoExportTypes.h"
#include "DMSNotifyManager.generated.h"


class UDMSSequence;
class IDMSEffectorInterface;
/**
 *	========================================
 * 
 *	노티파이 매니저가 노티파이 리시버들에게 [ "SourceObjectClass"가 "EffectKeyword"를 (어떻게) "TargetObjectClass"에게 "Timing"(하기전/하는중/한후) ] 
 *
 *	========================================
 */
UCLASS()
class PROJECTDMSGAME_API UDMSNotifyManager : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TArray<TScriptInterface<IDMSEffectorInterface>> NotifyObjects;

public:

	UFUNCTION(BlueprintCallable)
	void BroadCast(UDMSSequence* NotifyData);
	UFUNCTION(BlueprintCallable)
	void RegisterNotifyObject(TScriptInterface<IDMSEffectorInterface> Object);

};

//DEPRECATED
//#define GetDMSNotifyManager() ( GetDMSGameMode() ? GetDMSGameMode()->GetNotifyManager() : nullptr )