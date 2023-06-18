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
 *	IDMSEffectorInterface : ����Ʈ �ν��Ͻ��� ������ �� �ְ� ����Ʈ�� �ߵ� �� �� �ִ� �Ǵ� �� �� �ְ� ���� Ŭ������ �����ؾ� �ϴ� �������̽�
 *
 *	========================================
 */
class PROJECTDMSGAME_API IDMSEffectorInterface
{
	GENERATED_BODY()

public:

	/** 
	 * ���������� '���� ����Ʈ�� ������ ��ü'�� ��ȯ 
	 * ( ex) EI Manager Comp ���� ��� Outer(ī��, �� ���)�� ��ȯ 
	 */
	virtual UObject* GetObject();

	/**
	 * Get owner of this object ( Usally PlayerState or GameState if it controlled by game.
	 */
	virtual AActor* GetOwningPlayer(); // RENAME?
	
	/**
	 * Get PlayerController if object owned by Player.
	 */
	virtual APlayerController* GetOwningPlayerController(); // RENAME?

	/**
	 * Attach effect instance to apply effects.
	 */
	virtual void AttachEffectInstance(UDMSEffectInstance* EI);
	
	/**
	 * ��ü�� ��Ƽ���̸� �޾��� ���� ����
	 * ����Ʈ�� �ߵ� ����� �Ϲ������� ��Ƽ���̿� ������ �� ���� �����⵵ �Ͽ� �� �������̽��� ��Ƽ ���� ���õ� ����.
	 */
	virtual bool OnNotifyReceived(TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*>& ResponsedObjects, bool iChainable,UDMSSequence* Seq, UObject* SourceTweak=nullptr);
	
	/**
	 * ��ü�� ��ü������ ������ ����Ʈ ��Ʈ�� ��ȯ.
	 */
	virtual UDMSEffectSet* GetOwningEffectSet(const FGameplayTag& iSetName);

	//~~
};
