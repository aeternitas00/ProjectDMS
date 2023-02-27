// Copyright Epic Games, Inc. All Rights Reserved.

#include "DMSCardEditor.h"
#include "DMSCardEditorStyle.h"
#include "DMSCardEditorCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName DMSCardEditorTabName("DMSCardEditor");

#define LOCTEXT_NAMESPACE "FDMSCardEditorModule"

void FDMSCardEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FDMSCardEditorStyle::Initialize();
	FDMSCardEditorStyle::ReloadTextures();

	FDMSCardEditorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FDMSCardEditorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FDMSCardEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FDMSCardEditorModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(DMSCardEditorTabName, FOnSpawnTab::CreateRaw(this, &FDMSCardEditorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FDMSCardEditorTabTitle", "DMSCardEditor"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FDMSCardEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FDMSCardEditorStyle::Shutdown();

	FDMSCardEditorCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(DMSCardEditorTabName);
}

TSharedRef<SDockTab> FDMSCardEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FDMSCardEditorModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("DMSCardEditor.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FDMSCardEditorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(DMSCardEditorTabName);
}

void FDMSCardEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FDMSCardEditorCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FDMSCardEditorCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDMSCardEditorModule, DMSCardEditor)