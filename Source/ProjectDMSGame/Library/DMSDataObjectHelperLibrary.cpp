// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/DMSDataObjectHelperLibrary.h"

bool UDMSDataObjectHelperLibrary::GetData_ObjectArr(UDMSDataObject* iObj, TArray<UObject*>& outData)
{
	if (iObj==nullptr || !iObj->TypeCheck<TArray<UObject*>>())return false;

	outData = iObj->Get<TArray<UObject*>>();
	return true;
}

bool UDMSDataObjectHelperLibrary::GetData_NumericArr(UDMSDataObject* iObj, TArray<float>& outData)
{
	if (iObj == nullptr || !iObj->TypeCheck<TArray<float>>())return false;

	outData = iObj->Get<TArray<float>>();
	return true;
}

bool UDMSDataObjectHelperLibrary::GetData_StringArr(UDMSDataObject* iObj, TArray<FString>& outData)
{
	if (iObj == nullptr || !iObj->TypeCheck<TArray<FString>>())return false;

	outData = iObj->Get<TArray<FString>>();
	return true;
}

bool UDMSDataObjectHelperLibrary::GetData_NameArr(UDMSDataObject* iObj, TArray<FName>& outData)
{
	if (iObj == nullptr || !iObj->TypeCheck<TArray<FName>>())return false;

	outData = iObj->Get<TArray<FName>>();
	return true;
}

bool UDMSDataObjectHelperLibrary::GetData_uint8Arr(UDMSDataObject* iObj, TArray<uint8>& outData)
{
	if (iObj == nullptr || !iObj->TypeCheck<TArray<uint8>>())return false;

	outData = iObj->Get<TArray<uint8>>();
	return true;
}

bool UDMSDataObjectHelperLibrary::GetData_Object(UDMSDataObject* iObj, UObject*& outData)
{
	if (iObj == nullptr || !iObj->TypeCheck<UObject*>())return false;

	outData = iObj->Get<UObject*>();
	return true;
}

bool UDMSDataObjectHelperLibrary::GetData_Numeric(UDMSDataObject* iObj, float& outData)
{
	if (iObj == nullptr || !iObj->TypeCheck<float>())return false;

	outData = iObj->Get<float>();
	return true;
}

bool UDMSDataObjectHelperLibrary::GetData_String(UDMSDataObject* iObj, FString& outData)
{
	if (iObj == nullptr || !iObj->TypeCheck<FString>())return false;

	outData = iObj->Get<FString>();
	return true;
}

bool UDMSDataObjectHelperLibrary::GetData_Name(UDMSDataObject* iObj, FName& outData)
{
	if (iObj == nullptr || !iObj->TypeCheck<FName>())return false;

	outData = iObj->Get<FName>();
	return true;
}

bool UDMSDataObjectHelperLibrary::GetData_uint8(UDMSDataObject* iObj, uint8& outData)
{
	if (iObj == nullptr || !iObj->TypeCheck<uint8>())return false;

	outData = iObj->Get<uint8>();
	return true;
}