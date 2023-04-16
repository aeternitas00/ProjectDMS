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

#include "ProjectDMS.h"
//#include "DMSCommonDelegates.generated.h"

class UDMSSequence;

//	========================= //
//			Delegates
//	========================= //

DECLARE_DELEGATE(FSimpleEventSignature);
DECLARE_MULTICAST_DELEGATE(FSimpleMulticastEventSignature);

DECLARE_DELEGATE(FOnResolveCompleted);
DECLARE_DELEGATE_OneParam(FResolveIteratingDelegate, UDMSSequence*);
DECLARE_DELEGATE_OneParam(FOnApplyCompleted, UDMSSequence*);

