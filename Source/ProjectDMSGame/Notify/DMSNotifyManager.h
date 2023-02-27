// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "UObject/NoExportTypes.h"
#include "DMSNotifyManager.generated.h"

/**
 * 
 */

// ��Ƽ���� �Ŵ����� ��Ƽ���� ���ù��鿡�� [ "SourceObjectClass"�� "EffectKeyword"�� (���) "TargetObjectClass"���� "Timing"(�ϱ���/�ϴ���/����) ] 
class UDMSSequence;
class IDMSEffectorInterface;

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