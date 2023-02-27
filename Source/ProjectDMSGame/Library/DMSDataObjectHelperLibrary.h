// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Common/DMSCommons.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DMSDataObjectHelperLibrary.generated.h"

/**
 * DataObject의 BP 환경에서 사용 지원용 함수들.
 * 필요한 타입이면 상속받아서 추가하기?
 * [ BP호환을 위해선 이렇게 더러운 방식 밖에 안되는 걸까 ]
 */
UCLASS()
class PROJECTDMSGAME_API UDMSDataObjectHelperLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Data Object")
	static bool GetData_ObjectArr(UDMSDataObject* iObj, TArray<UObject*>& outData);

	UFUNCTION(BlueprintCallable, Category = "Data Object")
	static void SetData_ObjectArr(UDMSDataObject* iObj, const TArray<UObject*>& inData){iObj->Set(inData);}

	UFUNCTION(BlueprintCallable, Category = "Data Object")
	static bool GetData_Object(UDMSDataObject* iObj, UObject*& outData);

	UFUNCTION(BlueprintCallable, Category = "Data Object")
	static void SetData_Object(UDMSDataObject* iObj, UObject* inData) { iObj->Set(inData); }

	UFUNCTION(BlueprintCallable, Category = "Data Object")
	static bool GetData_NumericArr(UDMSDataObject* iObj, TArray<float>& outData);

	UFUNCTION(BlueprintCallable, Category = "Data Object")
	static void SetData_NumericArr(UDMSDataObject* iObj, const TArray<float>& inData) { iObj->Set(inData); }

	UFUNCTION(BlueprintCallable, Category = "Data Object")
	static bool GetData_Numeric(UDMSDataObject* iObj, float& outData);

	UFUNCTION(BlueprintCallable, Category = "Data Object")
	static void SetData_Numeric(UDMSDataObject* iObj, const float& inData) { iObj->Set(inData); }

	UFUNCTION(BlueprintCallable, Category = "Data Object")
	static bool GetData_StringArr(UDMSDataObject* iObj, TArray<FString>& outData);

	UFUNCTION(BlueprintCallable, Category = "Data Object")
	static void SetData_StringArr(UDMSDataObject* iObj, const TArray<FString>& inData) { iObj->Set(inData); }

	UFUNCTION(BlueprintCallable, Category = "Data Object")
	static bool GetData_String(UDMSDataObject* iObj, FString& outData);

	UFUNCTION(BlueprintCallable, Category = "Data Object")
	static void SetData_String(UDMSDataObject* iObj, const FString& inData) { iObj->Set(inData); }

	UFUNCTION(BlueprintCallable, Category = "Data Object")
	static bool GetData_NameArr(UDMSDataObject* iObj, TArray<FName>& outData);

	UFUNCTION(BlueprintCallable, Category = "Data Object")
	static void SetData_NameArr(UDMSDataObject* iObj, const TArray<FName>& inData) { iObj->Set(inData); }

	UFUNCTION(BlueprintCallable, Category = "Data Object")
	static bool GetData_Name(UDMSDataObject* iObj, FName& outData);

	UFUNCTION(BlueprintCallable, Category = "Data Object")
	static void SetData_Name(UDMSDataObject* iObj, const FName& inData) { iObj->Set(inData); }




};
