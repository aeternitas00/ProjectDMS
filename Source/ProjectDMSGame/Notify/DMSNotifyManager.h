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
 *	노티파이 매니저가 노티파이 리시버들에게 [ "SourceObjectClass"가 "EffectKeyword"를 (어떻게) "TargetObjectClass"에게 "Timing"(하기전/하는중/한후) ] 
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

	//이거 좀 더 전체순회 없이 하는 방법 있을까?

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