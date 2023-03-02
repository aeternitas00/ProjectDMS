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
 *	�̰� ��ǻ� SeqMan�̶� �����ص� ����� �����ʳ�?
 */

#include "ProjectDMS.h"
#include "UObject/NoExportTypes.h"
#include "DMSNotifyManager.generated.h"


class UDMSSequence;
class IDMSEffectorInterface;
/**
 *	========================================
 * 
 *	��Ƽ���� �Ŵ����� ��Ƽ���� ���ù��鿡�� [ "SourceObjectClass"�� "EffectKeyword"�� (���) "TargetObjectClass"���� "Timing"(�ϱ���/�ϴ���/����) ] 
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