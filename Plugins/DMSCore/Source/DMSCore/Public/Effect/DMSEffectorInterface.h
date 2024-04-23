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

#include "DMSCoreIncludes.h"
#include "UObject/Interface.h"
#include "DMSEffectorInterface.generated.h"

class ADMSActiveEffect;
class ADMSSequence;
class UDMSEffectSet;
class ADMSPlayerControllerBase;
class UDMSEIManagerComponent;

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
class DMSCORE_API IDMSEffectorInterface
{
	GENERATED_BODY()

public:

	/**
	*
	*/
	virtual UDMSEIManagerComponent* GetEffectorManagerComponent();

	/** 
	 * 
	 */
	virtual AActor* GetObject();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetOwningPlayer();
	/**
	 * Get owner of this object ( Usally PlayerState or GameState if it controlled by game.
	 */
	virtual AActor* GetOwningPlayer_Implementation(); // RENAME?
	
	/**
	 * 
	 */
	virtual int32 GetOwnerPlayerID();

	/**
	 * Get PlayerController if object owned by Player.
	 */
	virtual ADMSPlayerControllerBase* GetOwningPlayerController(); // RENAME?



	/**
	 * Attach effect instance to apply effects.
	 * DEPRECATED :: Not work with replicate. might be better using someother context to create ei it self.
	 */
	//virtual void AttachEffectInstance(ADMSActiveEffect* EI);
	
	/**
	 * 객체가 노티파이를 받았을 때의 응답
	 * 이펙트의 발동 기능은 일반적으로 노티파이와 떨어질 수 없는 구조기도 하여 이 인터페이스에 노티 파이 관련도 통합.
	 */
	//virtual bool ReceiveNotify(TMultiMap<TScriptInterface<IDMSEffectorInterface>, ADMSActiveEffect*>& ResponsedObjects, bool iChainable,ADMSSequence* Seq, UObject* SourceTweak=nullptr);
	
	/**
	 * 객체가 자체적으로 소유한 이펙트 세트를 반환.
	 */
	virtual UDMSEffectSet* GetOwningEffectSet(const FGameplayTag& iSetName);

	//~~
};
