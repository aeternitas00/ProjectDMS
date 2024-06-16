// Copyright JeongWoo Lee

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
#include "DMSEffectorOwnableInterface.generated.h"

class ADMSActiveEffect;
class ADMSSequence;
class UDMSEffectSet;
class ADMSPlayerControllerBase;

UINTERFACE(MinimalAPI, BlueprintType)
class UDMSEffectorOwnableInterface : public UInterface
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
class DMSCORE_API IDMSEffectorOwnableInterface
{
	GENERATED_BODY()

public:

	// Get 'PLAYER CONTROLLER' which handle widgets for this 'player' object.
	// In default DMSGAME	-->	PlayerState will return Owner PC. 
	//						-->	GameState will return LeaderPlayer's PC.
	virtual ADMSPlayerControllerBase* GetWidgetOwner();

	virtual int32 GetID() = 0 ;
};
