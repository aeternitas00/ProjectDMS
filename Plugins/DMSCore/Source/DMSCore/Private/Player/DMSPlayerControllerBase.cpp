// Copyright JeongWoo Lee


#include "Player/DMSPlayerControllerBase.h"
#include "Player/DMSPlayerStateBase.h"
#include "Sequence/DMSSeqManager.h"
#include "GameModes/DMSGameStateBase.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Selector/DMSConfirmWidgetBase.h"
#include "Selector/DMSSelectorManager.h"

ADMSPlayerControllerBase::ADMSPlayerControllerBase(const FObjectInitializer& ObjectInitializer)
	/*: Super(ObjectInitializer)*//*, InteractionMode(EDMSCardInteractionMode::PIM_Play)*/
{
	//WidgetQueue = CreateDefaultSubobject<UDMSWidgetQueue>("WidgetQueue");
}


//bool ADMSPlayerControllerBase::SetupWidgetQueue(ADMSSequence* SourceSequence, TArray<UDMSSelectorHandle*> Handles )
//{
//	//return WidgetQueue->SetupQueue(SourceSequence,Handles);
//}
