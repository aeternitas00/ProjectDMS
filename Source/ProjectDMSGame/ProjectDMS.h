// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"
#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"

/// Debugger or logger module?

PROJECTDMSGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogDMS, Log, All);

#define DMS_LOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define DMS_LOG_C(Verbosity) UE_LOG(LogDMS, Verbosity, TEXT("%s"), *DMS_LOG_CALLINFO)
#define DMS_LOG(Verbosity, Format, ...) UE_LOG(LogDMS, Verbosity, TEXT("%s %s"), *DMS_LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__)) 
#define DMS_LOG_SCREEN(Format,...) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, FString::Printf(Format,##__VA_ARGS__))