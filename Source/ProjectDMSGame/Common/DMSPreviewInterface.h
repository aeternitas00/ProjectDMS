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
// *	IDMSEffectorInterface : 이펙트 인스턴스가 부착될 수 있고 이펙트를 발동 할 수 있는 또는 할 수 있게 돕는 클래스가 구현해야 하는 인터페이스
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
