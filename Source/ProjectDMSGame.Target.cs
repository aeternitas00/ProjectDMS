// Copyright JeongWoo Lee

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectDMSGameTarget : TargetRules
{
	public ProjectDMSGameTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		ExtraModuleNames.AddRange( new string[] { "ProjectDMSGame" } );
	}
}
