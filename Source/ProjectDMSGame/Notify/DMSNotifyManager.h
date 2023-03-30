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
//#include "Containers/Map.h"
#include "Selector/DMSDecisionWidget.h"
#include "DMSNotifyManager.generated.h"


class UDMSSequence;
class IDMSEffectorInterface;

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_System_Notify_Respondent)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_System_Notify_ActivatingEffect)

UCLASS()
class PROJECTDMSGAME_API UDMSNotifyRespondentSelector : public UDMSDecisionWidget
{
	GENERATED_BODY()

public:
	//UPROPERTY()
	TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*> ResponsedObjects;

	UPROPERTY(BlueprintReadOnly)
	TArray<TScriptInterface<IDMSEffectorInterface>> Respondents;

	//UPROPERTY(BlueprintReadOnly)
	//FGameplayTag OutTag;

	//UDMSSequence* CurrentSequence;

	UFUNCTION(BlueprintCallable)
	UDMSDataObjectSet* MakeOutputDatas(UObject* Respondent, UObject* EffectInstance);

	UFUNCTION(BlueprintCallable)
	void GetEffectInstancesFromObject(TScriptInterface<IDMSEffectorInterface> iObject, TArray<UDMSEffectInstance*>& outArray);
};
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
	UPROPERTY(BlueprintGetter = GetNotifyObjects)
	TArray<TScriptInterface<IDMSEffectorInterface>> NotifyObjects;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	TSubclassOf<UDMSNotifyRespondentSelector> ResponsedObjectSelector;
public:

	//�̰� �� �� ��ü��ȸ ���� �ϴ� ��� ������?

	template<typename FuncCompleted >
	void BroadCast(UDMSSequence* NotifyData, FuncCompleted&& ResponseCompleted);

	UFUNCTION(BlueprintCallable)
	void RegisterNotifyObject(TScriptInterface<IDMSEffectorInterface> Object);

	void CreateRespondentSelector(UDMSSequence* CurrentSequence, TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*>& ResponsedObjects);

	void CallResponseCompleted(UDMSSequence* CurrentSequence);

	UFUNCTION(BlueprintCallable,BlueprintPure)
	TArray<TScriptInterface<IDMSEffectorInterface>> GetNotifyObjects(){return NotifyObjects;}

	DECLARE_DELEGATE(FOnResponseCompleted);

	TMap<UDMSSequence*, FOnResponseCompleted> OnResponseCompleted; // MAKE IT SEQ RELATIVE
};

//DEPRECATED
//#define GetDMSNotifyManager() ( GetDMSGameMode() ? GetDMSGameMode()->GetNotifyManager() : nullptr )