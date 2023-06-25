//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
///**
// * 	========================================
// *
// *	DMS_CORE_MODULE
// *
// *	
// *
// *	========================================
// */
//
//#include "ProjectDMS.h"
//#include "UObject/Interface.h"
//#include "DMSPreviewInterface.generated.h"
//
//class IDMSEffectorInterface;
//
//UINTERFACE(MinimalAPI, BlueprintType)
//class UDMSPreviewInterface : public UInterface
//{
//	GENERATED_BODY()
//};
//
///**
// * 	========================================
// *
// *	IDMSEffectorInterface : ����Ʈ �ν��Ͻ��� ������ �� �ְ� ����Ʈ�� �ߵ� �� �� �ִ� �Ǵ� �� �� �ְ� ���� Ŭ������ �����ؾ� �ϴ� �������̽�
// *
// *	========================================
// */
//class PROJECTDMSGAME_API IDMSPreviewInterface
//{
//	GENERATED_BODY()
//
//public:
//
//	/**
//	 * Get preview object.
//	 */
//	virtual IDMSEffectorInterface* GetPreviewObject() = 0;
//
//	/**
//	 * Get preview object.
//	 */
//	virtual void ResetPreviewObject();
//};
