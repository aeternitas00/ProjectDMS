// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *	Helps getting value from std::any formed Data Objects in Blueprint.
 *
 *	========================================
 */

#include "DMSCoreIncludes.h"
#include "Common/DMSCommons.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DMSDataObjectHelperLibrary.generated.h"

struct FDMSEffectValueDef;
/**
 * 필요한 타입이면 상속받아서 추가하기?
 * [ BP호환을 위해선 이렇게 더러운 방식 밖에 안되는 걸까 ]
 */
UCLASS()
class DMSCORE_API UDMSDataObjectHelperLibrary : public UBlueprintFunctionLibrary
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

	UFUNCTION(BlueprintCallable, Category = "Data Object", meta = (Displayname = "Get Data uint8 Array"))
	static bool GetData_uint8Arr(UDMSDataObject* iObj, TArray<uint8>& outData);

	UFUNCTION(BlueprintCallable, Category = "Data Object", meta = (Displayname = "Set Data uint8 Array"))
	static void SetData_uint8Arr(UDMSDataObject* iObj, const TArray<uint8>& inData) { iObj->Set(inData); }

	UFUNCTION(BlueprintCallable, Category = "Data Object", meta = (Displayname= "Get Data uint8"))
	static bool GetData_uint8(UDMSDataObject* iObj, uint8& outData);

	UFUNCTION(BlueprintCallable, Category = "Data Object", meta = (Displayname = "Set Data uint8"))
	static void SetData_uint8(UDMSDataObject* iObj, const uint8& inData) { iObj->Set(inData); }

	template<typename T>
	static TArray<UDMSDataObject*> RawDataToDataObjectArr(TArray<T> RawDataArray, UObject* Outer = (UObject*)GetTransientPackage());

	UFUNCTION(BlueprintCallable)
	static UDMSDataObject* GetDataFromSelectionForm(FDMSValueSelectionForm SelectionForm,UDMSDataObjectSet* DataSet);
	//UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Project DMS")
	//static float SearchWithEffectValueDefs(UDMSDataObjectSet* iSet, const TArray<FDMSEffectValueDef>& DefArray, float DefaultValue=0.0f);
};

template<typename T>
TArray<UDMSDataObject*> UDMSDataObjectHelperLibrary::RawDataToDataObjectArr(TArray<T> RawDataArray, UObject* Outer)
{
	TArray<UDMSDataObject*> rv;

	for (auto& RawData : RawDataArray)
	{
		auto NewData = NewObject<UDMSDataObject>(Outer);
		NewData->Set(RawData);
		rv.Add(NewData);
	}

	return rv;
}
