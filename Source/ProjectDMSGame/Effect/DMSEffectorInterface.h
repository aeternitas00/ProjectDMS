// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *	Interface that can be affected or affect other object must implements.
 *
 *	========================================
 */

#include "ProjectDMS.h"
#include "UObject/Interface.h"
#include "DMSEffectorInterface.generated.h"

class UDMSEffectInstance;
class UDMSSequence;
class UDMSEffectSet;

UINTERFACE(MinimalAPI, BlueprintType)
class UDMSEffectorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 	========================================
 *
 *	IDMSEffectorInterface : 이펙트 인스턴스가 부착될 수 있고 이펙트를 발동 할 수 있는 또는 할 수 있게 돕는 클래스가 구현해야 하는 인터페이스
 *
 *	========================================
 */
class PROJECTDMSGAME_API IDMSEffectorInterface
{
	GENERATED_BODY()

public:
	/** 
	 * 개념적으로 '실제 이펙트를 가지는 객체'를 반환 
	 * ( ex) EI Manager Comp 같은 경우 Outer(카드, 적 등등)를 반환 
	 */
	virtual UObject* GetObject();
	
	/**
	 * 
	 */
	virtual AActor* GetOwningPlayer(); // RENAME?
	
	/**
	 *
	 */
	virtual APlayerController* GetOwningPlayerController(); // RENAME?

	/**
	 * EI 부착
	 */
	virtual void AttachEffectInstance(UDMSEffectInstance* EI);
	
	/**
	 * 객체가 노티파이를 받았을 때의 응답
	 * 이펙트의 발동 기능은 일반적으로 노티파이와 떨어질 수 없는 구조기도 하여 이 인터페이스에 노티 파이 관련도 통합.
	 */
	virtual bool OnNotifyReceived(TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*>& ResponsedObjects, bool iChainable,UDMSSequence* Seq, UObject* SourceTweak=nullptr);
	
	/**
	 * 객체가 자체적으로 소유한 이펙트 세트를 반환.
	 */
	virtual UDMSEffectSet* GetOwningEffectSet(const FGameplayTag& iSetName);

	//~~
};
