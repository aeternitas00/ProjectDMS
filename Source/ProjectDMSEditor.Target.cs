// Copyright JeongWoo Lee

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectDMSEditorTarget : TargetRules
{
	public ProjectDMSEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		//IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		bUsesSteam = true;
		ExtraModuleNames.AddRange( new string[] { "ProjectDMSGame" } ); // Editor Module
	}
}
