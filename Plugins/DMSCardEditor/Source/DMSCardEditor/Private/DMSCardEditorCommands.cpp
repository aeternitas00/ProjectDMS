// Copyright Epic Games, Inc. All Rights Reserved.

#include "DMSCardEditorCommands.h"

#define LOCTEXT_NAMESPACE "FDMSCardEditorModule"

void FDMSCardEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "DMSCardEditor", "Bring up DMSCardEditor window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
