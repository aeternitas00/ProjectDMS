// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *	ENUMS and Data Object that used often in DMS.
 *
 *	========================================
 */

#include "ProjectDMS.h"
//#include "DMSCommonDelegates.generated.h"

class UDMSSequence;

//	========================================
//				
//	========================================

DECLARE_DELEGATE(FSimpleEventSignature);
DECLARE_DELEGATE(FOnResolveCompleted);
DECLARE_DELEGATE_OneParam(FResolveIteratingDelegate, UDMSSequence*);
DECLARE_DELEGATE_OneParam(FOnApplyCompleted, UDMSSequence*);

