// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *	Delegates that used often in DMS.
 *
 *	========================================
 */

#include "DMSCoreIncludes.h"
//#include "DMSCommonDelegates.generated.h"

class ADMSSequence;

//	========================= //
//			Delegates
//	========================= //

//DECLARE_DYNAMIC_DELEGATE(FOnOptionCompleted);

DECLARE_MULTICAST_DELEGATE(FSimpleMulticastEventSignature);

DECLARE_DELEGATE_TwoParams(FResolveIteratingDelegate, ADMSSequence*, bool);
DECLARE_DELEGATE_TwoParams(FOnApplyCompleted, ADMSSequence*, bool);

