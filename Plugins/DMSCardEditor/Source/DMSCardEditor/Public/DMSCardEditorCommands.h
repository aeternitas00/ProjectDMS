// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "DMSCardEditorStyle.h"

class FDMSCardEditorCommands : public TCommands<FDMSCardEditorCommands>
{
public:

	FDMSCardEditorCommands()
		: TCommands<FDMSCardEditorCommands>(TEXT("DMSCardEditor"), NSLOCTEXT("Contexts", "DMSCardEditor", "DMSCardEditor Plugin"), NAME_None, FDMSCardEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};