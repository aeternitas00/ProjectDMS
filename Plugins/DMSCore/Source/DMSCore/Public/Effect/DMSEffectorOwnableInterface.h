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
 *	IDMSEffectorInterface : 이펙트 인스턴스가 부착될 수 있고 이펙트를 발동 할 수 있는 또는 할 수 있게 돕는 클래스가 구현해야 하는 인터페이스
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
