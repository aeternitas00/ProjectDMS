// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *	Widget for "Decision Step"
 *
 * =========================================
 */

#include "ProjectDMS.h"
#include "Selector/DMSConfirmWidgetBase.h"
#include "DMSDecisionWidget.generated.h"

/**
 *	Definition of "Decision Step": It is a step that a player must go through when trying to activate an effect. 
 *	When a player tries to activate an effect, the necessary information for that effect is collected, and the resulting changes are previewed to the player based on that information. 
 *	The player can then make a final decision on whether to activate the effect while considering the preview, and the "Decision Step" ends.
 *
 *	This widget is designed for the step of "collecting the necessary information", where the player has the choice to select the information and confirm it. 
 *	For example, if the effect can be activated on "one enemy", the widget for this would prompt the player to choose from the list of available enemies as the target.
 */
UCLASS(Blueprintable, Abstract)
class PROJECTDMSGAME_API UDMSDecisionWidget : public UDMSConfirmWidgetBase
{
	GENERATED_BODY()

public:


};
